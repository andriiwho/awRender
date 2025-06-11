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
	};
}