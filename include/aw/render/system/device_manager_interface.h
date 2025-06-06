#pragma once

#include "render_window_interface.h"

#include "aw/core/primitive/numbers.h"
#include "aw/render/api/device_interface.h"
#include "aw/render/misc/aw_render_checks.h"

namespace aw::render
{
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

		static IDeviceManagerInterface* get_or_create(DeviceManagerAPI api = DeviceManagerAPI::Default);
		static void shutdown();

		virtual IRenderWindowInterface* create_device_and_window(const RenderWindowCreateInfo& create_info, IRenderDeviceInterface** out_device_opt) = 0;
	};
}