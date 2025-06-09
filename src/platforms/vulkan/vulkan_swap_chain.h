#pragma once

#include "aw/render/api/swap_chain.h"

#include "vulkan_common.h"

namespace aw::render
{
	class VulkanDevice;
	class VulkanWindow;

	extern vk::Format g_preferred_swap_chain_format;
	extern vk::Format g_fallback_swap_chain_image_format;
	extern bool g_swap_chain_vsync_enabled;

	class VulkanSwapChain final : public ISwapChain
	{
	public:
		explicit VulkanSwapChain(const VulkanWindow& associated_window);
		~VulkanSwapChain() override;

		vk::SwapchainKHR get_swap_chain() const { return m_SwapChain; }
		core::u32 get_current_image_index() const { return m_CurrentImageIndex; }

		vk::Semaphore get_image_available_semaphore() const { return m_ImageAvailableSemaphore; }

	private:
		void clear_swap_chain();
		void create_swap_chain();

		vk::raii::SwapchainKHR m_SwapChain{nullptr};

		const VulkanWindow& m_AssociatedWindow;
		vk::SurfaceFormatKHR m_SurfaceFormat{};
		vk::Extent2D m_SwapChainExtent{};
		vk::PresentModeKHR m_PresentMode{};

		core::u32 m_CurrentImageIndex = 0;
		vk::raii::Semaphore m_ImageAvailableSemaphore{nullptr};
	};
} // namespace aw::render