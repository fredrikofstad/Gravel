#pragma once

#ifdef GR_PLATFORM_WINDOWS

extern Gravel::Application* Gravel::CreateApplication();

int main(int argc, char** argv)
{
	// allocate to heap
	auto app = Gravel::CreateApplication();

	app->Run();

	delete app;

}


#endif