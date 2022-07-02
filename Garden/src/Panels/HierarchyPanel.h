#pragma once

#include <Gravel.h>

namespace Gravel {

	class HierarchyPanel
	{
	public:
		HierarchyPanel() = default;
		HierarchyPanel(const Shared<Scene>& scene);

		void SetScene(const Shared<Scene>& scene);

		void OnImguiRender();

	private:

		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);

	private:
		Shared<Scene> m_scene;
		Entity m_selection;
	};


}