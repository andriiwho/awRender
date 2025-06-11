#include "aw/render/api/pixel_format.h"

#include "dxgi1_6.h"

namespace aw::render
{
	inline constexpr DXGI_FORMAT to_dxgi_format(PixelFormat format)
	{
		switch (format)
		{
			case PixelFormat::none:
				break;
			case PixelFormat::r32g32b32a32_sfloat:
				return DXGI_FORMAT_R32G32B32A32_FLOAT;
				break;
			case PixelFormat::r32g32b32_sfloat:
				return DXGI_FORMAT_R32G32B32_FLOAT;
				break;
			case PixelFormat::r32g32_sfloat:
				return DXGI_FORMAT_R32G32_FLOAT;
				break;
			case PixelFormat::r32_sfloat:
				return DXGI_FORMAT_R32_FLOAT;
				break;
			case PixelFormat::r16g16b16a16_sfloat:
				return DXGI_FORMAT_R16G16B16A16_FLOAT;
				break;
			case PixelFormat::r16g16_sfloat:
				return DXGI_FORMAT_R16G16_FLOAT;
				break;
			case PixelFormat::r16_sfloat:
				return DXGI_FORMAT_R16_FLOAT;
				break;
			case PixelFormat::r32g32b32a32_uint:
				return DXGI_FORMAT_R32G32B32A32_UINT;
				break;
			case PixelFormat::r32g32b32_uint:
				return DXGI_FORMAT_R32G32B32_UINT;
				break;
			case PixelFormat::r32g32_uint:
				return DXGI_FORMAT_R32G32_UINT;
				break;
			case PixelFormat::r32_uint:
				return DXGI_FORMAT_R32_UINT;
				break;
			case PixelFormat::r16g16b16a16_uint:
				return DXGI_FORMAT_R16G16B16A16_UINT;
				break;
			case PixelFormat::r16g16_uint:
				return DXGI_FORMAT_R16G16_UINT;
				break;
			case PixelFormat::r16_uint:
				return DXGI_FORMAT_R16_UINT;
				break;
			case PixelFormat::r8g8b8a8_unorm:
				return DXGI_FORMAT_R8G8B8A8_UNORM;
				break;
			case PixelFormat::b8g8r8a8_unorm:
				return DXGI_FORMAT_B8G8R8A8_UNORM;
				break;
			case PixelFormat::r8g8_unorm:
				return DXGI_FORMAT_R8G8_UNORM;
				break;
			case PixelFormat::r8_unorm:
				return DXGI_FORMAT_R8_UNORM;
				break;
			case PixelFormat::r8g8b8a8_uint:
				return DXGI_FORMAT_R8G8B8A8_UINT;
				break;
			case PixelFormat::bc1_rgba_unorm_block:
				return DXGI_FORMAT_BC1_UNORM;
				break;
			case PixelFormat::bc2_unorm_block:
				return DXGI_FORMAT_BC2_UNORM;
				break;
			case PixelFormat::bc3_unorm_block:
				return DXGI_FORMAT_BC3_UNORM;
				break;
			case PixelFormat::bc4_unorm_block:
				return DXGI_FORMAT_BC4_UNORM;
				break;
			case PixelFormat::bc5_unorm_block:
				return DXGI_FORMAT_BC5_UNORM;
				break;
			case PixelFormat::bc6h_ufloat_block:
				return DXGI_FORMAT_BC6H_UF16;
				break;
			case PixelFormat::bc7_unorm_block:
				return DXGI_FORMAT_BC7_UNORM;
				break;
			case PixelFormat::d32_sfloat:
				return DXGI_FORMAT_D32_FLOAT;
				break;
			case PixelFormat::d24_unorm_s8_uint:
				return DXGI_FORMAT_D24_UNORM_S8_UINT;
				break;
			case PixelFormat::d32_sfloat_s8_uint:
				return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
				break;
			case PixelFormat::a2b10g10r10_unorm_pack32:
				return DXGI_FORMAT_R10G10B10A2_UNORM;
				break;
			case PixelFormat::b10g11r11_ufloat_pack32:
				return DXGI_FORMAT_R11G11B10_FLOAT;
				break;
		}

		return DXGI_FORMAT_UNKNOWN;
	}

	inline constexpr PixelFormat to_pixel_format(const DXGI_FORMAT format)
	{
		switch (format)
		{
			case DXGI_FORMAT_R32G32B32A32_FLOAT:
				return PixelFormat::r32g32b32a32_sfloat;
			case DXGI_FORMAT_R32G32B32_FLOAT:
				return PixelFormat::r32g32b32_sfloat;
			case DXGI_FORMAT_R32G32_FLOAT:
				return PixelFormat::r32g32_sfloat;
			case DXGI_FORMAT_R32_FLOAT:
				return PixelFormat::r32_sfloat;
			case DXGI_FORMAT_R16G16B16A16_FLOAT:
				return PixelFormat::r16g16b16a16_sfloat;
			case DXGI_FORMAT_R16G16_FLOAT:
				return PixelFormat::r16g16_sfloat;
			case DXGI_FORMAT_R16_FLOAT:
				return PixelFormat::r16_sfloat;
			case DXGI_FORMAT_R32G32B32A32_UINT:
				return PixelFormat::r32g32b32a32_uint;
			case DXGI_FORMAT_R32G32B32_UINT:
				return PixelFormat::r32g32b32_uint;
			case DXGI_FORMAT_R32G32_UINT:
				return PixelFormat::r32g32_uint;
			case DXGI_FORMAT_R32_UINT:
				return PixelFormat::r32_uint;
			case DXGI_FORMAT_R16G16B16A16_UINT:
				return PixelFormat::r16g16b16a16_uint;
			case DXGI_FORMAT_R16G16_UINT:
				return PixelFormat::r16g16_uint;
			case DXGI_FORMAT_R16_UINT:
				return PixelFormat::r16_uint;
			case DXGI_FORMAT_R8G8B8A8_UNORM:
				return PixelFormat::r8g8b8a8_unorm;
			case DXGI_FORMAT_B8G8R8A8_UNORM:
				return PixelFormat::b8g8r8a8_unorm;
			case DXGI_FORMAT_R8G8_UNORM:
				return PixelFormat::r8g8_unorm;
			case DXGI_FORMAT_R8_UNORM:
				return PixelFormat::r8_unorm;
			case DXGI_FORMAT_R8G8B8A8_UINT:
				return PixelFormat::r8g8b8a8_uint;
			case DXGI_FORMAT_BC1_UNORM:
				return PixelFormat::bc1_rgba_unorm_block;
			case DXGI_FORMAT_BC2_UNORM:
				return PixelFormat::bc2_unorm_block;
			case DXGI_FORMAT_BC3_UNORM:
				return PixelFormat::bc3_unorm_block;
			case DXGI_FORMAT_BC4_UNORM:
				return PixelFormat::bc4_unorm_block;
			case DXGI_FORMAT_BC5_UNORM:
				return PixelFormat::bc5_unorm_block;
			case DXGI_FORMAT_BC6H_UF16:
				return PixelFormat::bc6h_ufloat_block;
			case DXGI_FORMAT_BC7_UNORM:
				return PixelFormat::bc7_unorm_block;
			case DXGI_FORMAT_D32_FLOAT:
				return PixelFormat::d32_sfloat;
			case DXGI_FORMAT_D24_UNORM_S8_UINT:
				return PixelFormat::d24_unorm_s8_uint;
			case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
				return PixelFormat::d32_sfloat_s8_uint;
			case DXGI_FORMAT_R10G10B10A2_UNORM:
				return PixelFormat::a2b10g10r10_unorm_pack32;
			case DXGI_FORMAT_R11G11B10_FLOAT:
				return PixelFormat::b10g11r11_ufloat_pack32;
			default:
				break;
		}
		return PixelFormat::none;
	}
} // namespace aw::render