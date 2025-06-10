#include "vulkan_buffer.h"

#include "vulkan_device.h"

namespace aw::render
{
	namespace detail
	{

		static vk::BufferUsageFlags to_vulkan_buffer_flags(const core::EnumMask<DeviceBufferUsage> mask)
		{
			vk::BufferUsageFlags flags{};
			if (mask.contains(DeviceBufferUsage::vertex))
				flags |= vk::BufferUsageFlagBits::eVertexBuffer;
			if (mask.contains(DeviceBufferUsage::index))
				flags |= vk::BufferUsageFlagBits::eIndexBuffer;
			if (mask.contains(DeviceBufferUsage::uniform))
				flags |= vk::BufferUsageFlagBits::eUniformBuffer;
			if (mask.contains(DeviceBufferUsage::storage))
				flags |= vk::BufferUsageFlagBits::eStorageBuffer;
			if (mask.contains(DeviceBufferUsage::copy_src))
				flags |= vk::BufferUsageFlagBits::eTransferSrc;
			if (mask.contains(DeviceBufferUsage::copy_dst))
				flags |= vk::BufferUsageFlagBits::eTransferDst;
			return flags;
		}
	} // namespace detail

	VulkanBuffer::VulkanBuffer(DeviceBufferCreateInfo&& info)
		: DeviceBuffer(std::move(info))
	{
		const auto create_info = vk::BufferCreateInfo()
									 .setSize(get_create_info().size_in_bytes)
									 .setUsage(detail::to_vulkan_buffer_flags(get_create_info().usage))
									 .setSharingMode(vk::SharingMode::eExclusive);

		VmaAllocationCreateInfo alloc_info{
			.flags = 0,
			.usage = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE,
		};

		if (get_create_info().usage.contains(DeviceBufferUsage::sequential_write))
		{
			alloc_info.flags |= VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;
		}

		if (get_create_info().usage.contains(DeviceBufferUsage::random_access))
		{
			alloc_info.flags |= VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT;
		}

		if (get_create_info().usage.contains(DeviceBufferUsage::initially_mapped))
		{
			alloc_info.flags |= VMA_ALLOCATION_CREATE_MAPPED_BIT;
		}

		VkBufferCreateInfo c_create_info = create_info;
		VkBuffer buffer{};
		if (const VkResult result = vmaCreateBuffer(g_vulkan_device->get_allocator(), &c_create_info, &alloc_info, &buffer, &m_Allocation, nullptr);
			result != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to allocate memory for buffer");
		}

		m_Buffer = vk::raii::Buffer(g_vulkan_device->get_device(), buffer);
	}

	VulkanBuffer::~VulkanBuffer()
	{
		if (m_Allocation)
		{
			vmaFreeMemory(g_vulkan_device->get_allocator(), m_Allocation);
			m_Allocation = nullptr;
		}
	}
} // namespace aw::render