#pragma once

#include "aw/render/api/device_resources.h"
#include "vk_mem_alloc.h"

#include "vulkan_common.h"

namespace aw::render
{
	class VulkanBuffer final : public DeviceBuffer
	{
	public:
		explicit VulkanBuffer(DeviceBufferCreateInfo&& info);
		~VulkanBuffer() override;

		vk::Buffer get_handle() const { return m_Buffer; }

	private:
		VmaAllocation m_Allocation{};
		vk::raii::Buffer m_Buffer{nullptr};
	};
}