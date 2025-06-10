#pragma once

#include "aw/core/all.h"

namespace aw::render
{
	enum class DeviceResourceType
	{
		none,
		buffer,
		image,
	};

	class DeviceResource : public core::IntrusiveRefCounted
	{
	public:
		explicit DeviceResource(DeviceResourceType type);

		DeviceResourceType get_type() const { return m_Type; }

	private:
		DeviceResourceType m_Type;
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
		core::String debug_name{};
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
}