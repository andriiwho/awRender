#define VMA_IMPLEMENTATION
#include "vulkan_device.h"

#include "vulkan_buffer.h"
#include "vulkan_command_list.h"
#include "vulkan_fence.h"
#include "vulkan_frame.h"
#include "vulkan_image.h"
#include "vulkan_image_view.h"
#include "vulkan_queue.h"
#include "vulkan_swap_chain.h"
#include "vulkan_window.h"
#include "GLFW/glfw3.h"
#include "aw/render/system/device_manager_interface.h"

#include <ranges>
#include <fmt/printf.h>

namespace aw::render
{
	static constexpr std::array s_required_device_extensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME,
		VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME,
		VK_KHR_PUSH_DESCRIPTOR_EXTENSION_NAME
	};

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
			fflush(stdout);
			return vk::False;
		}
	} // namespace debug

	VulkanDevice::VulkanDevice(VulkanWindow* window)
	{
		g_vulkan_device = this;
		fmt::println("(awRender) creating vulkan device...");

		init_vulkan_instance();
		window->create_surface(m_Instance);
		m_CreatedSurfaces.push_back(window->get_surface());
		pick_physical_device(window);
		create_device();
		init_allocator();
	}

	VulkanDevice::~VulkanDevice()
	{
		m_Device.waitIdle();

		if (m_Allocator)
		{
			vmaDestroyAllocator(m_Allocator);
		}

		for (const auto& surface : m_CreatedSurfaces)
		{
			(*m_Instance).destroySurfaceKHR(surface, nullptr);
		}

		fmt::println("(awRender) vulkan device destroyed.");
	}

	IDeviceQueue* VulkanDevice::create_device_queue(const DeviceQueueType queue_type)
	{
		core::u32 queue_index = 0;
		switch (queue_type)
		{
			case DeviceQueueType::graphics:
				queue_index = m_GraphicsQueueIndex;
				break;
			default:
				throw std::runtime_error("Unsupported queue type!");
		}

		try
		{
			return aw_new VulkanDeviceQueue(m_Device.getQueue(queue_index, 0), queue_index, queue_type);
		}
		catch (const std::exception& e)
		{
			fmt::println("(awRender) Failed to create device queue: {}", e.what());
			return nullptr;
		}
	}

	IDeviceFence* VulkanDevice::create_fence()
	{
		return aw_new VulkanFence(m_Device);
	}

	DeviceCommandList* VulkanDevice::create_command_list()
	{
		return aw_new VulkanCommandList;
	}

	DeviceBuffer* VulkanDevice::create_buffer(DeviceBufferCreateInfo&& create_info)
	{
		return aw_new VulkanBuffer(std::move(create_info));
	}

	DeviceImage* VulkanDevice::create_image(DeviceImageCreateInfo&& create_info)
	{
		return aw_new VulkanImage(std::move(create_info));
	}

	DeviceImageView* VulkanDevice::create_image_view(DeviceImage* image, DeviceImageViewCreateInfo&& create_info)
	{
		return aw_new VulkanImageView(image, std::move(create_info));
	}

	ISwapChain* VulkanDevice::create_swap_chain(const IRenderWindowInterface& window)
	{
		try
		{
			return aw_new VulkanSwapChain(static_cast<const VulkanWindow&>(window));
		}
		catch (const std::exception& e)
		{
			fmt::println("(awRender) Failed to create swap chain: {}", e.what());
			return nullptr;
		}
	}

	IFrameContext* VulkanDevice::create_frame_context()
	{
		return aw_new VulkanFrame;
	}

	void VulkanDevice::wait_idle()
	{
		m_Device.waitIdle();
	}

	void VulkanDevice::init_vulkan_instance()
	{
		constexpr auto app_info = vk::ApplicationInfo()
									  .setPApplicationName("awRender")
									  .setApplicationVersion(VK_MAKE_API_VERSION(0, 1, 0, 0))
									  .setPEngineName("awRender")
									  .setEngineVersion(VK_MAKE_API_VERSION(0, 1, 0, 0))
									  .setApiVersion(m_ApiVersion);

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
					.setMessageSeverity(eError | eWarning);

				if (g_enable_verbose_render_api_logging)
				{
					debug_messenger_create_info->messageSeverity |= eInfo;
				}

				const vk::DebugUtilsMessengerCreateInfoEXT& debug_messenger_create_info_ptr = debug_messenger_create_info.value();
				create_info.setPNext(&debug_messenger_create_info_ptr);
			}

			if (utils::is_valid_instance_layer("VK_LAYER_KHRONOS_validation"))
			{
				validation_layers.push_back("VK_LAYER_KHRONOS_validation");
			}

			create_info.setPEnabledLayerNames(validation_layers);
		}

		// Set extensions here because we need to ensure that the debug extension is added.
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
			const auto& di_features = features.get<vk::PhysicalDeviceDescriptorIndexingFeatures>();

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

			bool all_extensions_supported = true;
			const auto supported_extensions = physical_device.enumerateDeviceExtensionProperties();
			for (const char* extension : s_required_device_extensions)
			{
				const auto supports_extension = [&supported_extensions, extension] {
					return std::ranges::find_if(supported_extensions, [extension](const auto& ext) { return strcmp(ext.extensionName, extension) == 0; }) != supported_extensions.end();
				};

				if (!supports_extension())
				{
					all_extensions_supported = false;
				}
			}

			if (!all_extensions_supported)
			{
				continue;
			}

			for (const auto queue_family_properties = physical_device.getQueueFamilyProperties(); const auto& queue_family_property : queue_family_properties)
			{
				if (queue_family_property.queueFlags & vk::QueueFlagBits::eGraphics)
				{
					if (const auto surface_formats = physical_device.getSurfaceFormatsKHR(window->get_surface()); !surface_formats.empty())
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

		// Collect queue family indices
		for (const auto queue_families = m_PhysicalDevice.getQueueFamilyProperties();
			const auto& [index, family] : std::ranges::views::enumerate(queue_families))
		{
			if (family.queueFlags & vk::QueueFlagBits::eGraphics)
			{
				m_GraphicsQueueIndex = index;
			}

			// TODO: No compute queue for now
			if (has_valid_queue_indices())
			{
				break;
			}
		}
	}

	void VulkanDevice::create_device()
	{
		core::Vector<vk::DeviceQueueCreateInfo> queue_create_infos{};
		for (const std::array queue_indices = { m_GraphicsQueueIndex };
			const auto queue_index : queue_indices)
		{
			static float priority = 1.0f;
			const auto queue_create_info = vk::DeviceQueueCreateInfo()
											   .setPQueuePriorities(&priority)
											   .setQueueFamilyIndex(queue_index)
											   .setQueueCount(1);
			queue_create_infos.push_back(queue_create_info);
		}

		const auto features = m_PhysicalDevice.getFeatures2<vk::PhysicalDeviceFeatures2, vk::PhysicalDeviceDescriptorIndexingFeatures>();
		const auto create_info = vk::DeviceCreateInfo()
									 .setPNext(&features)
									 .setQueueCreateInfoCount(1)
									 .setQueueCreateInfos(queue_create_infos)
									 .setPEnabledExtensionNames(s_required_device_extensions);
		m_Device = m_PhysicalDevice.createDevice(create_info);

		for (const char* extension : s_required_device_extensions)
		{
			fmt::println("(awRender) -- Device extension {} enabled.", extension);
		}
	}

	void VulkanDevice::init_allocator()
	{
		const VmaAllocatorCreateInfo allocator_info{
			.physicalDevice = *m_PhysicalDevice,
			.device = *m_Device,
			.instance = *m_Instance,
			.vulkanApiVersion = m_ApiVersion,
		};
		if (const VkResult result = vmaCreateAllocator(&allocator_info, &m_Allocator); result != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create vma allocator!");
		}

		fmt::println("(awRender) vma allocator created successfully.");
	}
} // namespace aw::render