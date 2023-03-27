-- This file is a part of Akel
-- Authors : @kbz_8
-- Created : 02/10/2021
-- Updated : 27/03/2023

-- Globals settings
add_repositories("local-repo libs")

add_repositories("nazara-repo https://github.com/NazaraEngine/xmake-repo")
add_requires("nzsl")

add_requires("entt", "spirv-reflect", "imgui_sdl_vk v1.89-docking", "imguizmo_sdl_vk", "libsdl", "stb", "libsndfile", "openal-soft", "vulkan-memory-allocator", "sol2", "nlohmann_json", "tinyobjloader")
add_requires("volk", { configs = { header_only = true}})

add_rules("mode.debug", "mode.release")
set_languages("cxx17")

set_objectdir("build/objects/$(os)_$(arch)")
set_targetdir("build/$(os)_$(arch)")

set_optimize("fastest")

-- Lib Akel Build
target("Akel")
	set_default(true)
	set_license("MIT")
	set_kind("static")
	add_files("Akel/src/**.cpp")
	add_includedirs("Akel/include", "Akel/src", "libs/include")

	if is_mode("debug") then
		add_defines("AK_DEBUG")
	elseif is_mode("release") then
		add_defines("AK_RELEASE")
	end

	set_pcxxheader("Akel/include/Akpch.h")

	add_defines("AK_BUILD")
	add_defines("AK_STATIC")
	add_defines("SDL_MAIN_HANDLED")

	add_packages("spirv-reflect", { public = true })
	add_packages("stb",           { public = true })
	add_packages("libsdl",		  { public = true })
	add_packages("imgui_sdl_vk",  { public = true })
	add_packages("sol2",  		  { public = true })
	add_packages("nlohmann_json", { public = true })
	add_packages("entt", 	      { public = true })
	add_packages("vulkan-memory-allocator", { public = true })
	add_packages("volk",          { public = true })
	add_packages("nzsl",          { public = true })
	add_packages("libsndfile",    { public = true })
	add_packages("openal-soft",   { public = true })
	add_packages("tinyobjloader", { public = true })
target_end() -- optional but I think the code is cleaner with this

-- Akel Studio Launcher Build
target("Akel_Studio")
	set_default(false)
	set_license("MIT")
    set_kind("binary")
	add_includedirs("Akel/include", "Akel_Studio/src/Launcher", "Akel_Studio/src", "libs/include")
    add_deps("Akel")
    add_deps("akelstudio_application")
	
    add_files("Akel_Studio/src/Launcher/**.cpp")

	set_objectdir("Akel_Studio/build/objects/$(os)_$(arch)")
	set_targetdir("Akel_Studio/")

	if is_mode("debug") then
		add_defines("AK_STUDIO_DEBUG")
	elseif is_mode("release") then
		add_defines("AK_STUDIO_RELEASE")
	end
target_end()

-- Akel Studio Build
target("akelstudio_application")
	set_default(false)
	set_license("MIT")
    set_kind("binary")
	add_includedirs("Akel/include", "Akel_Studio/src/Akel_Studio", "Akel_Studio/src", "libs/include")
    add_deps("Akel")
	
    add_files("Akel_Studio/src/Akel_Studio/**.cpp")

	set_objectdir("Akel_Studio/build/objects/$(os)_$(arch)")
	set_targetdir("Akel_Studio/")

	add_packages("imguizmo_sdl_vk")

	set_pcxxheader("Akel_Studio/src/Akel_Studio/AkSpch.h")

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
	add_includedirs("Akel/include", "SandBox/Native/rect/src", "libs/include")
    add_files("SandBox/Native/rect/src/*.cpp")
	set_targetdir("SandBox/Native/rect")
target_end()

-- Cube Demo Build
target("CubeDemo")
	set_default(false)
    set_kind("binary")
    add_deps("Akel")
	add_includedirs("Akel/include", "SandBox/Native/cube/src", "libs/include")
    add_files("SandBox/Native/cube/src/*.cpp")
	set_targetdir("SandBox/Native/cube")
target_end()

-- Skybox Demo Build
target("SkyDemo")
	set_default(false)
    set_kind("binary")
    add_deps("Akel")
	add_includedirs("Akel/include", "SandBox/Native/skybox/src", "libs/include")
    add_files("SandBox/Native/skybox/src/*.cpp")
	set_targetdir("SandBox/Native/skybox")
target_end()

-- Model Demo Build
target("ModelDemo")
	set_default(false)
    set_kind("binary")
    add_deps("Akel")
	add_includedirs("Akel/include", "SandBox/Native/model/src", "libs/include")
    add_files("SandBox/Native/model/src/*.cpp")
	set_targetdir("SandBox/Native/model")
target_end()

-- Scripting Demo Build
target("ScriptDemo")
	set_default(false)
    set_kind("binary")
    add_deps("Akel")
	add_includedirs("Akel/include", "SandBox/Native/script/src", "libs/include")
    add_files("SandBox/Native/script/src/*.cpp")
	set_targetdir("SandBox/Native/script")
target_end()

-- Audio Demo Build
target("AudioDemo")
	set_default(false)
    set_kind("binary")
    add_deps("Akel")
	add_includedirs("Akel/include", "SandBox/Native/audio/src", "libs/include")
    add_files("SandBox/Native/audio/src/*.cpp")
	set_targetdir("SandBox/Native/audio")
target_end()

-- Kila tester Build
target("Kila_tester")
	set_default(false)
    set_kind("binary")
    add_deps("Akel")
	set_targetdir("Tests/Kila/")
	add_includedirs("Akel/include", "Tests/Kila", "libs/include")
    add_files("Tests/Kila/*.cpp")
target_end()
