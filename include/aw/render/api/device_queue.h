#pragma once

#include "aw/core/all.h"

namespace aw::render
{
	class IDeviceCommandList;
	class IDeviceFence;
	class ISwapChain;

	class IDeviceQueue : public core::IntrusiveRefCounted
	{
	public:
		virtual void set_signal_fence_on_submit(IDeviceFence* fence) = 0;
		virtual void present_swap_chain(ISwapChain* swap_chain) = 0;

		virtual void submit(IDeviceCommandList* command_list) = 0;
	};
}