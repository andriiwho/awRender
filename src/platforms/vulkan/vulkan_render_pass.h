#pragma once

#include "aw/render/api/render_pass.h"
#include "vulkan_common.h"

namespace aw::render
{
	class VulkanRenderPass final : public IRenderPass
	{
	public:
		explicit VulkanRenderPass(const std::string& debug_name);
		~VulkanRenderPass() override;

		vk::RenderPass get_render_pass() const { return m_RenderPass; }

		void build() override;

		void add_pass_image(const PassImageDesc& image_desc) override;

		std::span<const vk::ClearValue> get_clear_values() const { return m_ClearValues; }

	private:
		std::string m_DebugName{};
		vk::raii::RenderPass m_RenderPass{nullptr};

		std::vector<PassImageDesc> m_Images{};
		std::vector<vk::ClearValue> m_ClearValues{};
	};
}