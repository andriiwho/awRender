#pragma once

#include "aw/core/all.h"

namespace aw::render
{
	class IDeviceFence;

	class IDeviceQueue : public core::IntrusiveRefCounted
	{
	public:
		virtual void set_signal_fence_on_submit(IDeviceFence* fence) = 0;
	};
}