#pragma once

#include "aw/core/all.h"

namespace aw::render
{
	class IFrameContext;

	class ISwapChain : public core::IntrusiveRefCounted
	{
	public:
		virtual core::u32 acquire_next_image(IFrameContext* frame_context) = 0;
	};
}