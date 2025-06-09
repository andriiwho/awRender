#include <aw/core/all.h>
#include <aw/render/all.h>

using namespace aw;

core::i32 main()
{
	aw_init_global_thread_pool_scoped();
	render::g_enable_gpu_validation = true;

	render::IDeviceManagerInterface* device_manager = aw::render::IDeviceManagerInterface::get_or_create();
	defer[]
	{
		render::IDeviceManagerInterface::shutdown();
	};

	render::IRenderDeviceInterface* device = nullptr;
	const render::IRenderWindowInterface* window = device_manager->create_device_and_window(render::RenderWindowCreateInfo{ 1280, 720, "awRender" }, &device);
	defer[device]
	{
		device->release();
	};

	while (true)
	{
		device_manager->poll_os_events();
		if (window->should_close())
		{
			break;
		}
	}

	return 0;
}
