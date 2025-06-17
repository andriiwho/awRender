#pragma once

#include "aw/core/all.h"
#include "aw/render/api/pixel_format.h"
#include "aw/render/api/device_resources.h"

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

	struct PassImageDesc
	{
		PixelFormat format{};
		DeviceResourceState state_before_pass{};
		DeviceResourceState state_in_pass{};
		DeviceResourceState state_after_pass{};
		ImageLoadOp load_op{};
		ImageStoreOp store_op{};
		ImageLoadOp stencil_load{ImageLoadOp::none};
		ImageStoreOp stencil_store{ImageStoreOp::none};
	};

	class IRenderPass : public core::IntrusiveRefCounted
	{
	public:
		virtual void build() = 0;

		virtual void add_pass_image(const PassImageDesc& image_desc) = 0;
	};
}