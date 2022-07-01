#include "grpch.h"
#include "Scene.h"

#include "Components.h"
#include "Entity.h"
#include "Engine/Renderer/Renderer2D.h"
#include <glm/glm.hpp>

namespace Gravel {

	Scene::Scene()
	{

	}

	Scene::~Scene()
	{

	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		return entity;
	}

	void Scene::OnUpdate(Timestep deltaTime)
	{
		// update script components

		{
			m_registry.view<CodeComponent>().each([=](auto entity, auto& component)
				{
					if (!component.Instance)
					{
						component.Instance = component.Instantiate();
						component.Instance->m_entity = Entity{ entity, this };
						component.Instance->OnCreate();
					}

					component.Instance->OnUpdate(deltaTime);
				});
		}

		// camera

		Camera* mainCamera = nullptr;
		glm::mat4* cameraTransform = nullptr;

		auto view = m_registry.view<TransformComponent, CameraComponent>();
		for (auto entity : view)
		{
			auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

			if (camera.Primary)
			{
				mainCamera = &camera.Camera;
				cameraTransform = &transform.Transform;
				break;
			}
		}

		if (mainCamera)
		{
			Renderer2D::StartScene(mainCamera->GetProjection(), *cameraTransform);

			auto group = m_registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				Renderer2D::DrawQuad(transform, sprite.Color);
			}

			Renderer2D::EndScene();
		}

	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_viewportWidth = width;
		m_viewportHeight = height;

		// Resize our non-FixedAspectRatio cameras
		auto view = m_registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio)
				cameraComponent.Camera.SetViewportSize(width, height);
		}

	}

}