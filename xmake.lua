-- This file is a part of Akel
-- Author : @kbz_8
-- Created : 02/10/2021
-- Updated : 15/11/2022

-- Globals settings
add_repositories("local-repo libs")

add_requires("spirv-reflect", "imgui_sdl_vk v1.87-docking", "imguizmo_sdl_vk", "libsdl", "libsdl_image", "libsdl_ttf", "libsndfile", "openal-soft", "vulkan-memory-allocator", "sol2", "nlohmann_json")

add_rules("mode.debug", "mode.release")
set_languages("cxx17")

set_objectdir("build/objects/$(os)_$(arch)")
set_targetdir("build/$(os)_$(arch)")

-- Lib Akel Build
target("Akel")
	set_default(true)
	set_license("MIT")
	set_kind("static")
	add_files("Akel/src/**.cpp")
	add_includedirs("Akel/src", "libs/include")

	if is_mode("debug") then
		add_defines("AK_DEBUG")
	elseif is_mode("release") then
		add_defines("AK_RELEASE")
	end

	set_pcxxheader("Akel/src/Akpch.h")
	
	add_defines("SDL_MAIN_HANDLED")

	add_packages("spirv-reflect", { public = true })
	add_packages("libsdl_image",  { public = true })
	add_packages("libsdl",		  { public = true })
	add_packages("libsdl_ttf",    { public = true })
	add_packages("imgui_sdl_vk",  { public = true })
	add_packages("sol2",  		  { public = true })
	add_packages("nlohmann_json", { public = true })
	add_packages("vulkan-memory-allocator", { public = true })

	add_packages("libsndfile",    { public = true })
	add_packages("openal-soft",   { public = true })
target_end() -- optional but I think the code is cleaner with this

-- Akel Studio Build
target("Akel_Studio")
	set_default(true)
	set_license("MIT")
    set_kind("binary")
	add_includedirs("Akel/src", "Akel_Studio/src", "libs/include")
    add_deps("Akel")
	
    add_files("Akel_Studio/src/**.cpp")

	set_objectdir("Akel_Studio/build/objects/$(os)_$(arch)")
	set_targetdir("Akel_Studio/")

	add_packages("imguizmo_sdl_vk")
	add_packages("spirv-reflect")

	set_pcxxheader("Akel_Studio/src/AkSpch.h")

	if is_mode("debug") then
		add_defines("AK_STUDIO_DEBUG")
	elseif is_mode("release") then
		add_defines("AK_STUDIO_RELEASE")
	end
target_end()

-- Rectangle Demo Build
target("RectDemo")
	set_default(false)
    set_kind("binary")
    add_deps("Akel")
	add_includedirs("Akel/src", "SandBox/rect/src", "libs/include")
    add_files("SandBox/rect/src/*.cpp")
target_end()

-- Cube Demo Build
target("CubeDemo")
	set_default(false)
    set_kind("binary")
    add_deps("Akel")
	add_includedirs("Akel/src", "SandBox/cube/src", "libs/include")
    add_files("SandBox/cube/src/*.cpp")
target_end()

-- Kila tester Build
target("Kila_tester")
	set_default(false)
    set_kind("binary")
    add_deps("Akel")
	set_targetdir("Tests/Kila/")
	add_includedirs("Akel/src", "Tests/Kila", "libs/include")
    add_files("Tests/Kila/*.cpp")
target_end()

-- ELTM tester Build
target("eltm_tester")
	set_default(false)
    set_kind("binary")
    add_deps("Akel")
	set_targetdir("Tests/eltm/")
	add_includedirs("Akel/src", "Tests/eltm", "libs/include")
    add_files("Tests/eltm/*.cpp")
target_end()

-- Multipe Windows Rendering tester Build
target("mult_win_render")
	set_default(false)
    set_kind("binary")
    add_deps("Akel")
	set_targetdir("Tests/Windowing/multiple_windows_rendering/")
	add_includedirs("Akel/src", "libs/include")
    add_files("Tests/Windowing/multiple_windows_rendering/*.cpp")
target_end()

-- Lua tester build
target("lua")
	set_default(false)
    set_kind("binary")
    add_deps("Akel")
	set_targetdir("Tests/Scripting/Lua/")
	add_includedirs("Akel/src", "libs/include")
    add_files("Tests/Scripting/Lua/*.cpp")
target_end()
