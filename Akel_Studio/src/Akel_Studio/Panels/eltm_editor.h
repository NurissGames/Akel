// This file is a part of Akel Studio
// Authors : @kbz_8
// Created : 28/10/2021
// Updated : 17/09/2023

#ifndef __AK_STUDIO_ELTM_EDITOR__
#define __AK_STUDIO_ELTM_EDITOR__

#include <AkSpch.h>
#include <Panels/Base/panel.h>

class ELTM_editor : public Panel
{
    public:
        ELTM_editor(std::shared_ptr<Ak::ELTM> eltm, Ak::Core::ProjectFile& project);
        void onUpdate(Ak::Maths::Vec2<int>& size) override;
        void onQuit() override;
        inline void onOpen() override { _is_open = !_is_open; }
        inline bool isOpen() override { return _is_open; }
        ~ELTM_editor() = default;

    private:
        void editor();
        static int InputCallback(ImGuiInputTextCallbackData* data);

		std::unordered_map<std::string, std::string> _texts;
		std::unordered_map<std::string, std::unordered_map<std::string, std::string>> _modules;

        std::string _file;
        std::string _input_buffer;
        Ak::UniquePtr<Ak::ELTM> _loader;
        bool _is_open = false;
};

#endif // __AK_STUDIO_ELTM_EDITOR__
