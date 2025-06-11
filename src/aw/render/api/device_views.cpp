#include "aw/render/api/device_views.h"

namespace aw::render
{
	DeviceResourceView::DeviceResourceView(ViewableDeviceResource* resource, const DeviceResourceViewAspect aspect, const DeviceResourceViewDimensions dimensions)
		: m_Resource(resource)
		, m_Aspect(aspect)
		, m_Dimensions(dimensions)
	{
		m_Resource->add_ref();
	}

	void DeviceResourceView::add_ref() const
	{
		IntrusiveRefCounted::add_ref();

		if (m_Resource)
		{
			m_Resource->add_ref();
		}
	}
	void DeviceResourceView::release() const
	{
		if (m_Resource)
		{
			m_Resource->release();
		}

		IntrusiveRefCounted::release();
	}

	DeviceImageView::DeviceImageView(DeviceImage* image, DeviceImageViewCreateInfo&& info)
		: DeviceResourceView(image, info.aspect, info.dimensions)
		, m_CreateInfo(std::move(info))
	{
	}
} // namespace aw::render