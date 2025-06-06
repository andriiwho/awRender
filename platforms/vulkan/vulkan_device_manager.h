#pragma once

#include "aw/render/system/device_manager_interface.h"

namespace aw::render
{
	class VulkanDevice;

	class VulkanDeviceManager final : public IDeviceManagerInterface
	{
	public:
		VulkanDeviceManager() = default;
		~VulkanDeviceManager() override = default;

		IRenderWindowInterface* create_device_and_window(const RenderWindowCreateInfo& create_info, IRenderDeviceInterface** out_render_device_opt) override;

	private:
		core::RefPtr<VulkanDevice> m_Device;
	};
}