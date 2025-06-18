#pragma once

#include "aw/render/api/swap_chain.h"

#include "vulkan_common.h"

namespace aw::render
{
	class VulkanImage;
	class VulkanDevice;
	class VulkanWindow;
	class VulkanImageView;

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
		PixelFormat get_pixel_format() const override;

		core::MulticastDelegate<> on_swap_chain_resized{};

		std::span<const core::RefPtr<VulkanImageView>> get_swap_chain_image_views() const { return m_SwapChainImageViews; }

		vk::Extent2D get_extent() const { return m_SwapChainExtent; }

	private:
		void clear_swap_chain();
		void create_swap_chain();
		void query_images();
		void init_views();

		vk::raii::SwapchainKHR m_SwapChain{nullptr};

		const VulkanWindow& m_AssociatedWindow;
		vk::SurfaceFormatKHR m_SurfaceFormat{};
		vk::Extent2D m_SwapChainExtent{};
		vk::PresentModeKHR m_PresentMode{};

		core::u32 m_CurrentImageIndex = 0;
		std::vector<core::RefPtr<VulkanImage>> m_SwapChainImages{};
		std::vector<core::RefPtr<VulkanImageView>> m_SwapChainImageViews{};
	};
} // namespace aw::render