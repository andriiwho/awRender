#pragma once

#include "vk_mem_alloc.h"
#include "aw/render/api/device_resources.h"

#include "vulkan_common.h"
#include <variant>

namespace aw::render
{
	class VulkanImage final : public DeviceImage
	{
	public:
		explicit VulkanImage(DeviceImageCreateInfo&& info);
		VulkanImage(DeviceImageCreateInfo&& info, vk::Image unowned_image);
		~VulkanImage() override;

		bool owns_image() const { return std::holds_alternative<vk::raii::Image>(m_ImageHandle); }

		vk::Image get_unowned_handle() const { return std::get<vk::Image>(m_ImageHandle); }
		vk::raii::Image& get_owned_handle() { return std::get<vk::raii::Image>(m_ImageHandle); }
		const vk::raii::Image& get_owned_handle() const { return std::get<vk::raii::Image>(m_ImageHandle); }

	private:
		std::variant<std::monostate, vk::raii::Image, vk::Image> m_ImageHandle{};
		VmaAllocation m_Allocation{};
	};
}