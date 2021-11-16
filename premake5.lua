workspace "TituEngine"
	architecture "x86_64"

	configurations
	{
		"Debug",
		"Release"
	}
	
	startproject "TituEditor"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "TituEngine/vendor/GLFW/include"
IncludeDir["Glad"] = "TituEngine/vendor/Glad/include"
IncludeDir["ImGui"] = "TituEngine/vendor/imgui"
IncludeDir["glm"] = "TituEngine/vendor/glm"
IncludeDir["stb"] = "TituEngine/vendor/stb"
IncludeDir["entt"] = "TituEngine/vendor/entt/include"
IncludeDir["yaml"] = "TituEngine/vendor/yaml-cpp/include"
IncludeDir["ImGuizmo"] = "TituEngine/vendor/imguizmo"
IncludeDir["TituSignals"] = "TituEngine/vendor/titusignals/include"

--like c++ this will COPY the content of the path here
group "Dependencies"
include "TituEngine/vendor/Glad"
include "TituEngine/vendor/GLFW"
include "TituEngine/vendor/imgui"
include "TituEngine/vendor/yaml-cpp"
group ""

project "TituEngine"
	location "TituEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "tepch.h"
	pchsource "TituEngine/src/tepch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
		"%{prj.name}/vendor/stb/**.h",
		"%{prj.name}/vendor/stb/**.hpp",
		"%{prj.name}/vendor/stb/**.cpp",
		"%{prj.name}/vendor/imguizmo/ImGuizmo.h",
		"%{prj.name}/vendor/imguizmo/ImGuizmo.cpp"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}
	
	includedirs
	{
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.TituSignals}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"yaml-cpp",
		"opengl32.lib"
	}
	
	filter "files:ImGuizmo.cpp"
		flags {"NoPCH"}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"TE_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "TE_ENABLE_ASSERTS"
		defines "TE_ENABLE_PROFILER"
		defines "TE_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "TE_RELEASE"
		runtime "Release"
		optimize "on"

project "TituEditor"
	location "TituEditor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"

	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"TituEngine/vendor/spdlog/include",
		"TituEngine/src",
		"TituEngine/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.TituSignals}"
	}

	links
	{
		"TituEngine",
		"GLFW"
	}

	filter "system:windows"
		staticruntime "off"
		systemversion "latest"

	filter "configurations:Debug"
		defines "TE_DEBUG"
		defines "TE_ENABLE_PROFILER"
		defines "TE_ENABLE_ASSERTS"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "TE_RELEASE"
		runtime "Release"
		optimize "on"