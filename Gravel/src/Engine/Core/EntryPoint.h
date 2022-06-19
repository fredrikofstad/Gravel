#pragma once

#ifdef GR_PLATFORM_WINDOWS

extern Gravel::Application* Gravel::CreateApplication();

int main(int argc, char** argv)
{

	Gravel::Log::Init();
	GR_CORE_WARN("Initialized Gravel Log.");
	GR_INFO("Initialized Client Log.");

	auto app = Gravel::CreateApplication();
	app->Run();
	delete app;

}

#endif