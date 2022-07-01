#pragma once

#include "Entity.h"

namespace Gravel {

	class ScriptableEntity
	{
	public:
		virtual ~ScriptableEntity() = default;

		template<typename T>
		T& GetComponent()
		{
			return m_entity.GetComponent<T>();
		}

	protected:
		virtual void OnCreate() {}
		virtual void OnUpdate(Timestep deltaTime) {}
		virtual void OnDestroy() {}


	private:
		Entity m_entity;
		friend class Scene;
	};

}