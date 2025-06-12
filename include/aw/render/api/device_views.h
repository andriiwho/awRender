#pragma once

#include "device_resources.h"

namespace aw::render
{
	enum class DeviceResourceViewAspect : core::u8
	{
		none,
		color,
		depth,
		stencil,
		depth_stencil,
	};

	enum class DeviceResourceViewDimensions
	{
		none,
		image_1d,
		image_2d,
		image_3d,
		image_1d_array,
		image_2d_array,
		image_cube,
		image_cube_array,
	};

	class DeviceResourceView : public core::IntrusiveRefCounted
	{
	public:
		DeviceResourceView(ViewableDeviceResource* resource, DeviceResourceViewAspect aspect, DeviceResourceViewDimensions dimensions);

		ViewableDeviceResource* get_resource() const { return m_Resource.get(); }
		DeviceResourceViewAspect get_aspect() const { return m_Aspect; }
		DeviceResourceViewDimensions get_dimensions() const { return m_Dimensions; }

		void add_ref() const override;
		void release() const override;

	private:
		core::RefPtr<ViewableDeviceResource> m_Resource;
		DeviceResourceViewAspect m_Aspect;
		DeviceResourceViewDimensions m_Dimensions;
	};

	struct DeviceImageViewCreateInfo
	{
		std::string debug_name{};
		DeviceResourceViewAspect aspect{};
		DeviceResourceViewDimensions dimensions{};
		core::u8 mip_levels{1};
		core::u8 base_mip_level{};
		core::u8 array_layers{1};
		core::u8 base_array_layer{};
	};

	class DeviceImageView : public DeviceResourceView
	{
	public:
		explicit DeviceImageView(DeviceImage* image, DeviceImageViewCreateInfo&& info);

		DeviceImage* get_image() const { return static_cast<DeviceImage*>(get_resource()); }
		DeviceImageViewCreateInfo get_create_info() const { return m_CreateInfo; }

	private:
		DeviceImageViewCreateInfo m_CreateInfo{};
	};
}