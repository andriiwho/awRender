#pragma once

#include "aw/render/system/render_window_interface.h"
#include "vulkan_common.h"

extern "C" typedef struct GLFWwindow GLFWwindow;

namespace aw::render
{
	class VulkanWindow final : public IRenderWindowInterface
	{
	public:
		explicit VulkanWindow(const RenderWindowCreateInfo& create_info);
		~VulkanWindow() override;

		void create_surface(vk::Instance instance);
		vk::SurfaceKHR get_surface() const { return m_Surface; }

	private:
		GLFWwindow* m_Window{};
		vk::SurfaceKHR m_Surface{};
	};
}