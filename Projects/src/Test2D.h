#pragma once

#include <Gravel.h>

class Test2D : public Gravel::Layer
{
public:
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	void OnUpdate(Gravel::Timestep deltaTime) override;
	virtual void OnImguiRender() override;
	virtual void OnEvent(Gravel::Event& e) override;
private:
};