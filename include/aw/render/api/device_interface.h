#pragma once

#include <aw/core/all.h>

namespace aw::render
{
	class IDeviceCommandList;
	class IDeviceFence;
	class IDeviceQueue;
	class ISwapChain;
	class IRenderWindowInterface;

	enum class DeviceQueueType : core::u8
	{
		graphics,
	};

	class IRenderDeviceInterface : public core::IntrusiveRefCounted
	{
	public:
		virtual IDeviceQueue* create_device_queue(DeviceQueueType queue_type) = 0;
		virtual IDeviceFence* create_fence() = 0;
		virtual ISwapChain* create_swap_chain(const IRenderWindowInterface& window) = 0;
		virtual IDeviceCommandList* create_command_list() = 0;
	};
}