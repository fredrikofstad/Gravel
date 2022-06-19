#pragma once

#include "Engine/Core/Layer.h"
#include "Engine/Events/Event.h"
#include "Engine/Events/KeyEvent.h"
#include "Engine/Events/ApplicationEvent.h"
#include "Engine/Events/MouseEvent.h"



namespace Gravel {

	class GRAVEL_API ImguiLayer : public Layer
	{
	public:
		ImguiLayer();
		~ImguiLayer();

		void OnAttach() override;
		void OnDetach() override;
		void OnImguiRender() override;

		void Start();
		void End();


	private:
		float m_time;
	};

}