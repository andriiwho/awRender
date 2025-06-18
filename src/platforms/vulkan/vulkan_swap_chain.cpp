#include "vulkan_swap_chain.h"

#include "vulkan_device.h"
#include "vulkan_frame.h"
#include "vulkan_window.h"
#include "vulkan_image.h"
#include "vulkan_image_view.h"
#include "vulkan_pixel_format.h"
#include "aw/render/api/device_views.h"
#include "fmt/ostream.h"

namespace aw::render
{
	vk::Format g_preferred_swap_chain_format = vk::Format::eB8G8R8A8Unorm;
	vk::Format g_fallback_swap_chain_format = vk::Format::eR8G8B8A8Unorm;

	VulkanSwapChain::VulkanSwapChain(const VulkanWindow& associated_window)
		: m_AssociatedWindow(associated_window)
	{
		create_swap_chain();
		query_images();
		init_views();
	}

	VulkanSwapChain::~VulkanSwapChain()
	{
	}

	core::u32 VulkanSwapChain::acquire_next_image(IFrameContext* frame_context)
	{
		if (!frame_context)
		{
			throw std::runtime_error("Frame context is null.");
		}

		const VulkanFrame* vk_frame = static_cast<VulkanFrame*>(frame_context);
		auto [result, id] = m_SwapChain.acquireNextImage(UINT64_MAX, vk_frame->get_image_ready_semaphore(), nullptr);

		if (result != vk::Result::eSuccess)
		{
			throw std::runtime_error("Failed to acquire next image.");
		}

		m_CurrentImageIndex = id;
		return m_CurrentImageIndex;
	}

	PixelFormat VulkanSwapChain::get_pixel_format() const
	{
		return to_pixel_format(m_SurfaceFormat.format);
	}

	void VulkanSwapChain::clear_swap_chain()
	{
		m_SwapChainImageViews.clear();
		m_SwapChainImages.clear();
	}

	namespace detail
	{
		static vk::SurfaceFormatKHR choose_surface_format(const vk::SurfaceKHR surface)
		{
			const auto surface_formats = g_vulkan_device->get_physical_device().getSurfaceFormatsKHR(surface);
			if (const auto iter = std::ranges::find_if(surface_formats, [](const auto& format) { return format.format == g_preferred_swap_chain_format; }); iter != surface_formats.end())
			{
				return *iter;
			}

			if (const auto iter = std::ranges::find_if(surface_formats, [](const auto& format) { return format.format == g_fallback_swap_chain_format; }); iter != surface_formats.end())
			{
				return *iter;
			}

			return surface_formats[0];
		}

		static vk::Extent2D choose_swap_chain_extent(const vk::SurfaceCapabilitiesKHR& surface_capabilities)
		{
			return vk::Extent2D(
				core::Math::clamp(surface_capabilities.currentExtent.width, surface_capabilities.minImageExtent.width, surface_capabilities.maxImageExtent.width),
				core::Math::clamp(surface_capabilities.currentExtent.height, surface_capabilities.minImageExtent.height, surface_capabilities.maxImageExtent.height));
		}

		static vk::PresentModeKHR choose_swap_chain_present_mode(const vk::SurfaceKHR surface)
		{
			// TODO: There is a problem when vk::PresentModeKHR::eFifo is set. Need to find a better strategy for storing semaphores. For now only Mailbox will be supported.

			const auto present_modes = g_vulkan_device->get_physical_device().getSurfacePresentModesKHR(surface);
			if (const auto iter = std::ranges::find_if(present_modes, [](const auto& mode) { return mode == vk::PresentModeKHR::eMailbox; }); iter != present_modes.end())
			{
				return *iter;
			}

			return vk::PresentModeKHR::eFifo;
		}
	} // namespace detail

	void VulkanSwapChain::create_swap_chain()
	{
		clear_swap_chain();

		const vk::SurfaceCapabilitiesKHR surface_capabilities = g_vulkan_device->get_physical_device().getSurfaceCapabilitiesKHR(m_AssociatedWindow.get_surface());
		m_SurfaceFormat = detail::choose_surface_format(m_AssociatedWindow.get_surface());
		m_SwapChainExtent = detail::choose_swap_chain_extent(surface_capabilities);
		m_PresentMode = detail::choose_swap_chain_present_mode(m_AssociatedWindow.get_surface());
		fmt::println("Selected present mode: {}", vk::to_string(m_PresentMode));

		const auto create_info = vk::SwapchainCreateInfoKHR()
									 .setSurface(m_AssociatedWindow.get_surface())
									 .setMinImageCount(3)
									 .setImageFormat(m_SurfaceFormat.format)
									 .setImageColorSpace(m_SurfaceFormat.colorSpace)
									 .setImageExtent(m_SwapChainExtent)
									 .setImageArrayLayers(1)
									 .setImageUsage(vk::ImageUsageFlagBits::eColorAttachment)
									 .setImageSharingMode(vk::SharingMode::eExclusive)
									 .setPreTransform(surface_capabilities.currentTransform)
									 .setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque)
									 .setPresentMode(m_PresentMode)
									 .setClipped(true)
									 .setOldSwapchain(m_SwapChain);
		m_SwapChain = g_vulkan_device->get_device().createSwapchainKHR(create_info);
	}

	void VulkanSwapChain::query_images()
	{
		const auto images = m_SwapChain.getImages();
		m_SwapChainImages.reserve(images.size());
		for (const vk::Image image : images)
		{
			DeviceImageCreateInfo create_info{
				.debug_name = "SwapChain Image",
				.type = DeviceImageType::image_2d,
				.mip_levels = 1,
				.array_layers = 1,
				.width = m_SwapChainExtent.width,
				.height = m_SwapChainExtent.height,
				.depth = 1,
				.format = to_pixel_format(m_SurfaceFormat.format),
				.usage = DeviceImageUsage::color_attachment,
			};
			m_SwapChainImages.emplace_back(aw_new VulkanImage(std::move(create_info), image));
		}
	}

	void VulkanSwapChain::init_views()
	{
		for (const auto& image : m_SwapChainImages)
		{
			DeviceImageViewCreateInfo create_info {
				.debug_name = "SwapChain Image View",
				.aspect = DeviceResourceViewAspect::color,
				.dimensions = DeviceResourceViewDimensions::image_2d,
				.mip_levels = 1,
				.base_mip_level = 0,
				.array_layers = 1,
				.base_array_layer = 0
			};
			m_SwapChainImageViews.push_back(aw_new VulkanImageView(image, std::move(create_info)));
		}
	}
} // namespace aw::render