workspace "TituEngine"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}
	
	startproject "Sandbox"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "TituEngine/vendor/GLFW/include"
IncludeDir["Glad"] = "TituEngine/vendor/Glad/include"
IncludeDir["ImGui"] = "TituEngine/vendor/imgui"

--like c++ this will COPY the content of the path here
include "TituEngine/vendor/Glad"
include "TituEngine/vendor/GLFW"
include "TituEngine/vendor/imgui"

project "TituEngine"
	location "TituEngine"
	kind "SharedLib"
	language "C++"
	staticruntime "off"

	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "tepch.h"
	pchsource "TituEngine/src/tepch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"TE_BUILD_DLL",
			"TE_PLATFORM_WINDOWS",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "TE_ENABLE_ASSERTS"
		defines "TE_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "TE_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "TE_DIST"
		runtime "Release"
		symbols "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	staticruntime "off"

	language "C++"

	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"TituEngine/vendor/spdlog/include",
		"TituEngine/src"
	}

	links
	{
		"TituEngine"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"TE_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "TE_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "TE_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "TE_DIST"
		runtime "Release"
		symbols "On"