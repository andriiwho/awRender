#include "vulkan_image.h"

#include "vulkan_device.h"
#include "vulkan_pixel_format.h"

namespace aw::render
{
	namespace detail
	{
		static constexpr vk::ImageUsageFlags to_vulkan_image_usage_flags(const core::EnumMask<DeviceImageUsage> usage)
		{
			vk::ImageUsageFlags flags{};
			if (usage.contains(DeviceImageUsage::sampled))
				flags |= vk::ImageUsageFlagBits::eSampled;
			if (usage.contains(DeviceImageUsage::storage))
				flags |= vk::ImageUsageFlagBits::eStorage;
			if (usage.contains(DeviceImageUsage::color_attachment))
				flags |= vk::ImageUsageFlagBits::eColorAttachment;
			if (usage.contains(DeviceImageUsage::depth_stencil_attachment))
				flags |= vk::ImageUsageFlagBits::eDepthStencilAttachment;
			if (usage.contains(DeviceImageUsage::copy_src))
				flags |= vk::ImageUsageFlagBits::eTransferSrc;
			if (usage.contains(DeviceImageUsage::copy_dst))
				flags |= vk::ImageUsageFlagBits::eTransferDst;

			return flags;
		}

		static constexpr vk::ImageType to_vulkan_image_type(DeviceImageType type)
		{
			switch (type)
			{
				case DeviceImageType::image_1d:
					return vk::ImageType::e1D;
				case DeviceImageType::image_2d:
					return vk::ImageType::e2D;
					break;
				case DeviceImageType::image_3d:
					return vk::ImageType::e3D;
					break;
			}

			return vk::ImageType::e2D;
		}
	} // namespace detail

	VulkanImage::VulkanImage(DeviceImageCreateInfo&& info)
		: DeviceImage(std::move(info))
	{
		const auto create_info = vk::ImageCreateInfo()
									 .setImageType(detail::to_vulkan_image_type(get_create_info().type))
									 .setFormat(to_vulkan_format(get_create_info().format))
									 .setExtent({ get_create_info().width, get_create_info().height, get_create_info().depth })
									 .setMipLevels(get_create_info().mip_levels)
									 .setArrayLayers(get_create_info().array_layers)
									 .setSamples(vk::SampleCountFlagBits::e1)
									 .setTiling(vk::ImageTiling::eOptimal)
									 .setUsage(detail::to_vulkan_image_usage_flags(get_create_info().usage))
									 .setSharingMode(vk::SharingMode::eExclusive)
									 .setInitialLayout(vk::ImageLayout::eUndefined);

		VmaAllocationCreateInfo alloc_info {
			.flags = 0,
			.usage = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE,
		};
		VkImage out_image{};
		const VkImageCreateInfo c_create_info = create_info;
		if (const VkResult result = vmaCreateImage(g_vulkan_device->get_allocator(), &c_create_info, &alloc_info, &out_image, &m_Allocation, nullptr);
			result != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create vulkan image");
		}

		m_ImageHandle = vk::raii::Image(g_vulkan_device->get_device(), out_image);
	}

	VulkanImage::VulkanImage(DeviceImageCreateInfo&& info, vk::Image unowned_image)
		: DeviceImage(std::move(info))
		, m_ImageHandle(unowned_image)
	{
	}

	VulkanImage::~VulkanImage()
	{
		if (owns_image())
		{
			vmaFreeMemory(g_vulkan_device->get_allocator(), m_Allocation);
			m_Allocation = nullptr;
		}

		m_ImageHandle = std::monostate{};
	}
} // namespace aw::render