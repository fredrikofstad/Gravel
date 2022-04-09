workspace "Gravel"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Distribution"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLWF"] = "Gravel/vendor/GLFW/include"
IncludeDir["Glad"] = "Gravel/vendor/Glad/include"
IncludeDir["Imgui"] = "Gravel/vendor/imgui/include"

include "Gravel/vendor/GLFW"
include "Gravel/vendor/Glad"
include "Gravel/vendor/imgui"


project "Gravel"
	location "Gravel"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "grpch.h"
	pchsource "Gravel/src/grpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs
	{
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/src",
		"%{prj.name}/vendor/GLFW/include",
		"%{prj.name}/vendor/Glad/include",
		"%{prj.name}/vendor/imgui",
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
		cppdialect "c++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"GR_PLATFORM_WINDOWS",
			"GR_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "GR_DEBUG"
		runtime "Debug"
        buildoptions "/MDd"
		symbols "On"

	filter "configurations:Release"
		defines "GR_RELEASE"
		runtime "Release"
        buildoptions "/MD"
		optimize "On"

	filter "configurations:Distribution"
		defines "GR_DIST"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs
	{
		"Gravel/vendor/spdlog/include",
		"Gravel/src"
	}

	links
	{
		"Gravel"
	}

	filter "system:windows"
		cppdialect "c++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"GR_PLATFORM_WINDOWS"
		}


	filter "configurations:Debug"
		defines "GR_DEBUG"
		buildoptions "/MDd"
		symbols "On"

	filter "configurations:Release"
		defines "GR_RELEASE"
		buildoptions "/MD"
		optimize "On"

	filter "configurations:Distribution"
		defines "GR_DIST"
		optimize "On"