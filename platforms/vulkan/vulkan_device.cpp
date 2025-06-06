#include "vulkan_device.h"

#include "vulkan_window.h"
#include "GLFW/glfw3.h"
#include "aw/render/system/device_manager_interface.h"

#include <fmt/printf.h>

namespace aw::render
{
	namespace utils
	{
		static bool is_valid_instance_extension(const char* extension, bool clear = false)
		{
			static auto instance_extensions = vk::enumerateInstanceExtensionProperties();
			defer[clear]
			{
				if (clear)
				{
					instance_extensions.clear();
				}
			};
			return std::ranges::find_if(instance_extensions, [extension](const auto& ext) { return strcmp(ext.extensionName, extension) == 0; }) != instance_extensions.end();
		}

		static bool is_valid_instance_layer(const char* layer, bool clear = false)
		{
			static auto instance_layers = vk::enumerateInstanceLayerProperties();
			defer[clear]
			{
				if (clear)
				{
					instance_layers.clear();
				}
			};
			return std::ranges::find_if(instance_layers, [layer](const auto& l) { return strcmp(l.layerName, layer) == 0; }) != instance_layers.end();
		}
	} // namespace utils

	namespace debug
	{
		static vk::Bool32 vulkan_validation_callback(const vk::DebugUtilsMessageSeverityFlagBitsEXT message_severity, const vk::DebugUtilsMessageTypeFlagsEXT message_type, const vk::DebugUtilsMessengerCallbackDataEXT* callback_data, void* user_data)
		{
			fmt::println("(awRender) Vulkan validation layer ({}) ({}): {}", vk::to_string(message_severity), vk::to_string(message_type), callback_data->pMessage);
			return vk::False;
		}
	} // namespace debug

	VulkanDevice::VulkanDevice(VulkanWindow* window)
	{
		fmt::println("(awRender) creating vulkan device...");

		init_vulkan_instance();
		window->create_surface(m_Instance);
		m_CreatedSurfaces.push_back(window->get_surface());
		pick_physical_device(window);
	}

	VulkanDevice::~VulkanDevice()
	{
		for (const auto& surface : m_CreatedSurfaces)
		{
			(*m_Instance).destroySurfaceKHR(surface, nullptr);
		}

		fmt::println("(awRender) vulkan device destroyed.");
	}

	void VulkanDevice::init_vulkan_instance()
	{
		constexpr auto app_info = vk::ApplicationInfo()
									  .setPApplicationName("awRender")
									  .setApplicationVersion(VK_MAKE_API_VERSION(0, 1, 0, 0))
									  .setPEngineName("awRender")
									  .setEngineVersion(VK_MAKE_API_VERSION(0, 1, 0, 0))
									  .setApiVersion(VK_API_VERSION_1_3);

		core::Vector<const char*> extensions{};

		core::u32 glfw_ext_count = 0;
		const char** glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_ext_count);
		extensions.resize(glfw_ext_count);
		for (core::u32 i = 0; i < glfw_ext_count; ++i)
		{
			extensions[i] = glfw_extensions[i];
		}

		auto create_info = vk::InstanceCreateInfo()
							   .setPApplicationInfo(&app_info);

		core::Vector<const char*> validation_layers{};
		std::optional<vk::DebugUtilsMessengerCreateInfoEXT> debug_messenger_create_info{};
		if (g_enable_gpu_validation)
		{
			if (utils::is_valid_instance_extension(VK_EXT_DEBUG_UTILS_EXTENSION_NAME))
			{
				extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

				using enum vk::DebugUtilsMessageTypeFlagBitsEXT;
				using enum vk::DebugUtilsMessageSeverityFlagBitsEXT;

				debug_messenger_create_info = vk::DebugUtilsMessengerCreateInfoEXT();
				debug_messenger_create_info->setPfnUserCallback(debug::vulkan_validation_callback)
					.setMessageType(eGeneral | eValidation | ePerformance)
					.setMessageSeverity(eError | eWarning | eInfo);

				const vk::DebugUtilsMessengerCreateInfoEXT& debug_messenger_create_info_ptr = debug_messenger_create_info.value();
				create_info.setPNext(&debug_messenger_create_info_ptr);
			}

			if (utils::is_valid_instance_layer("VK_LAYER_KHRONOS_validation"))
			{
				validation_layers.push_back("VK_LAYER_KHRONOS_validation");
			}

			create_info.setPEnabledLayerNames(validation_layers);
		}

		// Set extensions here, because we need to ensure that the debug extension is added.
		create_info.setPEnabledExtensionNames(extensions);

		m_Instance = m_Context.createInstance(create_info);

		if (g_enable_gpu_validation && debug_messenger_create_info.has_value())
		{
			m_DebugMessenger = m_Instance.createDebugUtilsMessengerEXT(debug_messenger_create_info.value());
		}

		// Clear the extensions and layers if we don't need them anymore
		utils::is_valid_instance_extension("none", true);
		utils::is_valid_instance_layer("none", true);

		fmt::println("(awRender) vulkan instance created successfully.");
	}

	void VulkanDevice::pick_physical_device(const VulkanWindow* window)
	{
		for (const auto physical_devices = m_Instance.enumeratePhysicalDevices(); const auto& physical_device : physical_devices)
		{
			// Check for bindless
			const auto features = physical_device.getFeatures2<vk::PhysicalDeviceFeatures2, vk::PhysicalDeviceDescriptorIndexingFeatures>();
			const vk::PhysicalDeviceDescriptorIndexingFeatures& di_features = features.get<vk::PhysicalDeviceDescriptorIndexingFeatures>();

#define check_bool(b)   \
	if (!di_features.b) \
	continue
			check_bool(shaderSampledImageArrayNonUniformIndexing);
			check_bool(descriptorBindingSampledImageUpdateAfterBind);
			check_bool(shaderUniformBufferArrayNonUniformIndexing);
			check_bool(descriptorBindingUniformBufferUpdateAfterBind);
			check_bool(shaderStorageBufferArrayNonUniformIndexing);
			check_bool(descriptorBindingStorageBufferUpdateAfterBind);
#undef check_bool

			for (const auto queue_family_properties = physical_device.getQueueFamilyProperties(); const auto& queue_family_property : queue_family_properties)
			{
				if (queue_family_property.queueFlags & vk::QueueFlagBits::eGraphics)
				{
					if (const auto surface_formats = physical_device.getSurfaceFormatsKHR(window->get_surface()); surface_formats.size() > 0)
					{
						m_PhysicalDevice = physical_device;
						break;
					}
				}
			}

			if (*m_PhysicalDevice)
			{
				fmt::println("(awRender) Selected physical device: {}", m_PhysicalDevice.getProperties().deviceName.data());
				break;
			}
		}

		if (!*m_PhysicalDevice)
		{
			throw std::runtime_error("Failed to find a suitable GPU!");
		}
	}
	
	void VulkanDevice::create_device()
	{
	}
} // namespace aw::render