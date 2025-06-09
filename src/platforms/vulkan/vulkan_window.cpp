#include "vulkan_window.h"

#include "GLFW/glfw3.h"

namespace aw::render
{

	VulkanWindow::VulkanWindow(const RenderWindowCreateInfo& create_info)
	{
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		m_Window = glfwCreateWindow(create_info.width, create_info.height, create_info.title.data(), nullptr, nullptr);

		if (!m_Window)
		{
			throw std::runtime_error("Failed to create window");
		}
	}

	VulkanWindow::~VulkanWindow()
	{
		if (m_Window)
		{
			glfwDestroyWindow(m_Window);
		}
	}

	void VulkanWindow::create_surface(const vk::Instance instance)
	{
		VkSurfaceKHR c_surface{};
		if (const VkResult result = glfwCreateWindowSurface(instance, m_Window, nullptr, &c_surface); result != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create window surface");
		}
		m_Surface = vk::SurfaceKHR(c_surface);
	}

	bool VulkanWindow::should_close() const
	{
		if (m_Window)
		{
			return glfwWindowShouldClose(m_Window) == GLFW_TRUE;
		}

		// If we have no window, it means it's either closed or wasn't opened at all. Hence...
		return true;
	}
} // namespace aw::render