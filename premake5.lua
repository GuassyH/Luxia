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
	toolset "msc-v145"

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
	toolset "msc-v145"

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
		"$(MONO_PROJECT)include/mono-2.0",
		"%{prj.name}/vendor",
		"%{prj.name}/vendor/glm",
		"%{prj.name}/vendor/imgui",
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/vendor/yaml-cpp/include",
		"%{prj.name}/vendor/assimp/include",
		"%{prj.name}/vendor/Jolt"

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
			"JPH_SHARED_LIBRARY",
			"JPH_FLOATING_POINT_EXCEPTIONS_ENABLED",
			"JPH_DEBUG_RENDERER",
			"JPH_PROFILE_ENABLED",
			"JPH_OBJECT_STREAM"
		}
		
		buildoptions { "/utf-8", "/wd4251" } -- Ignores the "needs to be dll interfaced warning"
		linkoptions { "/SUBSYSTEM:CONSOLE" }

		-- Copy file from the buildtarget to bin/outputdir/Editor
		postbuildcommands{
			('{COPY} "%{cfg.buildtarget.relpath}" "../bin/' .. outputdir .. '/Editor"'),
			('{COPY} "$(MONO_PROJECT)bin/mono-2.0-sgen.dll" "../bin/' .. outputdir .. '/Editor"'),
			('{COPYDIR} "$(MONO_PROJECT)lib/mono" "../bin/' .. outputdir .. '/Editor/mono"'),
			-- ('{COPY} "%{prj.name}/vendor/Jolt/lib/Jolt.dll" "../bin/' .. outputdir .. '/Editor"'),
			-- ('{COPY} "%{prj.name}/vendor/glfw/glfw3.dll" "../bin/' .. outputdir .. '/Editor"'),
		}

		
		libdirs { 
			"%{prj.name}/vendor/glfw", 
			"%{prj.name}/vendor/Jolt/lib",
			"$(MONO_PROJECT)lib"
		}
		links { 
			"glfw3dll",
			"opengl32", 
			"Jolt",
			"mono-2.0-sgen"
		}

	-- Specify how to build different configs
	filter "configurations:Debug"
		defines "LUXIA_DEBUG"
		runtime "Debug"
	    links { 
			"yaml-cppd",
			"assimp-vc143-mtd"
		}
		libdirs { 
			"%{prj.name}/vendor/yaml-cpp/build/Debug",
			"%{prj.name}/vendor/assimp/build/lib/Debug",
		}

		-- Why no worky
		postbuildcommands{
			-- ('{COPY} "Luxia/vendor/yaml-cpp/build/Debug/yaml-cppd.dll" "../bin/' .. outputdir .. '/Editor"'),
			-- ('{COPY} "Luxia/vendor/assimp/build/lib/Debug/assimp-vc143-mtd.dll" "../bin/' .. outputdir .. '/Editor"'),
		}

		symbols "On"

	filter "configurations:Release"
		defines "LUXIA_RELEASE"
		runtime "Release"
	    links { 
			"yaml-cpp",
			"assimp-vc143-mt"
		}
		libdirs { 
			"%{prj.name}/vendor/yaml-cpp/build/Release",
			"%{prj.name}/vendor/assimp/build/lib/Release",
		}

		postbuildcommands{
			-- ('{COPY} "Luxia/vendor/yaml-cpp/build/Release/yaml-cpp.dll" "../bin/' .. outputdir .. '/Editor"'),
			-- ('{COPY} "Luxia/vendor/assimp/build/lib/Release/assimp-vc143-mt.dll" "../bin/' .. outputdir .. '/Editor"'),
		}

		optimize "On"

	filter "configurations:Distribution"
		defines "LUXIA_DIST"
		optimize "Full"



project "Editor"
	location "Editor"
	kind "ConsoleApp"
	language "C++"
	toolset "msc-v145"

	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

	-- Set target build directories
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	-- What files are we looking for / copying
	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"Luxia/vendor/glad/**.c"
	}

	-- what should be included?
	includedirs{
		"Luxia/vendor",
		"Luxia/vendor/spdlog/include",
		"Luxia/vendor/yaml-cpp/include",
		"Luxia/vendor/glm",
		"Luxia/vendor/assimp/include",
		"Luxia/src",
		"Luxia/vendor/Jolt",
		"Editor/src",
	}

	libdirs {
		"Luxia/vendor/glfw",
		"Luxia/vendor/Jolt/lib",
		"mono/4.5",
	}
	links{
		"Luxia",
		"LuxiaVendor",
		"opengl32",
		"glfw3dll",
		"Jolt",
		"mono-2.0-sgen.dll",
	}

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "Off"
		systemversion "latest"

		-- Define what should be defined
		defines{
			"LUXIA_PLATFORM_WINDOWS",
			"LUXIA_RENDERER_OPENGL",
			"JPH_SHARED_LIBRARY",
			"JPH_FLOATING_POINT_EXCEPTIONS_ENABLED",
			"JPH_DEBUG_RENDERER",
			"JPH_PROFILE_ENABLED",
			"JPH_OBJECT_STREAM"
		}

		buildoptions { "/utf-8", "/wd4251" }
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


project "LuxiaBehaviour"
	location "LuxiaBehaviour"
	kind "SharedLib"
	language "C#"
	dotnetframework "4.8"

	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
	
	-- Set target build directories
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"LuxiaBehaviour/**.cs"
	}

	postbuildcommands{
		('{COPY} "%{cfg.buildtarget.relpath}" "../bin/' .. outputdir .. '/Editor"'),
	}