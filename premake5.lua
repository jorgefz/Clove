workspace "Clove"
	architecture "x86"

	configurations {
		"Debug",
		"Release",
	}

	startproject "Game"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Clove"
	location "Clove"
	kind "StaticLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}") 

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs {
		"vendor/GLFW/include",
		"vendor/GLEW/include",
		"vendor/stb/include",
		"vendor/glm",
	}

	links { 
		"glfw3",
		"glew32s",
		"opengl32"
	}

	libdirs {
		"vendor/GLFW/lib-vc2019",
		"vendor/GLEW/lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines {
			"GLEW_STATIC",
			--"WIN32",
		}

		--postbuildcommands {
		--	("{COPY} %{cfg.buildtarget.replath} ../bin/" .. outputdir .. "/Game")
		--}

	filter "configurations:Debug"
		--defines "_DEBUG"
		symbols "On"
		buildoptions "/MDd"

	filter "configurations:Release"
		--defines "_RELEASE"
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
		"vendor/GLEW/include",
		"vendor/stb/include",
		"vendor/glm",
	}

	links {
		"glfw3",
	 	"glew32s",
	 	"opengl32",
		"Clove"
	}

	libdirs {
		"bin/" .. outputdir .. "/Clove",
		"vendor/GLFW/lib-vc2019",
		"vendor/GLEW/lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines {
			"GLEW_STATIC",
			--"WIN32",
		}

	filter "configurations:Debug"
		--defines "_DEBUG"
		symbols "On"
		buildoptions "/MDd"

	filter "configurations:Release"
		--defines "_RELEASE"
		optimize "On"
		buildoptions "/MD"
