#include "vulkan_image_view.h"

#include "vulkan_device.h"
#include "vulkan_image.h"
#include "vulkan_pixel_format.h"

namespace aw::render
{
	namespace detail
	{
		static vk::ImageViewType to_image_view_type(const DeviceResourceViewDimensions type)
		{
			switch (type)
			{
				case DeviceResourceViewDimensions::none:
					break;
				case DeviceResourceViewDimensions::image_1d:
					return vk::ImageViewType::e1D;
					break;
				case DeviceResourceViewDimensions::image_2d:
					return vk::ImageViewType::e2D;
					break;
				case DeviceResourceViewDimensions::image_3d:
					return vk::ImageViewType::e3D;
					break;
				case DeviceResourceViewDimensions::image_1d_array:
					return vk::ImageViewType::e1DArray;
					break;
				case DeviceResourceViewDimensions::image_2d_array:
					return vk::ImageViewType::e2DArray;
					break;
				case DeviceResourceViewDimensions::image_cube:
					return vk::ImageViewType::eCube;
					break;
				case DeviceResourceViewDimensions::image_cube_array:
					return vk::ImageViewType::eCubeArray;
					break;
			}

			return {};
		}

		static vk::ImageSubresourceRange to_image_subresource_range(const DeviceImageViewCreateInfo& create_info)
		{
			vk::ImageSubresourceRange out{};
			out.setBaseArrayLayer(create_info.base_array_layer)
				.setLayerCount(create_info.array_layers)
				.setBaseMipLevel(create_info.base_mip_level)
				.setLevelCount(create_info.mip_levels);

			switch (create_info.aspect)
			{
				case DeviceResourceViewAspect::none:
					break;
				case DeviceResourceViewAspect::color:
					out.aspectMask |= vk::ImageAspectFlagBits::eColor;
					break;
				case DeviceResourceViewAspect::depth:
					out.aspectMask |= vk::ImageAspectFlagBits::eDepth;
					break;
				case DeviceResourceViewAspect::stencil:
					out.aspectMask |= vk::ImageAspectFlagBits::eStencil;
					break;
				case DeviceResourceViewAspect::depth_stencil:
					out.aspectMask |= vk::ImageAspectFlagBits::eDepth | vk::ImageAspectFlagBits::eStencil;
					break;
			}

			return out;
		}
	} // namespace detail

	VulkanImageView::VulkanImageView(DeviceImage* image, DeviceImageViewCreateInfo&& info)
		: DeviceImageView(image, std::move(info))
	{
		const auto create_info = vk::ImageViewCreateInfo()
									 .setImage(static_cast<VulkanImage*>(image)->get_image())
									 .setViewType(detail::to_image_view_type(get_create_info().dimensions))
									 .setFormat(to_vulkan_format(image->get_create_info().format))
									 .setComponents(vk::ComponentMapping()) // TODO: Do we need swizzling?
									 .setSubresourceRange(detail::to_image_subresource_range(get_create_info()));
		m_ImageView = g_vulkan_device->get_device().createImageView(create_info);
	}
} // namespace aw::render