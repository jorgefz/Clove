workspace "Clove"
	architecture "x86"

	configurations {
		"Debug",
		"Release",
	}

	startproject "Game"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLAD"] = "vendor/GLAD/include"

include "vendor/GLAD"

project "Clove"
	location "Clove"
	kind "StaticLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}") 

	pchheader "clovepch.h"
	pchsource "%{prj.name}/src/Clove/clovepch.cpp"

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs {
		"vendor/GLFW/include",
		"vendor/GLAD/include",
		"vendor/stb/include",
		"vendor/glm",
	}

	links { 
		"glfw3",
		"GLAD",
		"opengl32"
	}

	libdirs {
		"vendor/GLFW/lib-vc2019",
		"vendor/GLAD/bin/" ..outputdir .. "/GLAD"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"
		defines {
			"CLOVE_WINDOWS"
		}

		--postbuildcommands {
		--	("{COPY} %{cfg.buildtarget.replath} ../bin/" .. outputdir .. "/Game")
		--}

	filter "configurations:Debug"
		defines "CLOVE_DEBUG"
		symbols "On"
		buildoptions "/MDd"

	filter "configurations:Release"
		defines "CLOVE_RELEASE"
		optimize "On"
		buildoptions "/MD"

project "Game"
	location "Game"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}") 

	files {
		"%{prj.name}/src/App.cpp"
	}

	includedirs {
		"Clove/src",
		"vendor/GLFW/include",
		"vendor/GLAD/include",
		"vendor/stb/include",
		"vendor/glm",
	}

	links {
		"glfw3",
	 	"GLAD",
	 	"opengl32",
		"Clove"
	}

	libdirs {
		"bin/" .. outputdir .. "/Clove",
		"vendor/GLFW/lib-vc2019",
		"vendor/GLAD/bin/" ..outputdir .. "/GLAD"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

	filter "configurations:Debug"
		symbols "On"
		buildoptions "/MDd"

	filter "configurations:Release"
		optimize "On"
		buildoptions "/MD"
