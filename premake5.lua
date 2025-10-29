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

	-- What files are we looking for / copying
	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glad/glad.c"
	}


	-- what should be included?
	includedirs{
		"%{prj.name}/src",
		"%{prj.name}/vendor",
		"%{prj.name}/vendor/spdlog/include"
	}


	filter "system:windows"
		cppdialect "C++20"
		staticruntime "Off"
		systemversion "latest"

		-- Define what should be defined
		defines{
			"LUXIA_EXPORT",
			"LUXIA_PLATFORM_WINDOWS"
		}

		buildoptions { "/utf-8" }
		linkoptions { "/SUBSYSTEM:CONSOLE" }

		-- Copy file from the buildtarget to bin/outputdir/Talloren
		postbuildcommands{
			('{COPY} %{cfg.buildtarget.relpath} ../bin/' .. outputdir .. '/Talloren')
		}

		
		libdirs { "Luxia/vendor/glfw" }
		links { "glfw3dll" }

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
		"Luxia/src"
	}


	links{
		"Luxia",
	}

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "Off"
		systemversion "latest"

		-- Define what should be defined
		defines{
			"LUXIA_PLATFORM_WINDOWS"
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