#include "vulkan_device_manager.h"

#include "vulkan_device.h"
#include "vulkan_shader_compiler.h"
#include "vulkan_window.h"

#include <stdexcept>

#include <GLFW/glfw3.h>

namespace aw::render
{
	static bool s_GlfwInitialized = false;

	VulkanDeviceManager::~VulkanDeviceManager()
	{
		if (s_GlfwInitialized)
		{
			glfwTerminate();
		}
	}

	IRenderWindowInterface* VulkanDeviceManager::create_device_and_window(const RenderWindowCreateInfo& create_info, IRenderDeviceInterface** out_render_device_opt)
	{
		if (m_Device)
		{
			throw std::runtime_error("Device already created. Cannot create another device.");
		}

		if (!s_GlfwInitialized)
		{
			if (const core::i32 result = glfwInit(); result != GLFW_TRUE)
			{
				glfwTerminate();
				throw std::runtime_error("Failed to initialize GLFW");
			}
			s_GlfwInitialized = true;
		}

		const auto out_window = aw_new VulkanWindow(create_info);
		m_Device = core::new_ref<VulkanDevice>(out_window);

		if (out_render_device_opt)
		{
			m_Device->add_ref();
			*out_render_device_opt = m_Device.get();
		}

		return out_window;
	}

	void VulkanDeviceManager::poll_os_events() const
	{
		glfwPollEvents();
	}

	IRenderDeviceInterface* VulkanDeviceManager::get_device() const
	{
		return m_Device.get();
	}

	IShaderCompiler* VulkanDeviceManager::create_shader_compiler()
	{
		return aw_new VulkanShaderCompiler;
	}
} // namespace aw::render