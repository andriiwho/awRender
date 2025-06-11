#include "aw/render/api/command_list.h"

namespace aw::render
{

	void DeviceCommandList::open()
	{
		clear_resource_states();
	}

	void DeviceCommandList::close()
	{
		flush_resource_states();
	}
} // namespace aw::render