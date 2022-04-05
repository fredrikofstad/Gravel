#include <Gravel.h>
class Sandbox : public Gravel::Application
{
public:
	Sandbox()
	{

	}
	~Sandbox()
	{

	}
};


Gravel::Application* Gravel::CreateApplication()
{
	return new Sandbox();
}
