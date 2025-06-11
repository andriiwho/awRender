#include "aw/render/api/render_state.h"

namespace aw::render
{
	DeviceResourceState RenderState::get_resource_state(DeviceResource* resource) const
	{
		if (const auto iter = m_ResourceStates.find(resource); iter != m_ResourceStates.end())
		{
			return iter->second;
		}
		
		start_resource_tracking(resource);
		return resource->get_state();
	}

	void RenderState::start_resource_tracking(DeviceResource* resource) const
	{
		if (m_ResourceStates.contains(resource))
		{
			return;
		}

		update_resource_state(resource, resource->get_state());
	}

	void RenderState::update_resource_state(DeviceResource* resource, const DeviceResourceState state) const
	{
		m_ResourceStates[resource] = state;
	}

	void RenderState::clear_resource_states() const
	{
		m_ResourceStates.clear();
	}

	void RenderState::flush_resource_states() const
	{
		for (auto [resource, state] : m_ResourceStates)
		{
			resource->update_state(state);
		}
	}
} // namespace aw::render