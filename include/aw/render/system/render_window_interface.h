#pragma once

#include <aw/core/all.h>

namespace aw::render
{
	struct RenderWindowCreateInfo
	{
		core::i32 width{};
		core::i32 height{};
		core::StringView title{};
	};

	class IRenderWindowInterface
	{
	public:
		virtual ~IRenderWindowInterface() = default;
	};
}