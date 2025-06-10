#pragma once

#include "aw/render/api/device_fence.h"

#include "vulkan_common.h"

namespace aw::render
{
	class VulkanFence final : public IDeviceFence
	{
	public:
		explicit VulkanFence(const vk::raii::Device& device);

		void wait() override;
		vk::Fence get_fence() const { return m_Fence; }

	private:
		vk::raii::Fence m_Fence{nullptr};
	};
}