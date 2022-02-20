workspace "Clove"
	architecture "x86_64"

	configurations {
		"Debug",
		"Release",
	}

	startproject "CinnamonEditor"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Clove/vendor/GLFW/include"
IncludeDir["GLAD"] = "Clove/vendor/GLAD/include"
IncludeDir["ImGui"] = "Clove/vendor/ImGui"
IncludeDir["stb"] = "Clove/vendor/stb"
IncludeDir["glm"] = "Clove/vendor/glm"
IncludeDir["entt"] = "Clove/vendor/entt/include"

include "Clove/vendor/GLFW"
include "Clove/vendor/GLAD"
include "Clove/vendor/ImGui"

project "Clove"
	location "Clove"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}") 

	pchheader "clovepch.h"
	pchsource "%{prj.name}/src/clovepch.cpp"

	defines {
		"GLFW_INCLUDE_NONE"
	}

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",

		"Clove/vendor/stb/**.cpp",
		"Clove/vendor/src/**.h"
	}

	includedirs {
		"%{prj.name}/src",
		"%{prj.name}/src/Clove",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.entt}"
	}

	links { 
		"GLFW",
		"GLAD",
		"ImGui",
		"opengl32"
	}

	libdirs {
		"vendor/GLAD/bin/" ..outputdir .. "/GLAD",
		"vendor/GLFW/bin/" ..outputdir .. "/GLFW"
	}

	filter "system:windows"
		systemversion "latest"
		defines {
			"CLOVE_WINDOWS",
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter "configurations:Debug"
		defines "CLOVE_DEBUG"
		symbols "on"

	filter "configurations:Release"
		defines "CLOVE_RELEASE"
		optimize "on"



project "CinnamonEditor"
	location "CinnamonEditor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	defines {
		"_CRT_SECURE_NO_WARNINGS"
	}

	files {
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/src/**.h"
	}

	includedirs {
		"Clove/src",
		"%{IncludeDir.glm}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.entt}"
	}

	links {
		"Clove", "ImGui"
	}

	libdirs {
		"bin/" .. outputdir .. "/Clove",
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"




project "Game"
	location "Game"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	defines {
		"_CRT_SECURE_NO_WARNINGS"
	}

	files {
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/src/**.h"
	}

	includedirs {
		"Clove/src",
		"%{IncludeDir.glm}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.entt}"
	}

	links {
		"Clove", "ImGui"
	}

	libdirs {
		"bin/" .. outputdir .. "/Clove",
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
