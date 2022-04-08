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

include "Gravel/vendor/GLFW"


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
		--"%{IncludeDir.GLFW}"
	}

	links
	{
		"GLFW",
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