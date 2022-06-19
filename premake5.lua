workspace "Gravel"
	architecture "x64"
	startproject "Projects"

	configurations
	{
		"Debug",
		"Release",
		"Distribution"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLWF"] = "Gravel/thirdparty/GLFW/include"
IncludeDir["Glad"] = "Gravel/thirdparty/Glad/include"
IncludeDir["Imgui"] = "Gravel/thirdparty/imgui"
IncludeDir["glm"] = "Gravel/thirdparty/glm/"
IncludeDir["stb"] = "Gravel/thirdparty/stb/"


group "Dependencies"
	include "Gravel/thirdparty/GLFW"
	include "Gravel/thirdparty/Glad"
	include "Gravel/thirdparty/imgui"

group ""

project "Gravel"
	location "Gravel"
	kind "StaticLib"
	language "C++"
	cppdialect "c++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "grpch.h"
	pchsource "Gravel/src/grpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/thirdparty/stb/**.h",
		"%{prj.name}/thirdparty/stb/**.cpp",
		"%{prj.name}/thirdparty/glm/glm/**.hpp",
		"%{prj.name}/thirdparty/glm/glm/**.inl",
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/thirdparty/spdlog/include",
		"%{prj.name}/src",
		"%{prj.name}/thirdparty/GLFW/include",
		"%{prj.name}/thirdparty/Glad/include",
		"%{prj.name}/thirdparty/imgui",
		"%{prj.name}/thirdparty/glm",
		"%{prj.name}/thirdparty/stb",


		--"%{IncludeDir.GLFW}"
	}

	links
	{
		"GLFW",
		"Glad",
		"Imgui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"GR_PLATFORM_WINDOWS",
			"GR_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "GR_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "GR_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Distribution"
		defines "GR_DIST"
		runtime "Release"
		optimize "on"

project "Projects"
	location "Projects"
	kind "ConsoleApp"
	language "C++"
	cppdialect "c++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs
	{
		"Gravel/thirdparty/spdlog/include",
		"Gravel/src",
		"Gravel/thirdparty/",
		"%{IncludeDir.glm}",
	}

	links
	{
		"Gravel"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"GR_PLATFORM_WINDOWS"
		}


	filter "configurations:Debug"
		defines "GR_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "GR_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Distribution"
		defines "GR_DIST"
		runtime "Release"
		optimize "on"
