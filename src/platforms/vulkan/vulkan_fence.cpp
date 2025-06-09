#include "vulkan_fence.h"

namespace aw::render
{
	VulkanFence::VulkanFence(const vk::raii::Device& device)
		: m_Fence(device.createFence(vk::FenceCreateInfo(vk::FenceCreateFlagBits::eSignaled)))
	{
	}
} // namespace aw::render