workspace "Luxia"
	architecture "x64"

	-- What configurations do we have
	configurations {
		"Debug",
		"Release",
		"Distribution"
	}


-- All cpp / c files that need to be included
project "LuxiaVendor"
    location "Luxia/vendor"
    kind "StaticLib"
    language "C++"

    files { 
		"Luxia/vendor/imgui/**.cpp",
		"Luxia/vendor/glad/**.c",
	}
    includedirs { 
		"Luxia/vendor/imgui", 
		"Luxia/vendor",
		"Luxia/vendor/yaml-cpp/include",
	}


	filter "configurations:Debug"
		links { "yaml-cppd" }
		libdirs { "Luxia/vendor/yaml-cpp/build/Debug" }

	filter "configurations:Release"
		links { "yaml-cpp" }
		libdirs { "Luxia/vendor/yaml-cpp/build/Release" }

	filter {}

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"

		-- Define what should be defined
		defines{
			"LUXIA_EXPORT",
			"LUXIA_PLATFORM_WINDOWS",
			"LUXIA_RENDERER_OPENGL", 
		}

		-- buildoptions { "/utf-8" }
		-- linkoptions { "/SUBSYSTEM:CONSOLE" }

	

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
	}


	-- what should be included?
	includedirs{
		"%{prj.name}/src",
		"%{prj.name}/vendor",
		"%{prj.name}/vendor/glm",
		"%{prj.name}/vendor/imgui",
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/vendor/yaml-cpp/include",
	}

	links { "LuxiaVendor" }

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "Off"
		systemversion "latest"

		-- Define what should be defined
		defines{
			"LUXIA_EXPORT",
			"LUXIA_PLATFORM_WINDOWS",
			"LUXIA_RENDERER_OPENGL", 
		}

		buildoptions { "/utf-8", "/wd4251" } -- Ignores the "needs to be dll interfaced warning"
		linkoptions { "/SUBSYSTEM:CONSOLE" }

		-- Copy file from the buildtarget to bin/outputdir/Editor
		postbuildcommands{
			('{COPY} %{cfg.buildtarget.relpath} ../bin/' .. outputdir .. '/Editor'),
		}

		
		libdirs { "Luxia/vendor/glfw", "Luxia/vendor/assimp/lib" }
		links { 
			"glfw3dll",
			"opengl32", 
			"assimp-vc143-mtd",
		}

	-- Specify how to build different configs
	filter "configurations:Debug"
		defines "LUXIA_DEBUG"
		runtime "Debug"
	    links { "yaml-cppd" }
		libdirs { "Luxia/vendor/yaml-cpp/build/Debug" }
		symbols "On"

	filter "configurations:Release"
		defines "LUXIA_RELEASE"
		runtime "Release"
		links { "yaml-cpp" }
		libdirs { "Luxia/vendor/yaml-cpp/build/Release" }
		optimize "On"

	filter "configurations:Distribution"
		defines "LUXIA_DIST"
		optimize "Full"



project "Editor"
	location "Editor"
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
		"Luxia/vendor/yaml-cpp/include",
		"Luxia/vendor/glm",
		"Luxia/src",
		"Editor/src",
	}

	libdirs {
		"Luxia/vendor/glfw",
		"Luxia/vendor/assimp/lib",
	}
	links{
		"Luxia",
		"LuxiaVendor",
		"opengl32",
		"assimp-vc143-mtd",
	}

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "Off"
		systemversion "latest"

		-- Define what should be defined
		defines{
			"LUXIA_PLATFORM_WINDOWS",
			"LUXIA_RENDERER_OPENGL",
		}

		buildoptions { "/utf-8", "/wd4251" }
		linkoptions { "/SUBSYSTEM:CONSOLE" }


	-- Specify how to build different configs
	filter "configurations:Debug"
		defines "LUXIA_DEBUG"
		links { "yaml-cppd" }
		libdirs { "Luxia/vendor/yaml-cpp/build/Debug" }
		symbols "On"

	filter "configurations:Release"
		defines "LUXIA_RELEASE"
		links { "yaml-cpp" }
		libdirs { "Luxia/vendor/yaml-cpp/build/Release" }
		optimize "On"

	filter "configurations:Distribution"
		defines "LUXIA_DIST"
		optimize "On"