#pragma once

#include "aw/render/api/swap_chain.h"

#include "vulkan_common.h"

namespace aw::render
{
	class VulkanImage;
	class VulkanDevice;
	class VulkanWindow;

	extern vk::Format g_preferred_swap_chain_format;
	extern vk::Format g_fallback_swap_chain_image_format;

	class VulkanSwapChain final : public ISwapChain
	{
	public:
		explicit VulkanSwapChain(const VulkanWindow& associated_window);
		~VulkanSwapChain() override;

		vk::SwapchainKHR get_swap_chain() const { return m_SwapChain; }
		core::u32 get_current_image_index() const { return m_CurrentImageIndex; }

		core::u32 acquire_next_image(IFrameContext* frame_context) override;

	private:
		void clear_swap_chain();
		void create_swap_chain();
		void query_images();

		vk::raii::SwapchainKHR m_SwapChain{nullptr};

		const VulkanWindow& m_AssociatedWindow;
		vk::SurfaceFormatKHR m_SurfaceFormat{};
		vk::Extent2D m_SwapChainExtent{};
		vk::PresentModeKHR m_PresentMode{};

		core::u32 m_CurrentImageIndex = 0;
		core::Vector<core::RefPtr<VulkanImage>> m_SwapChainImages{};
	};
} // namespace aw::render