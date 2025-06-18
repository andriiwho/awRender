#pragma once

#include <aw/core/all.h>

#include <optional>
#include <span>

namespace aw::render
{
	class IRenderPass;
	class DeviceImageView;

	struct FrameBufferCreateInfo
	{
		std::span<DeviceImageView*> color_attachments{};
		std::optional<DeviceImageView*> depth_stencil_attachment{};
		IRenderPass* render_pass{};
	};

	enum class FrameBufferType
	{
		normal,
		swap_chain,
	};

	class IFrameBuffer : public core::IntrusiveRefCounted
	{
	public:
		virtual FrameBufferType get_frame_buffer_type() const noexcept { return FrameBufferType::normal; }

		virtual core::u32 get_width() const = 0;
		virtual core::u32 get_height() const = 0;
	};

	class ISwapChainFrameBuffer : public IFrameBuffer
	{
	public:
		FrameBufferType get_frame_buffer_type() const noexcept final { return FrameBufferType::swap_chain; }

		virtual DeviceImageView* get_current_image_view() const = 0;
	};
}