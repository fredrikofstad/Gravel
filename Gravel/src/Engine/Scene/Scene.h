#pragma once

#include "entt.hpp"

#include "Engine/Core/Timestep.h"

namespace Gravel {

	class Scene
	{
	public:
		Scene();
		~Scene();

		entt::entity CreateEntity();

		entt::registry& Reg() { return m_registry; }

		void OnUpdate(Timestep ts);
	private:
		entt::registry m_registry;
	};

}