#include <aw/core/all.h>
#include <aw/render/all.h>

using namespace aw;
using namespace aw::core;
using namespace aw::render;

i32 main()
{
	aw_init_global_thread_pool_scoped();
	g_enable_gpu_validation = true;
	g_enable_verbose_render_api_logging = false;

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

	const RefPtr fence = device->create_fence();
	const RefPtr swap_chain = device->create_swap_chain(*window);

	while (true)
	{
		device_manager->poll_os_events();
		if (window->should_close())
		{
			break;
		}

		queue->set_signal_fence_on_submit(fence);
	}

	return 0;
}
