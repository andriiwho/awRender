#pragma once

#include <aw/core/all.h>

namespace aw::render
{
	struct RenderWindowCreateInfo
	{
		core::i32 width{};
		core::i32 height{};
		std::string_view title{};
	};

	class IRenderWindowInterface
	{
	public:
		virtual ~IRenderWindowInterface() = default;

		virtual bool should_close() const = 0;
	};
}