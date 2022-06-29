#include "grpch.h"
#include "Entity.h"

namespace Gravel {

	Entity::Entity(entt::entity handle, Scene* scene)
		:m_entityHandle(handle), m_scene(scene)
	{

	}

}