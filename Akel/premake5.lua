project "Akel"
	kind "SharedLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	architecture "x86_64"

	objdir ("../build-int/" .. outputdir)

	pchheader "src/Akpch.h"
	pchsource "src/Akpch.cpp"
	flags "NoPCH"
	linkoptions "-shared"

	filter "system:windows"
		targetdir ("../libs/lib/Windows")
		libdirs "../libs/lib/Windows"

	filter "system:linux"
		targetdir ("../libs/lib/Linux")
		libdirs "../libs/lib/Linux"

	links
	{
		"vulkan",
		"SDL2",
		"SDL2_image",
		"SDL2_mixer",
		"SDL2_ttf"
	}

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	defines "SDL_MAIN_HANDLED"

	includedirs 
	{
		"../libs/include",
		"%{prj.location}/src"
	}


	filter "configurations:Debug"
        defines "AK_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "AK_RELEASE"
        optimize "On"
