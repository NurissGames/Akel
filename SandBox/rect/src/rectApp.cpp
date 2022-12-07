#include <Akel.h>
#include <Akel_main.h>

class FPSprinterComponent : public Ak::Component
{
    public:
        void update() override { std::cout << "fps : " << Ak::CounterFPS::getFPS() << " - ticks : " << Ak::CounterFPS::getTicks() << '\r' << std::flush; }
};

class Rect : public Ak::Application
{
    public:
        Rect() : Ak::Application()
        {
            Ak::WindowComponent* window = Ak::memAlloc<Ak::WindowComponent>();
            add_component(window); // function inherited from Ak::Application
            window->title = "Rectangle using Akel Engine";
            window->resizable = true;
            window->vsync = false;
            window->size.SET(1280, 750);
            window->fetchSettings();

            add_component<FPSprinterComponent>();

            Ak::RendererComponent* renderer = Ak::memAlloc<Ak::RendererComponent>(window);

			renderer->loadCustomShader(Ak::Core::getMainDirPath() + "../../SandBox/rect/src/vert_u.spv");
			renderer->loadCustomShader(Ak::Core::getMainDirPath() + "../../SandBox/rect/src/frag_u.spv");

            add_component(renderer);

			Ak::Scene* scene = Ak::memAlloc<Ak::Scene>("main scene", window);

			Ak::Entity2D rectangle(Models::quad, { 50.0f, 50.0f }, { 100.0f, 100.0f }, Colors::red);
			scene->add_2D_entity(std::move(rectangle));

			Ak::SceneManager* scenes_manager = Ak::memAlloc<Ak::SceneManager>();
			scenes_manager->add_scene(scene);

			add_component(scenes_manager);
        }
        ~Rect() = default;
};

Ak::AkelInstance Akel_init()
{
    Ak::AkelInstance instance;
        instance.project_file_path = Ak::Core::getMainDirPath() + "../../SandBox/rect";
        instance.project_file_name = "rect";
    return instance;    
}

Ak::Application* Akel_mainApp()
{
	return Ak::memAlloc<Rect>();
}
