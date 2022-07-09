workspace "Gravel"
	architecture "x64"
	startproject "Garden"

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
IncludeDir["entt"] = "Gravel/thirdparty/entt/include"
IncludeDir["Imgui"] = "Gravel/thirdparty/imgui"
IncludeDir["Imguizmo"] = "Gravel/thirdparty/imguizmo"
IncludeDir["glm"] = "Gravel/thirdparty/glm/"
IncludeDir["stb"] = "Gravel/thirdparty/stb/"
IncludeDir["yaml_cpp"] = "Gravel/thirdparty/yaml-cpp/include"



group "Dependencies"
	include "Gravel/thirdparty/GLFW"
	include "Gravel/thirdparty/Glad"
	include "Gravel/thirdparty/imgui"
	include "Gravel/thirdparty/yaml-cpp"

group ""


project "Gravel"
	location "Gravel"
	kind "StaticLib"
	language "C++"
	cppdialect "c++17"
	staticruntime "off"

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

		"%{prj.name}/thirdparty/imguizmo/ImGuizmo.h",
		"%{prj.name}/thirdparty/imguizmo/ImGuizmo.cpp",
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/thirdparty/spdlog/include",
		"%{prj.name}/thirdparty/GLFW/include",
		"%{prj.name}/thirdparty/Glad/include",
		"%{prj.name}/thirdparty/entt/include",
		"%{prj.name}/thirdparty/imgui",
		"%{prj.name}/thirdparty/imguizmo",
		"%{prj.name}/thirdparty/glm",
		"%{prj.name}/thirdparty/stb",
		"%{prj.name}/thirdparty/yaml-cpp/include",
	}

	links
	{
		"GLFW",
		"Glad",
		"Imgui",
		"yaml-cpp",
		"opengl32.lib",
	}

	filter "files:Gravel/thirdparty/imguizmo/**.cpp"
	flags { "NoPCH" }

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"GR_PLATFORM_WINDOWS",
			"GR_BUILD_DLL",
			"GLFW_INCLUDE_NONE",
			"YAML_CPP_STATIC_DEFINE",
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

project "Tests"
	location "Tests"
	kind "ConsoleApp"
	language "C++"
	cppdialect "c++17"
	staticruntime "off"

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
		"%{IncludeDir.entt}",

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



project "Garden"
	location "Garden"
	kind "ConsoleApp"
	language "C++"
	cppdialect "c++17"
	staticruntime "off"

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
		"%{IncludeDir.entt}",

	}

	links
	{
		"Gravel"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"GR_PLATFORM_WINDOWS",
			"YAML_CPP_STATIC_DEFINE"
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
