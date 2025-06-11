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

		void copy_buffer_to_buffer(const BufferToBufferCopy& copy) override;

	private:
		vk::raii::CommandBuffer& current() { return m_CommandBuffer.at(0); }

		vk::raii::CommandPool m_CommandPool{nullptr};
		std::vector<vk::raii::CommandBuffer> m_CommandBuffer{};
	};
}