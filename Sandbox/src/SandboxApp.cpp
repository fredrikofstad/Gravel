#include <Gravel.h>

class ExampleLayer : public Gravel::Layer
{
public:
	ExampleLayer()
		: Layer("Example"){}

	void OnUpdate() override
	{
		GR_INFO("ExampleLayer::Update");
	}

	void OnEvent(Gravel::Event& event) override
	{
		GR_TRACE("{0}", event);
	}
};

class Sandbox : public Gravel::Application
{
public:
	Sandbox()
	{
		AddLayer(new ExampleLayer());
		AddOverlay(new Gravel::ImguiLayer());
	}
	~Sandbox()
	{

	}
};


Gravel::Application* Gravel::CreateApplication()
{
	return new Sandbox();
}
