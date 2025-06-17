#pragma once

#include "aw/core/all.h"
#include "aw/render/api/pixel_format.h"

namespace aw::render
{
	class IFrameContext;

	extern bool g_swap_chain_vsync_enabled;

	class ISwapChain : public core::IntrusiveRefCounted
	{
	public:
		virtual core::u32 acquire_next_image(IFrameContext* frame_context) = 0;
		virtual PixelFormat get_pixel_format() const = 0;
	};
}