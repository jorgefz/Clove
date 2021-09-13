workspace "Clove"
	architecture "x86"

	configurations {
		"Debug",
		"Release",
	}

	startproject "Game"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Clove/vendor/GLFW/include"
IncludeDir["GLAD"] = "Clove/vendor/GLAD/include"
IncludeDir["ImGui"] = "Clove/vendor/ImGui"
IncludeDir["stb"] = "Clove/vendor/stb/include"
IncludeDir["glm"] = "Clove/vendor/glm"

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
	pchsource "%{prj.name}/src/Clove/clovepch.cpp"

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs {
		"%{prj.name}/src",
		"%{prj.name}/src/Clove",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb}"
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
			"CLOVE_WINDOWS"
		}

	filter "configurations:Debug"
		defines "CLOVE_DEBUG"
		symbols "on"

	filter "configurations:Release"
		defines "CLOVE_RELEASE"
		optimize "on"



project "Game"
	location "Game"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}") 

	files {
		"%{prj.name}/src/App.cpp"
	}

	includedirs {
		"Clove/src",
		"%{IncludeDir.glm}"
	}

	links {
		"Clove"
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
