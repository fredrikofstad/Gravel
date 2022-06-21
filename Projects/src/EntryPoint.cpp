#include <Gravel.h>
#include <Engine/Core/EntryPoint.h>

#include "Test2D.h"
#include "ExampleLayer.h"

class Sandbox : public Gravel::Application
{
public:
	Sandbox()
	{
		//AddLayer(new ExampleLayer());
		AddLayer(new Test2D());
	}

	~Sandbox()
	{

	}
};

Gravel::Application* Gravel::CreateApplication()
{
	return new Sandbox();
}