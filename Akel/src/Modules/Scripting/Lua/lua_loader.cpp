// This file is a part of Akel
// Authors : @kbz_8
// Created : 04/11/2022
// Updated : 21/03/2023

#include <Modules/Scripting/Lua/lua_loader.h>
#include <Modules/Scripting/Lua/lua_script.h>
#include <Platform/input.h>
#include <Core/core.h>

namespace Ak
{
	static std::optional<sol::state> state;

	LuaLoader::LuaLoader(const Input& input) : ScriptLoader()
	{
		if(state)
			return;
		state = sol::state();
		state->open_libraries(sol::lib::base, sol::lib::package, sol::lib::math, sol::lib::table, sol::lib::os, sol::lib::string);
		bindLogs();
		bindInputs(input);
	}

	Script* LuaLoader::loadScript(std::filesystem::path lua_file)
	{
		if(!std::filesystem::exists(lua_file))
		{
			Core::log::report(ERROR, "Lua Loader : invalid script path '%s'", lua_file.string().c_str());
			return nullptr;
		}
		
		LuaScript* script;

		auto sol_script = state->script_file(lua_file.string(), sol::script_pass_on_error);
		if(!sol_script.valid())
		{
			sol::error err = sol_script;
			Core::log::report(ERROR, "Lua Error : %s", err.what());
			Core::log::report(ERROR, "Lua Loader : failed to load and execute Lua script '%s'", lua_file.string().c_str());
			return nullptr;
		}

		script = memAlloc<LuaScript>();

		sol::protected_function::set_default_handler((*state)["Ak"]["log"]["error"]);

		script->_on_init = (*state)["AkelOnInit"];
		script->_on_update = (*state)["AkelOnUpdate"];
		script->_on_quit = (*state)["AkelOnQuit"];

		state->collect_garbage();

		return script;
	}

	void LuaLoader::bindLogs()
	{
		auto log = (*state)["Ak"].get_or_create<sol::table>()["log"].get_or_create<sol::table>();

		log.set_function("message", [](std::string message)
				{ Core::log::report(MESSAGE, "Lua : %s", message.c_str()); });
		log.set_function("warning", [](std::string message)
				{ Core::log::report(WARNING, "Lua : %s", message.c_str()); });
		log.set_function("strong_warning", [](std::string message)
				{ Core::log::report(STRONG_WARNING, "Lua : %s", message.c_str()); });
		log.set_function("error", [](std::string message)
				{ Core::log::report(ERROR, "Lua : %s", message.c_str()); });
		log.set_function("fatal_error", [](std::string message)
				{ Core::log::report(FATAL_ERROR, "Lua : %s", message.c_str()); });
	}

	void LuaLoader::bindInputs(const Input& in)
	{
		auto input = (*state)["Ak"].get_or_create<sol::table>();

		input.set_function("getInKey", [&](SDL_Scancode key, action kind) -> bool
				{ return in.getInKey(key, kind); });
		input.set_function("getInMouse", [&](uint8_t key, action kind) -> bool
				{ return in.getInMouse(key, kind); });
		input.set_function("isMouseMoving", [&]() -> bool
				{ return in.isMouseMoving(); });

		input.set_function("getMouseX", [&]() -> int
				{ return in.getX(); });
		input.set_function("getMouseY", [&]() -> int
				{ return in.getY(); });
		input.set_function("getMouseXRel", [&]() -> int
				{ return in.getXRel(); });
		input.set_function("getMouseYRel", [&]() -> int
				{ return in.getYRel(); });

		input.set_function("finish", [&]() -> void
				{ return const_cast<Input&>(in).finish(); });
		input.set_function("run", [&]() -> void
				{ return const_cast<Input&>(in).run(); });

		std::initializer_list<std::pair<sol::string_view, action>> actions = {
			{ "down", action::down },
			{ "up", action::up },
        };
        input.new_enum<action, false>("action", actions);

		std::initializer_list<std::pair<sol::string_view, SDL_Scancode>> keyItems = {
			{ "A", AK_KEY_A },
			{ "B", AK_KEY_B },
			{ "C", AK_KEY_C },
			{ "D", AK_KEY_D },
			{ "E", AK_KEY_E },
			{ "F", AK_KEY_F },
			{ "G", AK_KEY_G },
			{ "H", AK_KEY_H },
			{ "I", AK_KEY_I },
			{ "J", AK_KEY_J },
			{ "K", AK_KEY_K },
			{ "L", AK_KEY_L },
			{ "M", AK_KEY_M },
			{ "N", AK_KEY_N },
			{ "O", AK_KEY_O },
			{ "P", AK_KEY_P },
			{ "Q", AK_KEY_Q },
			{ "R", AK_KEY_R },
			{ "S", AK_KEY_S },
			{ "T", AK_KEY_T },
			{ "U", AK_KEY_U },
			{ "V", AK_KEY_V },
			{ "W", AK_KEY_W },
			{ "X", AK_KEY_X },
			{ "Y", AK_KEY_Y },
			{ "Z", AK_KEY_Z },

			{ "0", AK_KEY_0 },
			{ "1", AK_KEY_1 },
			{ "2", AK_KEY_2 },
			{ "3", AK_KEY_3 },
			{ "4", AK_KEY_4 },
			{ "5", AK_KEY_5 },
			{ "6", AK_KEY_6 },
			{ "7", AK_KEY_7 },
			{ "8", AK_KEY_8 },
			{ "9", AK_KEY_9 },
        };
        input.new_enum<SDL_Scancode, false>("key", keyItems);

		std::initializer_list<std::pair<sol::string_view, uint8_t>> mouseItems = {
			{ "left", AK_MOUSE_BUTTON_LEFT },
			{ "middle", AK_MOUSE_BUTTON_MIDDLE },
			{ "right", AK_MOUSE_BUTTON_RIGHT },
			{ "x1", AK_MOUSE_BUTTON_X1 },
			{ "x2", AK_MOUSE_BUTTON_X2 },
        };
        input.new_enum<uint8_t, false>("mouse", mouseItems);
	}
}
