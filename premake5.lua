workspace "Luxia"
	architecture "x64"

	-- What configurations do we have
	configurations {
		"Debug",
		"Release",
		"Distribution"
	}

-- this will be Debug-Windows-x64

project "Luxia"
	location "Luxia"
	kind "SharedLib"
	language "C++"

	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
	
	-- Set target build directories
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "lxpch.h"
	pchsource "Luxia/src/lxpch.cpp"

	-- What files are we looking for / copying
	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glad/glad.c",
		--"%{prj.name}/vendor/glm/glm/**.cpp",
		--"%{prj.name}/vendor/glm/glm/**.inl"
	}


	-- what should be included?
	includedirs{
		"%{prj.name}/src",
		"%{prj.name}/vendor",
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/vendor/glm",
		--"%{prj.name}/vendor/yaml-cpp/include",
	}


	filter "system:windows"
		cppdialect "C++20"
		staticruntime "Off"
		systemversion "latest"

		-- Define what should be defined
		defines{
			"LUXIA_EXPORT",
			"LUXIA_PLATFORM_WINDOWS",
			"LUXIA_RENDERER_OPENGL"
		}

		buildoptions { "/utf-8" }
		linkoptions { "/SUBSYSTEM:CONSOLE" }

		-- Copy file from the buildtarget to bin/outputdir/Talloren
		postbuildcommands{
			('{COPY} %{cfg.buildtarget.relpath} ../bin/' .. outputdir .. '/Talloren')
		}

		
		libdirs { "Luxia/vendor/glfw", "Luxia/vendor/assimp/lib" }
		links { "glfw3dll", "opengl32", "assimp-vc143-mtd" }

	-- Specify how to build different configs
	filter "configurations:Debug"
		defines "LUXIA_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "LUXIA_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Distribution"
		defines "LUXIA_DIST"
		optimize "Full"



project "Talloren"
	location "Talloren"
	kind "ConsoleApp"
	language "C++"

	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

	-- Set target build directories
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	-- What files are we looking for / copying
	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	-- what should be included?
	includedirs{
		"Luxia/vendor",
		"Luxia/vendor/spdlog/include",
		"Luxia/vendor/glm",
		"Luxia/src",
		--"Luxia/vendor/yaml-cpp/include",
	}

	libdirs {
		"Luxia/vendor/glfw",
		"Luxia/vendor/assimp/lib",
	}
	links{
		"Luxia",
		"opengl32",
		"assimp-vc143-mtd"
	}

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "Off"
		systemversion "latest"

		-- Define what should be defined
		defines{
			"LUXIA_PLATFORM_WINDOWS",
			"LUXIA_RENDERER_OPENGL"
		}

		buildoptions { "/utf-8" }
		linkoptions { "/SUBSYSTEM:CONSOLE" }


	-- Specify how to build different configs
	filter "configurations:Debug"
		defines "LUXIA_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "LUXIA_RELEASE"
		optimize "On"

	filter "configurations:Distribution"
		defines "LUXIA_DIST"
		optimize "On"