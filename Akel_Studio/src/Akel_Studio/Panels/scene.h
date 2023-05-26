// This file is a part of Akel Studio
// Authors : @kbz_8
// Created : 12/03/2022
// Updated : 25/05/2023

#ifndef __AK_STUDIO_SCENE__
#define __AK_STUDIO_SCENE__

#include <AkSpch.h>
#include <Panels/Base/panel.h>

class Scene : public Panel
{
    public:
        Scene(std::shared_ptr<Ak::ELTM> eltm, Ak::Core::ProjectFile& project, class SceneCamera* camera);

        void onUpdate(Ak::Maths::Vec2<int>& size) override;
		void onQuit() override;

        ~Scene() = default;

    private:
		AkImGui::ImImage _play;
		class SceneCamera* _camera = nullptr;
        uint8_t _aspect_checked = 0;
		bool _grid = true;
};

#endif // __AK_STUDIO_SCENE__
