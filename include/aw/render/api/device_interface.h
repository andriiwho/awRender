#pragma once

#include <aw/core/all.h>
#include <array>

namespace aw::render
{
	class ISwapChainFrameBuffer;
	struct DeviceImageCreateInfo;
	class DeviceImage;
	class DeviceImageView;
	class DeviceBuffer;
	class DeviceCommandList;
	class IDeviceFence;
	class IDeviceQueue;
	class ISwapChain;
	class IRenderPass;
	class IRenderWindowInterface;
	class IFrameContext;
	class IFrameBuffer;
	struct DeviceBufferCreateInfo;
	struct DeviceImageViewCreateInfo;
	struct RenderPassCreateInfo;
	struct FrameBufferCreateInfo;


	enum class DeviceQueueType : core::u8
	{
		graphics,
	};

	class IRenderDeviceInterface : public core::IntrusiveRefCounted
	{
	public:
		virtual IDeviceQueue* create_device_queue(DeviceQueueType queue_type) = 0;
		virtual IDeviceFence* create_fence() = 0;
		virtual ISwapChain* create_swap_chain(const IRenderWindowInterface& window) = 0;
		virtual DeviceCommandList* create_command_list() = 0;
		virtual DeviceBuffer* create_buffer(DeviceBufferCreateInfo&& create_info) = 0;
		virtual DeviceImage* create_image(DeviceImageCreateInfo&& create_info) = 0;
		virtual DeviceImageView* create_image_view(DeviceImage* image, DeviceImageViewCreateInfo&& create_info) = 0;
		virtual IRenderPass* create_render_pass(const std::string& debug_name) = 0;
		virtual IFrameBuffer* create_frame_buffer(const FrameBufferCreateInfo& create_info) = 0;
		virtual ISwapChainFrameBuffer* create_swap_chain_frame_buffer(ISwapChain* swap_chain, IRenderPass* render_pass) = 0;

		virtual IFrameContext* create_frame_context() = 0;
		virtual void wait_idle() = 0;

		template <core::usize N>
		std::array<IFrameContext*, N> create_frame_contexts()
		{
			std::array<IFrameContext*, N> contexts{};
			for (core::usize i = 0; i < N; ++i)
			{
				contexts[i] = create_frame_context();
			}
			return contexts;
		}
	};
} // namespace aw::render