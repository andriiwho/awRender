#pragma once

#include "aw/core/all.h"

namespace aw::render
{
	class IDeviceFence : public core::IntrusiveRefCounted
	{
	public:
		virtual void wait() = 0;
	};
}