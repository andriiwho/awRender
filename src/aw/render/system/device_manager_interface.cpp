#include "aw/render/system/device_manager_interface.h"
#include "aw/core/all.h"

#ifdef AW_VULKAN_SUPPORTED
	#include "vulkan_device_manager.h"
#endif

namespace aw::render
{
	bool g_enable_gpu_validation = false;

	static IDeviceManagerInterface* s_Instance = nullptr;

	IDeviceManagerInterface* IDeviceManagerInterface::get_or_create(DeviceManagerAPI api)
	{
		if (s_Instance)
		{
			return s_Instance;
		}

		switch (api)
		{
#ifdef AW_VULKAN_SUPPORTED
			case DeviceManagerAPI::Vulkan:
				s_Instance = aw_new VulkanDeviceManager;
				break;
#endif
			default:
				return nullptr;
		}

		return s_Instance;
	}

	void IDeviceManagerInterface::shutdown()
	{
		if (s_Instance)
		{
			aw_delete(s_Instance);
			s_Instance = nullptr;
		}
	}
} // namespace aw::render