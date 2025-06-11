#pragma once

#include "aw/core/all.h"

namespace aw::render
{
	class DeviceCommandList;

	class IFrameContext : public core::IntrusiveRefCounted
	{
	public:
		virtual void wait_and_reset() = 0;
		virtual DeviceCommandList* cmd() const = 0;
	};
}