#pragma once

#include "aw/render/api/pixel_format.h"
#include "vulkan_common.h"

namespace aw::render
{
	inline constexpr vk::Format to_vulkan_format(const PixelFormat format)
	{
		switch (format)
		{
			case PixelFormat::none:
				break;
			case PixelFormat::r32g32b32a32_sfloat:
				return vk::Format::eR32G32B32A32Sfloat;
				break;
			case PixelFormat::r32g32b32_sfloat:
				return vk::Format::eR32G32B32Sfloat;
				break;
			case PixelFormat::r32g32_sfloat:
				return vk::Format::eR32G32Sfloat;
				break;
			case PixelFormat::r32_sfloat:
				return vk::Format::eR32Sfloat;
				break;
			case PixelFormat::r16g16b16a16_sfloat:
				return vk::Format::eR16G16B16A16Sfloat;
				break;
			case PixelFormat::r16g16_sfloat:
				return vk::Format::eR16G16Sfloat;
				break;
			case PixelFormat::r16_sfloat:
				return vk::Format::eR16Sfloat;
				break;
			case PixelFormat::r32g32b32a32_uint:
				return vk::Format::eR32G32B32A32Uint;
				break;
			case PixelFormat::r32g32b32_uint:
				return vk::Format::eR32G32B32Uint;
				break;
			case PixelFormat::r32g32_uint:
				return vk::Format::eR32G32Uint;
				break;
			case PixelFormat::r32_uint:
				return vk::Format::eR32Uint;
				break;
			case PixelFormat::r16g16b16a16_uint:
				return vk::Format::eR16G16B16A16Uint;
				break;
			case PixelFormat::r16g16_uint:
				return vk::Format::eR16G16Uint;
				break;
			case PixelFormat::r16_uint:
				return vk::Format::eR16Uint;
				break;
			case PixelFormat::r8g8b8a8_unorm:
				return vk::Format::eR8G8B8A8Unorm;
				break;
			case PixelFormat::b8g8r8a8_unorm:
				return vk::Format::eB8G8R8A8Unorm;
				break;
			case PixelFormat::r8g8_unorm:
				return vk::Format::eR8G8Unorm;
				break;
			case PixelFormat::r8_unorm:
				return vk::Format::eR8Unorm;
				break;
			case PixelFormat::r8g8b8a8_uint:
				return vk::Format::eR8G8B8A8Uint;
				break;
			case PixelFormat::bc1_rgba_unorm_block:
				return vk::Format::eBc1RgbaUnormBlock;
				break;
			case PixelFormat::bc2_unorm_block:
				return vk::Format::eBc2UnormBlock;
				break;
			case PixelFormat::bc3_unorm_block:
				return vk::Format::eBc3UnormBlock;
				break;
			case PixelFormat::bc4_unorm_block:
				return vk::Format::eBc4UnormBlock;
				break;
			case PixelFormat::bc5_unorm_block:
				return vk::Format::eBc5UnormBlock;
				break;
			case PixelFormat::bc6h_ufloat_block:
				return vk::Format::eBc6HUfloatBlock;
				break;
			case PixelFormat::bc7_unorm_block:
				return vk::Format::eBc7UnormBlock;
				break;
			case PixelFormat::d32_sfloat:
				return vk::Format::eD32Sfloat;
				break;
			case PixelFormat::d24_unorm_s8_uint:
				return vk::Format::eD24UnormS8Uint;
				break;
			case PixelFormat::d32_sfloat_s8_uint:
				return vk::Format::eD32SfloatS8Uint;
				break;
			case PixelFormat::a2b10g10r10_unorm_pack32:
				return vk::Format::eA2B10G10R10UnormPack32;
				break;
			case PixelFormat::b10g11r11_ufloat_pack32:
				return vk::Format::eB10G11R11UfloatPack32;
				break;
		}

		return vk::Format::eUndefined;
	}

	inline constexpr PixelFormat to_pixel_format(const vk::Format format)
	{
		switch (format)
		{
			case vk::Format::eR32G32B32A32Sfloat:
				return PixelFormat::r32g32b32a32_sfloat;
			case vk::Format::eR32G32B32Sfloat:
				return PixelFormat::r32g32b32_sfloat;
			case vk::Format::eR32G32Sfloat:
				return PixelFormat::r32g32_sfloat;
			case vk::Format::eR32Sfloat:
				return PixelFormat::r32_sfloat;
			case vk::Format::eR16G16B16A16Sfloat:
				return PixelFormat::r16g16b16a16_sfloat;
			case vk::Format::eR16G16Sfloat:
				return PixelFormat::r16g16_sfloat;
			case vk::Format::eR16Sfloat:
				return PixelFormat::r16_sfloat;
			case vk::Format::eR32G32B32A32Uint:
				return PixelFormat::r32g32b32a32_uint;
			case vk::Format::eR32G32B32Uint:
				return PixelFormat::r32g32b32_uint;
			case vk::Format::eR32G32Uint:
				return PixelFormat::r32g32_uint;
			case vk::Format::eR32Uint:
				return PixelFormat::r32_uint;
			case vk::Format::eR16G16B16A16Uint:
				return PixelFormat::r16g16b16a16_uint;
			case vk::Format::eR16G16Uint:
				return PixelFormat::r16g16_uint;
			case vk::Format::eR16Uint:
				return PixelFormat::r16_uint;
			case vk::Format::eR8G8B8A8Unorm:
				return PixelFormat::r8g8b8a8_unorm;
			case vk::Format::eB8G8R8A8Unorm:
				return PixelFormat::b8g8r8a8_unorm;
			case vk::Format::eR8G8Unorm:
				return PixelFormat::r8g8_unorm;
			case vk::Format::eR8Unorm:
				return PixelFormat::r8_unorm;
			case vk::Format::eR8G8B8A8Uint:
				return PixelFormat::r8g8b8a8_uint;
			case vk::Format::eBc1RgbaUnormBlock:
				return PixelFormat::bc1_rgba_unorm_block;
			case vk::Format::eBc2UnormBlock:
				return PixelFormat::bc2_unorm_block;
			case vk::Format::eBc3UnormBlock:
				return PixelFormat::bc3_unorm_block;
			case vk::Format::eBc4UnormBlock:
				return PixelFormat::bc4_unorm_block;
			case vk::Format::eBc5UnormBlock:
				return PixelFormat::bc5_unorm_block;
			case vk::Format::eBc6HUfloatBlock:
				return PixelFormat::bc6h_ufloat_block;
			case vk::Format::eBc7UnormBlock:
				return PixelFormat::bc7_unorm_block;
			case vk::Format::eD32Sfloat:
				return PixelFormat::d32_sfloat;
			case vk::Format::eD24UnormS8Uint:
				return PixelFormat::d24_unorm_s8_uint;
			case vk::Format::eD32SfloatS8Uint:
				return PixelFormat::d32_sfloat_s8_uint;
			case vk::Format::eA2B10G10R10UnormPack32:
				return PixelFormat::a2b10g10r10_unorm_pack32;
			case vk::Format::eB10G11R11UfloatPack32:
				return PixelFormat::b10g11r11_ufloat_pack32;
			default:
				return PixelFormat::none;
		}

		return PixelFormat::none;
	}
} // namespace aw::render