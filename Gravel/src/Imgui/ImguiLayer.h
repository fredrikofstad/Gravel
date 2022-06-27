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

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& e) override;

		void Start();
		void End();

		void BlockEvents(bool block) { m_blockEvents = block; }

	private:
		float m_time;
		bool m_blockEvents = true;
	};

}