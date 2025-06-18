#pragma once

#include "aw/core/all.h"
#include "aw/render/api/pixel_format.h"
#include "aw/render/api/device_resources.h"

#include <variant>

namespace aw::render
{
	enum class ImageLoadOp
	{
		clear,
		load,
		none,
	};

	enum class ImageStoreOp
	{
		store,
		none,
	};

	template <typename T>
	struct ClearValue_Color
	{
		T r{};
		T g{};
		T b{};
		T a{};
	};

	struct ClearValue_DepthStencil
	{
		float depth{};
		core::u32 stencil{};
	};

	using PassClearValue = std::variant<ClearValue_Color<float>, ClearValue_Color<core::i32>, ClearValue_Color<core::u32>, ClearValue_DepthStencil>;

	constexpr PassClearValue clear_depth_stencil(const float depth = 1.0f, const core::u32 stencil = 0)
	{
		return ClearValue_DepthStencil{ depth, stencil };
	}

	constexpr PassClearValue clear_color(const float r = 0.0f, const float g = 0.0f, const float b = 0.0f, const float a = 1.0f)
	{
		return ClearValue_Color<float>{ r, g, b, a };
	}

	constexpr PassClearValue clear_color_i32(const core::i32 r = 0, const core::i32 g = 0, const core::i32 b = 0, const core::i32 a = 1)
	{
		return ClearValue_Color<core::i32>{ r, g, b, a };
	}

	constexpr PassClearValue clear_color_u32(const core::u32 r = 0, const core::u32 g = 0, const core::u32 b = 0, const core::u32 a = 1)
	{
		return ClearValue_Color<core::u32>{ r, g, b, a };
	}

	struct PassImageDesc
	{
		PixelFormat format{};
		DeviceResourceState state_before_pass{};
		DeviceResourceState state_in_pass{};
		DeviceResourceState state_after_pass{};
		ImageLoadOp load_op{};
		ImageStoreOp store_op{};
		ImageLoadOp stencil_load{ ImageLoadOp::none };
		ImageStoreOp stencil_store{ ImageStoreOp::none };
		std::optional<PassClearValue> clear_value = {};
	};

	class IRenderPass : public core::IntrusiveRefCounted
	{
	public:
		virtual void build() = 0;

		virtual void add_pass_image(const PassImageDesc& image_desc) = 0;
	};
} // namespace aw::render