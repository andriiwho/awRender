#pragma once

#include "render_state.h"

namespace aw::render
{
	class ISwapChain;
	class IRenderPass;
	class IFrameBuffer;

	class DeviceCommandList : public RenderState
	{
	public:
		virtual void open();
		virtual void close();

		virtual void copy_buffer(DeviceBuffer* from, DeviceBuffer* to, core::u64 size) = 0;

		virtual void begin_render_pass(IRenderPass* render_pass, IFrameBuffer* frame_buffer) = 0;
		virtual void end_render_pass() = 0;
	};
}