#pragma once

#include "pixel_format.h"
#include "aw/core/all.h"

namespace aw::render
{
	enum class DeviceResourceType
	{
		none,
		buffer,
		image,
	};

	enum class DeviceResourceState
	{
		undefined,
		color_attachment,
		depth_attachment,
		depth_read_only,
		stencil_attachment,
		stencil_read_only,
		depth_stencil_attachment,
		depth_stencil_read_only,
		depth_read_only_stencil,
		shader_resource,
		copy_src,
		copy_dst,
		present,
	};

	class DeviceResource : public core::IntrusiveRefCounted
	{
	public:
		explicit DeviceResource(DeviceResourceType type);

		DeviceResourceType get_type() const { return m_Type; }

		/**
		 * Get the resource state of this resource.
		 * Note: this should not be used in the middle of command buffer recording.
		 * Command buffers update states after they are closed.
		 */
		DeviceResourceState get_state() const { return m_State; }


	private:
		friend class RenderState;

		void update_state(const DeviceResourceState state) { m_State = state; }

		DeviceResourceType m_Type;
		DeviceResourceState m_State = DeviceResourceState::undefined;
	};

	class ViewableDeviceResource : public DeviceResource
	{
	public:
		explicit ViewableDeviceResource(DeviceResourceType type);
	};

	enum class DeviceBufferUsage : core::u32
	{
		none = 0,
		vertex = 1 << 0,
		index = 1 << 1,
		uniform = 1 << 2,
		storage = 1 << 3,
		copy_src = 1 << 4,
		copy_dst = 1 << 5,
		sequential_write = 1 << 6,
		random_access = 1 << 7,
		initially_mapped = 1 << 8,
		all = 0xFFFFFFFF
	};
	aw_enum_flags(DeviceBufferUsage);

	struct DeviceBufferCreateInfo
	{
		std::string debug_name{};
		core::u64 size_in_bytes{};
		core::EnumMask<DeviceBufferUsage> usage{};
	};

	class DeviceBuffer : public DeviceResource
	{
	public:
		explicit DeviceBuffer(DeviceBufferCreateInfo&& info);

		const DeviceBufferCreateInfo& get_create_info() const { return m_CreateInfo; }

		virtual void* map() = 0;
		virtual void unmap() = 0;

	private:
		DeviceBufferCreateInfo m_CreateInfo{};
	};

	enum class DeviceImageUsage : core::u8
	{
		none = 0,
		copy_src = 1 << 0,
		copy_dst = 1 << 1,
		sampled = 1 << 2,
		storage = 1 << 3,
		color_attachment = 1 << 4,
		depth_stencil_attachment = 1 << 5,
		all = 0xFF
	};
	aw_enum_flags(DeviceImageUsage);

	enum class DeviceImageType : core::u8
	{
		image_1d,
		image_2d,
		image_3d,
	};

	struct DeviceImageCreateInfo
	{
		std::string debug_name{};
		DeviceImageType type{};
		core::u32 mip_levels{1};
		core::u32 array_layers{1};
		core::u32 width{};
		core::u32 height{};
		core::u32 depth{1};
		PixelFormat format{};
		core::EnumMask<DeviceImageUsage> usage{};
	};

	class DeviceImage : public ViewableDeviceResource
	{
	public:
		explicit DeviceImage(DeviceImageCreateInfo&& info);

		const DeviceImageCreateInfo& get_create_info() const { return m_CreateInfo; }

	private:
		DeviceImageCreateInfo m_CreateInfo{};
	};
}