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
		void OnViewportResize(uint32_t width, uint32_t height);
	private:
		entt::registry m_registry;
		uint32_t m_viewportWidth = 0, m_viewportHeight = 0;
		friend class Entity;
	};

}