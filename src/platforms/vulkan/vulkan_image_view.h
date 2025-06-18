#pragma once

#include "aw/render/api/device_views.h"

#include "vulkan_common.h"

namespace aw::render
{
	class VulkanImageView final : public DeviceImageView
	{
	public:
		explicit VulkanImageView(DeviceImage* image, DeviceImageViewCreateInfo&& info);

		vk::ImageView get_handle() const { return m_ImageView; }

	private:
		vk::raii::ImageView m_ImageView{nullptr};
	};
}