#pragma once

#include "entt.hpp"
#include "Engine/Core/Timestep.h"

namespace Gravel {

	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());

		entt::registry& GetRegistry() { return m_registry; }

		void OnUpdate(Timestep deltaTime);
	private:
		entt::registry m_registry;
		friend class Entity;
	};

}