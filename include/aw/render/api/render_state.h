#pragma once

#include "aw/core/all.h"
#include "device_resources.h"

namespace aw::render
{
	class RenderState : public core::IntrusiveRefCounted
	{
	public:
		DeviceResourceState get_resource_state(DeviceResource* resource) const;


	protected:
		void update_resource_state(DeviceResource* resource, DeviceResourceState state) const;
		void clear_resource_states() const;
		void flush_resource_states() const;
		void start_resource_tracking(DeviceResource* resource) const;

	private:
		mutable std::unordered_map<DeviceResource*, DeviceResourceState> m_ResourceStates;
	};
}