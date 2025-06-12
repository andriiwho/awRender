#pragma once

#include "aw/render/system/device_manager_interface.h"

namespace aw::render
{
	class VulkanDevice;

	class VulkanDeviceManager final : public IDeviceManagerInterface
	{
	public:
		VulkanDeviceManager() = default;
		~VulkanDeviceManager() override;

		IRenderWindowInterface* create_device_and_window(const RenderWindowCreateInfo& create_info, IRenderDeviceInterface** out_render_device_opt) override;

		void poll_os_events() const override;

		[[nodiscard]] IRenderDeviceInterface* get_device() const override;
		[[nodiscard]] IShaderCompiler* create_shader_compiler() override;

	private:
		core::RefPtr<VulkanDevice> m_Device;
	};
}