#pragma once

#include "vulkan_common.h"
#include "aw/render/api/device_interface.h"

#include "vk_mem_alloc.h"

namespace aw::render
{
	class VulkanWindow;
	class VulkanDevice final : public IRenderDeviceInterface
	{
	public:
		explicit VulkanDevice(VulkanWindow* window);
		~VulkanDevice() override;

		bool has_valid_queue_indices() const { return m_GraphicsQueueIndex != UINT32_MAX; }

		IDeviceQueue* create_device_queue(DeviceQueueType queue_type) override;
		IDeviceFence* create_fence() override;
		DeviceCommandList* create_command_list() override;
		DeviceBuffer* create_buffer(DeviceBufferCreateInfo&& create_info) override;
		DeviceImage* create_image(DeviceImageCreateInfo&& create_info) override;
		DeviceImageView* create_image_view(DeviceImage* image, DeviceImageViewCreateInfo&& create_info) override;
		IRenderPass* create_render_pass(const std::string& debug_name) override;
		IFrameBuffer* create_frame_buffer(const FrameBufferCreateInfo& create_info) override;
		ISwapChainFrameBuffer* create_swap_chain_frame_buffer(ISwapChain* swap_chain, IRenderPass* render_pass) override;
		IRenderPipeline* create_render_pipeline(RenderPipelineCreateInfo&& create_info) override;

		const vk::raii::Device& get_device() const { return m_Device; }
		const vk::raii::Instance& get_instance() const { return m_Instance; }
		const vk::raii::PhysicalDevice& get_physical_device() const { return m_PhysicalDevice; }
		const vk::raii::PipelineCache& get_pipeline_cache() const { return m_PipelineCache; }

		ISwapChain* create_swap_chain(const IRenderWindowInterface& window) override;
		core::u32 get_graphics_queue_index() const { return m_GraphicsQueueIndex; }

		IFrameContext* create_frame_context() override;

		void wait_idle() override;
		VmaAllocator get_allocator() const { return m_Allocator; }

	private:
		void init_vulkan_instance();
		void pick_physical_device(const VulkanWindow* window);
		void create_device();
		void init_allocator();
		void init_pipeline_cache();

		static constexpr core::u32 m_ApiVersion = VK_API_VERSION_1_2;

		vk::raii::Context m_Context{};
		vk::raii::Instance m_Instance{nullptr};
		core::Vector<vk::SurfaceKHR> m_CreatedSurfaces;
		vk::raii::DebugUtilsMessengerEXT m_DebugMessenger{nullptr};

		vk::raii::PhysicalDevice m_PhysicalDevice{nullptr};
		vk::raii::Device m_Device{nullptr};
		vk::raii::PipelineCache m_PipelineCache{nullptr};

		core::u32 m_GraphicsQueueIndex = UINT32_MAX;

		VmaAllocator m_Allocator{};
	};
}