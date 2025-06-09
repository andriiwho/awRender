#pragma once

#include "aw/render/api/command_list.h"

#include "vulkan_common.h"

namespace aw::render
{
	class VulkanCommandList final : public IDeviceCommandList
	{
	public:
		VulkanCommandList();
		~VulkanCommandList() override;

		void open() override;
		void close() override;

		vk::Semaphore get_buffer_semaphore() const { return m_BufferSemaphore; }

		void lock_swap_chain(ISwapChain* swap_chain) override;

	private:
		vk::raii::CommandBuffer& current() { return m_CommandBuffer.at(0); }

		vk::raii::CommandPool m_CommandPool{nullptr};
		std::vector<vk::raii::CommandBuffer> m_CommandBuffer{};
		vk::raii::Semaphore m_BufferSemaphore{nullptr};

		core::Vector<vk::Semaphore> m_SwapChainSemaphores{};
	};
}