#pragma once

#include "render_state.h"

namespace aw::render
{
	class ISwapChain;

	class IDeviceCommandList : public RenderState
	{
	public:
		virtual void open() = 0;
		virtual void close() = 0;

		virtual void lock_swap_chain(ISwapChain* swap_chain) = 0;
	};
}