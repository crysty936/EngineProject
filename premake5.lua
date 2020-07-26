workspace "EngineProject"
	architecture "x64"

	configurations
	{
	"Debug",
	"Release",
	"Dist"
	}

	outputdir = "%{cfg.buildcfg}"

	IncludeDir ={}
	IncludeDir["GLFW"]="EngineProject/vendor/GLFW/include"
	IncludeDir["Glad"]="EngineProject/vendor/Glad/include"
	IncludeDir["Imgui"]="EngineProject/vendor/imgui"
	IncludeDir["glm"]="EngineProject/vendor/glm"

	include "EngineProject/vendor/GLFW"
	include "EngineProject/vendor/Glad"
	include "EngineProject/vendor/Imgui"

	project "EngineProject"
		location "EngineProject"
		kind "SharedLib"
		language "C++"

		targetdir ("bin/" .. outputdir .. "/%{prj.name}")
		objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

		pchheader "pch.h"
		pchsource "EngineProject/src/pch.cpp"


		files
		{
			"%{prj.name}/src/**.h",
			"%{prj.name}/src/**.cpp"
		}

		includedirs
		{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.Imgui}",
		"%{IncludeDir.glm}"
		}

		links
		{
			"GLFW",
			"Glad",
			"Imgui",
			"opengl32.lib"
		}


		filter "system:windows"
			cppdialect "C++17"
			staticruntime "Off"
			systemversion "latest"

			defines
			{
				"PLATFORM_WINDOWS",
				"BUILD_DLL",
				"GLFW_INCLUDE_NONE"
			}

			postbuildcommands
			{
				{ "xcopy /y \"$(OutDir)EngineProject.dll\""  .. " \"$(SolutionDir)bin\\$(Configuration)\\SandboxTest\\\"" }
			}

		filter "configurations:Debug"
				defines "ENGINE_DEBUG"
				defines "ENABLE_ASSERTS"
				runtime "Debug"
				symbols "On"

		filter "configurations:Release"
				defines "ENGINE_RELEASE"
				runtime "Release"
				optimize "On"

		filter "configurations:Distributable"
				defines "ENGINE_DIST"
				runtime "Release"
				optimize "On"

	project "SandboxTest"
		location "SandboxTest"
		kind "ConsoleApp"
		language "C++"
		staticruntime "Off"


		
		targetdir ("bin/" .. outputdir .. "/%{prj.name}")
		objdir ("bin-int/" .. outputdir .. "/%{prj.name}")


		files
		{
			"%{prj.name}/src/**.h",
			"%{prj.name}/src/**.cpp"
		}

		includedirs
		{
		"EngineProject/vendor/spdlog/include;",
		"EngineProject/src",
		"%{IncludeDir.glm}"
		}

		links
		{
			"EngineProject"
		}


		filter "system:windows"
			cppdialect "C++17"
			staticruntime "On"
			systemversion "latest"

			defines
			{
				"PLATFORM_WINDOWS"
			}


		filter "configurations:Debug"
				defines "ENGINE_DEBUG"
				runtime "Debug"
				symbols "On"

		filter "configurations:Release"
				defines "ENGINE_RELEASE"
				runtime "Release"
				optimize "On"

		filter "configurations:Distributable"
				defines "ENGINE_DIST"
				runtime "Release"
				optimize "On"