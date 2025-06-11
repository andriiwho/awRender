#pragma once

#include "render_window_interface.h"

#include "aw/core/primitive/numbers.h"
#include "aw/render/api/device_interface.h"
#include "aw/render/misc/aw_render_checks.h"

namespace aw::render
{
	class IShaderCompiler;
	/** set to true before creating the first device to enable GPU validation */
	extern bool g_enable_gpu_validation;
	extern bool g_enable_verbose_render_api_logging;

	enum class DeviceManagerAPI
	{
		#ifdef AW_VULKAN_SUPPORTED
		Vulkan,
		#endif
		Default = 0,
	};

	class IDeviceManagerInterface
	{
	public:
		virtual ~IDeviceManagerInterface() = default;

		[[nodiscard]] static IDeviceManagerInterface* get_or_create(DeviceManagerAPI api = DeviceManagerAPI::Default);
		static void shutdown();

		/**
		 * Initialize the rendering device interface and create a window to use it in.
		 * The created window is considered as the main window.
		 * This function should only be called once.
		 * @param create_info create info with window specification.
		 * @param out_device_opt Pointer to where to store the created device handle. Optional.
		 * @return
		 */
		[[nodiscard]] virtual IRenderWindowInterface* create_device_and_window(const RenderWindowCreateInfo& create_info, IRenderDeviceInterface** out_device_opt) = 0;

		[[nodiscard]] virtual IShaderCompiler* create_shader_compiler() = 0;

		[[nodiscard]] virtual IRenderDeviceInterface* get_device() const = 0;

		virtual void poll_os_events() const = 0;
	};
}