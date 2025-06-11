#pragma once

#include <aw/core/all.h>

namespace aw::render
{
	enum class PixelFormat
	{
		none,

		r32g32b32a32_sfloat,
		r32g32b32_sfloat,
		r32g32_sfloat,
		r32_sfloat,
		r16g16b16a16_sfloat,
		r16g16_sfloat,
		r16_sfloat,
		r32g32b32a32_uint,
		r32g32b32_uint,
		r32g32_uint,
		r32_uint,
		r16g16b16a16_uint,
		r16g16_uint,
		r16_uint,
		r8g8b8a8_unorm,
		b8g8r8a8_unorm,
		r8g8_unorm,
		r8_unorm,
		r8g8b8a8_uint,
		bc1_rgba_unorm_block,
		bc2_unorm_block,
		bc3_unorm_block,
		bc4_unorm_block,
		bc5_unorm_block,
		bc6h_ufloat_block,
		bc7_unorm_block,
		d32_sfloat,
		d24_unorm_s8_uint,
		d32_sfloat_s8_uint,
		a2b10g10r10_unorm_pack32,
		b10g11r11_ufloat_pack32,
	};

	inline constexpr bool format_has_depth_component(const PixelFormat format)
	{
		return format == PixelFormat::d32_sfloat || format == PixelFormat::d24_unorm_s8_uint ||
			   format == PixelFormat::d32_sfloat_s8_uint;
	}

	inline constexpr bool format_has_stencil_component(const PixelFormat format)
	{
		return format == PixelFormat::d24_unorm_s8_uint || format == PixelFormat::d32_sfloat_s8_uint;
	}

	inline constexpr core::usize format_size(const PixelFormat format)
	{
		switch (format)
		{

			case PixelFormat::none:
				break;
			case PixelFormat::r32g32b32a32_sfloat:
				return 16;
				break;
			case PixelFormat::r32g32b32_sfloat:
				return 12;
				break;
			case PixelFormat::r32g32_sfloat:
				return 8;
				break;
			case PixelFormat::r32_sfloat:
				return 4;
				break;
			case PixelFormat::r16g16b16a16_sfloat:
				return 8;
				break;
			case PixelFormat::r16g16_sfloat:
				return 4;
				break;
			case PixelFormat::r16_sfloat:
				return 2;
				break;
			case PixelFormat::r32g32b32a32_uint:
				return 16;
				break;
			case PixelFormat::r32g32b32_uint:
				return 12;
				break;
			case PixelFormat::r32g32_uint:
				return 8;
				break;
			case PixelFormat::r32_uint:
				return 4;
				break;
			case PixelFormat::r16g16b16a16_uint:
				return 8;
				break;
			case PixelFormat::r16g16_uint:
				return 4;
				break;
			case PixelFormat::r16_uint:
				return 2;
				break;
			case PixelFormat::r8g8b8a8_unorm:
				return 4;
				break;
			case PixelFormat::b8g8r8a8_unorm:
				return 4;
				break;
			case PixelFormat::r8g8_unorm:
				return 2;
				break;
			case PixelFormat::r8_unorm:
				return 1;
				break;
			case PixelFormat::r8g8b8a8_uint:
				return 4;
				break;
			case PixelFormat::bc1_rgba_unorm_block:
				return 8;
				break;
			case PixelFormat::bc2_unorm_block:
				return 16;
				break;
			case PixelFormat::bc3_unorm_block:
				return 16;
				break;
			case PixelFormat::bc4_unorm_block:
				return 8;
				break;
			case PixelFormat::bc5_unorm_block:
				return 16;
				break;
			case PixelFormat::bc6h_ufloat_block:
				return 16;
				break;
			case PixelFormat::bc7_unorm_block:
				return 16;
				break;
			case PixelFormat::d32_sfloat:
				return 4;
				break;
			case PixelFormat::d24_unorm_s8_uint:
				return 4;
				break;
			case PixelFormat::d32_sfloat_s8_uint:
				return 8;
				break;
			case PixelFormat::a2b10g10r10_unorm_pack32:
				return 4;
				break;
			case PixelFormat::b10g11r11_ufloat_pack32:
				return 4;
				break;
		}

		return 0;
	}
} // namespace aw::render