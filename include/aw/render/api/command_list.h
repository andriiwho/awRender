#pragma once

#include "render_state.h"

namespace aw::render
{
	class ISwapChain;

	class DeviceCommandList : public RenderState
	{
	public:
		virtual void open();
		virtual void close();

		virtual void copy_buffer(DeviceBuffer* from, DeviceBuffer* to, core::u64 size) = 0;
	};
}