// This file is a part of Akel
// Authors : @kbz_8
// Created : 28/03/2021
// Updated : 09/03/2022

#ifndef __AK_WINDOW__
#define __AK_WINDOW__

#include <Akpch.h>
#include <Core/core.h>
#include <Maths/maths.h>
#include <Platform/input.h>
#include <Utils/utils.h>
#include <Platform/messageBox.h>
#include <Platform/input.h>

#define AK_WINDOW_MAX_SIZE 0xFFFF
#define AK_WINDOW_POS_CENTER 0xFFFE

enum class win
{
    title,
    size,
    position,
    fullscreen,
    border,
    brightness,
    opacity,
    resizable,
    visible,
    maximumSize,
    minimumSize,
    icon,
    vsync,
    maximize
};

namespace Ak
{

    class WindowComponent : public Component
    {
        public:
            WindowComponent();

			void onAttach() override;
			void onEvent(Input& input) override;
			void onQuit() override;

			void set(win setting, const char* value);
			void set(win setting, bool value);
			void set(win setting, float value);
			void set(win setting, uint16_t x, uint16_t y);

            // Getters
            std::variant get(win setting);

            inline const std::string& getTitle() const noexcept { return _title; }
            inline const Maths::Vec2<uint16_t>& getPosition() const noexcept { return _position; }
            const Maths::Vec2<uint16_t>& getSize();
			inline SDL_Window* getNativeWindow() const noexcept { return _window; }
            inline bool getVsync() noexcept { return _vsync; }

            ~WindowComponent() override;

		protected:
			void create();
            SDL_Window* _window = nullptr;

        private:

            std::string _title = "";
            Maths::Vec2<uint16_t> _position;
            Maths::Vec2<uint16_t> _size;

            uint32_t _flags = 0;
            bool _vsync = true;

            SDL_Surface* _icon = nullptr;
    };
}

#endif // __AK_WINDOW__
