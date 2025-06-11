#include "aw/render/api/device_resources.h"

namespace aw::render
{
	DeviceResource::DeviceResource(const DeviceResourceType type)
		: m_Type(type)
	{
	}

	ViewableDeviceResource::ViewableDeviceResource(const DeviceResourceType type)
		: DeviceResource(type)
	{
	}

	DeviceBuffer::DeviceBuffer(DeviceBufferCreateInfo&& info)
		: DeviceResource(DeviceResourceType::buffer)
		, m_CreateInfo(std::move(info))
	{
	}

	DeviceImage::DeviceImage(DeviceImageCreateInfo&& info)
		: ViewableDeviceResource(DeviceResourceType::image)
		, m_CreateInfo(std::move(info))
	{
	}
} // namespace aw::render