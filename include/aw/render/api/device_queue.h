#pragma once

#include "aw/core/all.h"

namespace aw::render
{
	class DeviceCommandList;
	class IDeviceFence;
	class ISwapChain;
	class IFrameContext;

	class IDeviceQueue : public core::IntrusiveRefCounted
	{
	public:
		virtual void present_swap_chain(IFrameContext* ctx, ISwapChain* swap_chain) = 0;

		virtual void submit(IFrameContext* ctx) = 0;
	};
}