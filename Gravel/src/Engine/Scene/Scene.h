#pragma once

#include "Engine/Camera/GardenCamera.h"

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
		void DestroyEntity(Entity entity);

		void OnUpdateRuntime(Timestep deltaTime);
		void OnUpdateEditor(Timestep deltaTime, GardenCamera& camera);
		
		void OnViewportResize(uint32_t width, uint32_t height);

		entt::registry& GetRegistry() { return m_registry; }
		Entity GetPrimaryCamera();

	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
	private:
		entt::registry m_registry;
		uint32_t m_viewportWidth = 0, m_viewportHeight = 0;

		friend class Entity;
		friend class HierarchyPanel;
		friend class SceneSerializer;
	};

}