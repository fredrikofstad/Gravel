#pragma once

#include "Engine/Layer.h"

namespace Gravel {

	class GRAVEL_API ImguiLayer : public Layer
	{
	public:
		ImguiLayer();
		~ImguiLayer();

		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate() override;
		void OnEvent(Event& event) override;
	private:
		float m_time;
	};

}