#include "vulkan_queue.h"

#include "vulkan_swap_chain.h"
#include "vulkan_frame.h"
#include "vulkan_command_list.h"

namespace aw::render
{
	VulkanDeviceQueue::VulkanDeviceQueue(const vk::Queue queue, const core::u32 queue_family_index, const DeviceQueueType queue_type)
		: m_Queue(queue)
		, m_QueueFamilyIndex(queue_family_index)
		, m_QueueType(queue_type)
	{
	}

	VulkanDeviceQueue::~VulkanDeviceQueue()
	{
		m_Queue.waitIdle();
	}

	void VulkanDeviceQueue::present_swap_chain(IFrameContext* ctx, ISwapChain* swap_chain)
	{
		const VulkanSwapChain* vulkan_swap_chain = static_cast<VulkanSwapChain*>(swap_chain);

		const vk::SwapchainKHR handle = vulkan_swap_chain->get_swap_chain();
		const core::u32 image_index = vulkan_swap_chain->get_current_image_index();

		vk::Semaphore render_finished_semaphore = static_cast<VulkanFrame*>(ctx)->get_render_finished_semaphore();

		const auto present_info = vk::PresentInfoKHR()
									  .setSwapchains(handle)
									  .setImageIndices(image_index)
									  .setWaitSemaphores(render_finished_semaphore);
		if (const vk::Result result = m_Queue.presentKHR(present_info); result != vk::Result::eSuccess)
		{
			throw std::runtime_error("Failed to present swap chain");
		}
	}

	void VulkanDeviceQueue::submit(IFrameContext* ctx)
	{
		const auto frame = static_cast<VulkanFrame*>(ctx);
		const auto cmd = frame->vk_cmd()->get_command_buffer();
		const auto image_ready_semaphore = frame->get_image_ready_semaphore();
		const auto render_finished_semaphore = frame->get_render_finished_semaphore();
		constexpr vk::PipelineStageFlags wait_mask = vk::PipelineStageFlagBits::eColorAttachmentOutput;

		const auto submit_info = vk::SubmitInfo()
			.setCommandBuffers(cmd)
			.setWaitSemaphores(image_ready_semaphore)
			.setWaitDstStageMask(wait_mask)
			.setSignalSemaphores(render_finished_semaphore);
		m_Queue.submit(submit_info, frame->get_fence()->get_fence());
	}
} // namespace aw::render