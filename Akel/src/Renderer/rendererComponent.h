// This file is a part of Akel
// CREATED : 23/09/2021
// UPDATED : 15/02/2022

#ifndef __AK_RENDERER_COMPONENT__
#define __AK_RENDERER_COMPONENT__

#include <Core/core.h>
#include <Renderer/instance.h>

namespace Ak
{
    class RendererComponent : public Component
    {
        public:
            RendererComponent(SDL_Window* window);
            RendererComponent();

            void onAttach() override;
            void update() override;
            void onQuit() override;

            void setShader(std::string vertexShader, std::string fragmentShader);
            void render_to_window(SDL_Window* window);

            ~RendererComponent() = default;

        private:
            Instance _instance;
            std::string _vertexShader = Core::getMainDirPath() + "Akel/src/Shaders/basic_2D/basic_2D.vert.spv";
            std::string _fragmentShader = Core::getMainDirPath() + "Akel/src/Shaders/basic_2D/basic_2D.frag.spv";
            SDL_Window* _window;
    };
}

#endif // __AK_RENDERER_COMPONENT__
