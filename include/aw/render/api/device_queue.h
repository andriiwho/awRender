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
		virtual void present_swap_chain(ISwapChain* swap_chain) = 0;

		virtual void submit(IDeviceCommandList* command_list, IDeviceFence* fence_to_signal) = 0;
	};
}