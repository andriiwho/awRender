#pragma once

#include <aw/core/all.h>

namespace aw::render
{
	class IDeviceFence;
	class IDeviceQueue;

	enum class DeviceQueueType : core::u8
	{
		graphics,
	};

	class IRenderDeviceInterface : public core::IntrusiveRefCounted
	{
	public:
		virtual IDeviceQueue* create_device_queue(DeviceQueueType queue_type) = 0;
		virtual IDeviceFence* create_fence() = 0;
	};
}