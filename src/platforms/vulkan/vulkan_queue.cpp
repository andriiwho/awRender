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
		const auto render_finished_semaphore = frame->get_render_finished_semaphore();

		core::u32 num_wait_semaphores = 1;
		constexpr std::array<vk::PipelineStageFlags, 2> wait_stages{ vk::PipelineStageFlagBits::eColorAttachmentOutput, vk::PipelineStageFlagBits::eTransfer };
		const std::array wait_semaphores{ frame->get_image_ready_semaphore(), frame->get_frame_transfer_semaphore() };

		// Submit transfers
		if (frame->vk_cmd()->has_any_transfer_commands())
		{
			const auto vk_cmd = frame->vk_cmd();
			auto transfer_cmd = vk_cmd->get_transfer_command_buffer();

			const auto submit_info = vk::SubmitInfo()
										 .setCommandBuffers(transfer_cmd)
										 .setSignalSemaphores(wait_semaphores[1]);
			m_Queue.submit(submit_info);

			num_wait_semaphores = 2;
		}

		const auto submit_info = vk::SubmitInfo()
									 .setCommandBuffers(cmd)
									 .setWaitSemaphoreCount(num_wait_semaphores)
									 .setPWaitSemaphores(wait_semaphores.data())
									 .setPWaitDstStageMask(wait_stages.data())
									 .setSignalSemaphores(render_finished_semaphore);
		m_Queue.submit(submit_info, frame->get_fence()->get_fence());
	}
} // namespace aw::render