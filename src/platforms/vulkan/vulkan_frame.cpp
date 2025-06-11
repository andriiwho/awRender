#include "vulkan_frame.h"

#include "vulkan_device.h"
#include "vulkan_command_list.h"

namespace aw::render
{
	VulkanFrame::VulkanFrame()
	{
		m_Fence = static_cast<VulkanFence*>(g_vulkan_device->create_fence());
		m_ImageReadySemaphore = g_vulkan_device->get_device().createSemaphore({});
		m_RenderFinishedSemaphore = g_vulkan_device->get_device().createSemaphore({});
		m_FrameTransferSemaphore = g_vulkan_device->get_device().createSemaphore({});
		m_CommandList = static_cast<VulkanCommandList*>(g_vulkan_device->create_command_list());
	}

	void VulkanFrame::wait_and_reset()
	{
		m_Fence->wait();
	}

	DeviceCommandList* VulkanFrame::cmd() const
	{
		return m_CommandList.get();
	}
} // namespace aw::render