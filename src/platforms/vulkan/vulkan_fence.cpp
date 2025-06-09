#include "vulkan_fence.h"

#include "vulkan_device.h"

namespace aw::render
{
	VulkanFence::VulkanFence(const vk::raii::Device& device)
		: m_Fence(device.createFence(vk::FenceCreateInfo(vk::FenceCreateFlagBits::eSignaled)))
	{
	}

	void VulkanFence::wait()
	{
		if (const vk::Result result = g_vulkan_device->get_device().waitForFences(*m_Fence, true, std::numeric_limits<uint64_t>::max());
			result != vk::Result::eSuccess)
		{
			throw std::runtime_error("Failed to wait for fence");
		}
	}
} // namespace aw::render