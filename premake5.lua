workspace "TituEngine"
	architecture "x86_64"

	configurations
	{
		"Debug",
		"Release"
	}
	
	startproject "TituEditor"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

VULKAN_SDK = os.getenv("VULKAN_SDK")

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
IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"
IncludeDir["assimp"] = "TituEngine/vendor/assimp/include"


LibraryDir = {}
LibraryDir["VulkanSDK"]			 = "%{VULKAN_SDK}/Lib"
LibraryDir["VulkanSDK_Debug"]	 = "%{wks.location}/TituEngine/vendor/VulkanSDK/Lib"
LibraryDir["assimp"]			 = "%{wks.location}/TituEngine/vendor/assimp/lib"

Library = {}
Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"
Library["VulkanUtils"] = "%{LibraryDir.VulkanSDK}/VkLayer_utils.lib"

Library["ShaderC_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/shaderc_sharedd.lib"
Library["SPIRV_Cross_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/spirv-cross-cored.lib"
Library["SPIRV_Cross_GLSL_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/spirv-cross-glsld.lib"
Library["SPIRV_Tools_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/SPIRV-Toolsd.lib"

Library["ShaderC_Release"] = "%{LibraryDir.VulkanSDK}/shaderc_shared.lib"
Library["SPIRV_Cross_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-core.lib"
Library["SPIRV_Cross_GLSL_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsl.lib"

Library["assimp_release"] = "%{LibraryDir.assimp}/Release/assimp-vc142-mt.lib"
Library["assimp_debug"] = "%{LibraryDir.assimp}/Debug/assimp-vc142-mtd.lib"

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
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
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
		"%{IncludeDir.TituSignals}",
		"%{IncludeDir.VulkanSDK}",
		"%{IncludeDir.assimp}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"yaml-cpp",
	}
	
	filter "files:TituEngine/vendor/imguizmo/**.cpp"
	flags { "NoPCH" }

	filter "system:windows"
		systemversion "latest"

		defines
		{
		}

	filter "configurations:Debug"
		defines "TE_ENABLE_ASSERTS"
		defines "TE_ENABLE_PROFILER"
		defines "TE_DEBUG"
		runtime "Debug"
		symbols "on"
		
		links
		{
			"%{Library.ShaderC_Debug}",
			"%{Library.SPIRV_Cross_Debug}",
			"%{Library.SPIRV_Cross_GLSL_Debug}",
			"%{Library.assimp_debug}"
		}

	filter "configurations:Release"
		defines "TE_RELEASE"
		runtime "Release"
		optimize "on"

		links
		{
			"%{Library.ShaderC_Release}",
			"%{Library.SPIRV_Cross_Release}",
			"%{Library.SPIRV_Cross_GLSL_Release}",
			"%{Library.assimp_release}"
		}

project "TituEditor"
	location "TituEditor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

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
