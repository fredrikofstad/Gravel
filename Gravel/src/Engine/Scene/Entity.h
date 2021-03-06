#pragma once

#include "Scene.h"
#include "entt.hpp"

namespace Gravel {

	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);
		Entity(const Entity& other) = default;

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			GR_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");
			T& component = m_scene->m_registry.emplace<T>(m_entityHandle, std::forward<Args>(args)...);
			m_scene->OnComponentAdded<T>(*this, component);
			return component;
		}

		template<typename T>
		T& GetComponent()
		{
			GR_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			return m_scene->m_registry.get<T>(m_entityHandle);
		}

		template<typename T>
		bool HasComponent()
		{
			return m_scene->m_registry.all_of<T>(m_entityHandle);
		}

		template<typename T>
		void RemoveComponent()
		{
			GR_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			m_scene->m_registry.remove<T>(m_entityHandle);
		}

		operator bool() const { return m_entityHandle  !=  entt::null; }

		operator entt::entity() const { return m_entityHandle; }

		operator uint32_t() const { return (uint32_t)m_entityHandle; }

		bool operator==(const Entity& other) const
		{
			return m_entityHandle == other.m_entityHandle && m_scene == other.m_scene;
		}

		bool operator!=(const Entity& other) const
		{
			return !(*this == other);
		}

	private:
		entt::entity m_entityHandle{ entt::null };
		Scene* m_scene = nullptr;
	};

}