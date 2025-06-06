#pragma once

#include "vulkan_common.h"
#include "aw/render/api/device_interface.h"

namespace aw::render
{
	class VulkanWindow;
	class VulkanDevice final : public IRenderDeviceInterface
	{
	public:
		explicit VulkanDevice(VulkanWindow* window);
		~VulkanDevice() override;

	private:
		void init_vulkan_instance();
		void pick_physical_device(const VulkanWindow* window);
		void create_device();

		vk::raii::Context m_Context{};
		vk::raii::Instance m_Instance{nullptr};
		core::Vector<vk::SurfaceKHR> m_CreatedSurfaces;
		vk::raii::DebugUtilsMessengerEXT m_DebugMessenger{nullptr};

		vk::raii::PhysicalDevice m_PhysicalDevice{nullptr};
		vk::raii::Device m_Device{nullptr};
	};
}