#pragma once

#include "aw/render/api/command_list.h"

#include "vulkan_common.h"

namespace aw::render
{
	class VulkanCommandList final : public DeviceCommandList
	{
	public:
		VulkanCommandList();
		~VulkanCommandList() override;

		void open() override;
		void close() override;

		vk::CommandBuffer get_command_buffer() const { return m_CommandBuffer.at(0);}
		vk::CommandBuffer get_transfer_command_buffer() const { return m_CommandBuffer.at(1); }

		bool has_any_transfer_commands() const { return m_HasAnyTransferCommands && m_TransferOpen; }

		void copy_buffer(DeviceBuffer* from, DeviceBuffer* to, core::u64 size) override;

		void begin_render_pass(IRenderPass* render_pass, IFrameBuffer* frame_buffer) override;
		void end_render_pass() override;

	private:
		vk::raii::CommandBuffer& cmd() { return m_CommandBuffer.at(0); }
		vk::raii::CommandBuffer& transfer() { return m_CommandBuffer.at(1); }

		void open_transfer();

		vk::raii::CommandPool m_CommandPool{nullptr};
		std::vector<vk::raii::CommandBuffer> m_CommandBuffer{};

		bool m_TransferOpen:1 = false;
		bool m_HasAnyTransferCommands:1 = false;
	};
}