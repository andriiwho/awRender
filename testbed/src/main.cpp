#include "vulkan_swap_chain.h"
#include "aw/render/api/frame_context.h"

#include <aw/core/all.h>
#include <aw/render/all.h>

using namespace aw;
using namespace aw::core;
using namespace aw::render;

struct Vertex
{
	Vector3 position;
	Vector3 color;
};

i32 main()
{
	aw_init_global_thread_pool_scoped();
	g_enable_gpu_validation = true;
	g_enable_verbose_render_api_logging = false;
	g_swap_chain_vsync_enabled = true;

	IDeviceManagerInterface* device_manager = aw::render::IDeviceManagerInterface::get_or_create();
	defer[]
	{
		IDeviceManagerInterface::shutdown();
	};

	RefPtr<IRenderDeviceInterface> device = nullptr;
	const IRenderWindowInterface* window = device_manager->create_device_and_window(RenderWindowCreateInfo{ 1280, 720, "awRender" }, &device);

	const RefPtr queue = device->create_device_queue(render::DeviceQueueType::graphics);
	if (!queue)
	{
		return -1;
	}

	const RefPtr swap_chain = device->create_swap_chain(*window);

	constexpr u32 num_frames_in_flight = 3;
	const auto frame_contexts = device->create_frame_contexts<num_frames_in_flight>();
	defer[&frame_contexts]
	{
		for (auto& frame_context : frame_contexts)
		{
			frame_context->release();
		}
	};

	Vertex vertices[] = {
		{ { 0.0f, 0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f } },
		{ { -0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f } },
		{ { 0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f } },
	};

	DeviceBufferCreateInfo vertex_buffer_info{
		.debug_name = "test_vb",
		.size_in_bytes = sizeof(vertices),
		.usage = DeviceBufferUsage::vertex | DeviceBufferUsage::sequential_write,
	};
	const RefPtr vertex_buffer = device->create_buffer(std::move(vertex_buffer_info));

	u32 current_frame = 0;
	while (true)
	{
		device_manager->poll_os_events();
		if (window->should_close())
		{
			break;
		}

		IFrameContext* context = frame_contexts[current_frame];
		context->wait_and_reset();

		swap_chain->acquire_next_image(context);
		IDeviceCommandList* cmd_list = context->cmd();
		cmd_list->open();

		// Draw commands here...

		cmd_list->close();
		queue->submit(context);

		queue->present_swap_chain(context, swap_chain);
		current_frame = (current_frame + 1) % num_frames_in_flight;
	}

	device->wait_idle();

	return 0;
}
