#pragma once

#include "aw/core/all.h"

namespace aw::render
{
	class IDeviceCommandList;

	class IFrameContext : public core::IntrusiveRefCounted
	{
	public:
		virtual void wait_and_reset() = 0;
		virtual IDeviceCommandList* cmd() const = 0;
	};
}