#pragma once

#include "aw/render/api/frame_buffer.h"
#include "vulkan_common.h"
#include "vulkan_swap_chain.h"
#include "aw/render/api/device_views.h"

namespace aw::render
{
	class VulkanImageView;
	class VulkanRenderPass;

	class VulkanFrameBuffer final : public IFrameBuffer
	{
	public:
		explicit VulkanFrameBuffer(const FrameBufferCreateInfo& info);
		~VulkanFrameBuffer() override;

		vk::Framebuffer get_handle() const { return m_Framebuffer; }

		core::u32 get_width() const override { return m_Width; }
		core::u32 get_height() const override { return m_Height; }

	private:
		void collect_handles(const FrameBufferCreateInfo& info);
		void collect_size(const VulkanImageView* view);
		void validate_size() const;

		std::vector<core::RefPtr<VulkanImageView>> m_ColorAttachments;
		std::optional<core::RefPtr<VulkanImageView>> m_DepthStencilAttachment;

		vk::raii::Framebuffer m_Framebuffer{nullptr};
		core::RefPtr<VulkanRenderPass> m_RenderPass{};

		core::u32 m_Width{};
		core::u32 m_Height{};
		core::u32 m_Layers{1};
	};

	class VulkanSwapChainFrameBuffer final : public ISwapChainFrameBuffer
	{
	public:
		explicit VulkanSwapChainFrameBuffer(VulkanSwapChain* swap_chain, const IRenderPass* render_pass);
		~VulkanSwapChainFrameBuffer() override;

		DeviceImageView* get_current_image_view() const override;
		vk::Framebuffer get_handle() const { return m_Framebuffers[m_SwapChain->get_current_image_index()]; }

		core::u32 get_width() const override;
		core::u32 get_height() const override;

	private:
		void on_swap_chain_resized();

		std::vector<vk::raii::Framebuffer> m_Framebuffers{};
		core::RefPtr<VulkanSwapChain> m_SwapChain{};
	};
}