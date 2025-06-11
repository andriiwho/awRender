#pragma once

#include "aw/core/all.h"
#include "aw/render/api/frame_context.h"
#include "vulkan_fence.h"
#include "vulkan_common.h"

namespace aw::render
{
	class VulkanCommandList;
	class VulkanFrame final : public IFrameContext
	{
	public:
		VulkanFrame();

		void wait_and_reset() override;

		VulkanFence* get_fence() const { return m_Fence.get(); }
		vk::Semaphore get_image_ready_semaphore() const { return m_ImageReadySemaphore; }
		vk::Semaphore get_render_finished_semaphore() const { return m_RenderFinishedSemaphore; }
		DeviceCommandList* cmd() const override;

		VulkanCommandList* vk_cmd() const { return m_CommandList.get(); }

	private:
		core::RefPtr<VulkanFence> m_Fence{};
		core::RefPtr<VulkanCommandList> m_CommandList{};
		vk::raii::Semaphore m_ImageReadySemaphore{nullptr};
		vk::raii::Semaphore m_RenderFinishedSemaphore{nullptr};
	};
}