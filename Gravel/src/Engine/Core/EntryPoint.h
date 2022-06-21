#pragma once

#ifdef GR_PLATFORM_WINDOWS

extern Gravel::Application* Gravel::CreateApplication();

int main(int argc, char** argv)
{

	Gravel::Log::Init();
	GR_CORE_WARN("Initialized Gravel Log.");
	GR_INFO("Initialized Client Log.");

	GR_PROFILE_BEGIN_SESSION("Startup", "log/Profile-Startup.json");
	auto app = Gravel::CreateApplication();
	GR_PROFILE_END_SESSION();

	GR_PROFILE_BEGIN_SESSION("Runtime", "log/Profile-Runtime.json");
	app->Run();
	GR_PROFILE_END_SESSION();

	GR_PROFILE_BEGIN_SESSION("Startup", "log/Profile-Shutdown.json");
	delete app;
	GR_PROFILE_END_SESSION();
}

#endif
