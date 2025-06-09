#include "vulkan_swap_chain.h"

#include "vulkan_device.h"
#include "vulkan_window.h"

namespace aw::render
{
	vk::Format g_preferred_swap_chain_format = vk::Format::eB8G8R8A8Unorm;
	vk::Format g_fallback_swap_chain_format = vk::Format::eR8G8B8A8Unorm;
	bool g_swap_chain_vsync_enabled = false;

	VulkanSwapChain::VulkanSwapChain(const VulkanWindow& associated_window)
		: m_AssociatedWindow(associated_window)
	{
		create_swap_chain();
	}

	VulkanSwapChain::~VulkanSwapChain()
	{
	}

	void VulkanSwapChain::clear_swap_chain()
	{
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
			const auto present_modes = g_vulkan_device->get_physical_device().getSurfacePresentModesKHR(surface);
			if (g_swap_chain_vsync_enabled)
			{
				if (const auto iter = std::ranges::find_if(present_modes, [](const auto& mode) { return mode == vk::PresentModeKHR::eMailbox; }); iter != present_modes.end())
				{
					return *iter;
				}

				if (const auto iter = std::ranges::find_if(present_modes, [](const auto& mode) { return mode == vk::PresentModeKHR::eFifo; }); iter != present_modes.end())
				{
					return *iter;
				}
			}
			else
			{
				if (const auto iter = std::ranges::find_if(present_modes, [](const auto& mode) { return mode == vk::PresentModeKHR::eFifoRelaxed; }); iter != present_modes.end())
				{
					return *iter;
				}
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
} // namespace aw::render