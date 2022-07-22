// This file is a part of Akel Studio
// Authors : @kbz_8
// Created : 28/10/2021
// Updated : 10/07/2022

#include <Panels/eltm_editor.h>

ELTM_editor::ELTM_editor(std::shared_ptr<Ak::ELTM> eltm, std::string* input_buffer, uint8_t* save) : Panel("__eltm_editor")
{
    _eltm = std::move(eltm);
    _save = save;
	_loader = Ak::create_Unique_ptr<Ak::ELTM>(false);
    _input_buffer = input_buffer;
}

const char* eltm_writter_formating(std::string text)
{
    size_t found = 0;
    while((found = text.find('\t', found + 2)) != std::string::npos)
    {
        text[found] = '\\';
        text.insert(text.begin() + found + 1, 't');
    }

    found = 0;
    while((found = text.find('\n', found + 6)) != std::string::npos)
    {
        text.insert(found, "\\n");
        text.insert(found + 3, "\t\t");
    }

    if(text.find('\n') != std::string::npos)
    {
        text.insert(text.begin(), '(');
        text.insert(text.end(), ')');
    }

    return text.c_str();
}

void ELTM_editor::onUpdate(Ak::Maths::Vec2<int>& size)
{
	/*
    if(!_input_buffer->empty())
    {
        if(!_loader->reload(*_input_buffer))
            Ak::Core::log::report(ERROR, "ELTM editor: Couldn't load %s", _input_buffer->c_str());
        _file = *_input_buffer;
        _texts.clear();
        for(auto it = _loader->getCurrentTexts().begin(); it != _loader->getCurrentTexts().end(); ++it)
            _texts.emplace_back(it->first, it->second);
        _modules.clear();
        for(auto it = _loader->getCurrentModules().begin(); it != _loader->getCurrentModules().end(); ++it)
        {
            for(auto it2 = it->second.begin(); it2 != it->second.end(); ++it2)
                _modules.emplace_back(it->first, { std::make_pair(it2->first, it2->second) });
        }
    }
    if(!_is_open)
        return;

	if(ImGui::Begin(_eltm->getLocalText("ELTM_Editor.name").c_str(), &_is_open, ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
    {
        ImGui::Text("File : %s", _file.c_str());

        editor();

        ImGui::Separator();

        static std::string id;
        static std::string text;

        if(ImGui::Button(_eltm->getLocalText("ELTM_Editor.add").c_str()))
        {
            size_t found = 0;
            if((found = id.find('.')) != std::string::npos)
            {
                std::string mod(id.begin(), id.begin() + found);
                std::string id2(id.begin() + found + 1, id.end());
                
                auto it = std::find_if(_modules.begin(), _modules.end(), [&](auto pair) { return pair.first == mod; });
                if(it != _modules.end())
                    it->second.emplace_back(id2, text);
                else
                    _modules.emplace_back(mod, { std::make_pair(id2, text) });
            }
            else
                _texts.emplace_back(id, text);
        }

        ImGui::SameLine(0);

        ImGui::PushItemWidth(255);
        ImGui::InputTextWithHint("##ID", "ID", id.data(), id.capacity(), ImGuiInputTextFlags_CallbackResize, InputCallback, (void*)&id);
        ImGui::PopItemWidth();

        ImGui::SameLine(0);
        
        ImGui::InputTextWithHint("##TEXT", _eltm->getLocalText("ELTM_Editor.text").c_str(), text.data(), text.capacity(), ImGuiInputTextFlags_CallbackResize, InputCallback, (void*)&text);

        ImGui::TextUnformatted(_eltm->getLocalText("ELTM_Editor.version").c_str());

		ImGui::End();
    }

    static std::string destination = "";
    if(*_save == 1)
        destination = _file;
    else if(*_save == 2)
        destination = std::move(pfd::save_file("Save", Ak::Core::getMainDirPath() + "New_File.eltm", { "ELTM files (.eltm .tm)", "*.eltm *.tm", "All Files", "*" }, pfd::opt::force_overwrite).result());

    if(*_save != 0)
    {
        std::ofstream out(destination.c_str(), std::ofstream::out);
        if(out.is_open())
        {
            out << "// This is an ELTM file generated by the Akel Studio ELTM editor\n" << std::endl;
            for(auto it = _texts.end(); it != _texts.begin(); --it)
                out << "set " << it->first << " = " << eltm_writter_formating(it->second) << std::endl;

            for(auto it = _modules.end(); it != _modules.begin(); --it)
            {
                out << "\nbegin module " << it->first << std::endl;
                for(auto it2 = it->second.end(); it2 != it->second.begin(); --it2)
                    out << "\tset " << it2->first << " = " << eltm_writter_formating(it2->second) << std::endl;
                out << "end module" << std::endl;
            }
            
            out.close();
        }
        *_save = 0;
    }
	*/
}

void ELTM_editor::onQuit()
{
    _loader.reset(nullptr);
}

void ELTM_editor::editor()
{
    if(ImGui::BeginChild("Scrolling", ImVec2(0, _height - 75), true))
    {
        static ImGuiTableFlags flags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody;

        if(ImGui::BeginTable("3ways", 2, flags))
        {
            ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_NoHide);
            ImGui::TableSetupColumn(_eltm->getLocalText("ELTM_Editor.text").c_str(), ImGuiTableColumnFlags_NoHide);
            ImGui::TableHeadersRow();

            for(auto it = _texts.end(); it!= _texts.begin(); --it)
            {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();

                ImGui::TextUnformatted(it->first.data());
                ImGui::TableNextColumn();
                if(it->second.find('\n') != std::string::npos)
                    ImGui::InputTextMultiline(std::string("##" + it->first).c_str(), it->second.data(), it->second.capacity(), {0, 0}, ImGuiInputTextFlags_CallbackResize | ImGuiInputTextFlags_AllowTabInput, InputCallback, (void*)&it->second);
                else
                    ImGui::InputText(std::string("##" + it->first).c_str(), it->second.data(), it->second.capacity(), ImGuiInputTextFlags_CallbackResize | ImGuiInputTextFlags_AllowTabInput, InputCallback, (void*)&it->second);
                it->second.shrink_to_fit();
            }

            for(auto it = _modules.end(); it != _modules.begin(); --it)
            {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();

                bool isopen = ImGui::TreeNodeEx(std::string(it->first + " (Module)").c_str(), ImGuiTreeNodeFlags_SpanFullWidth);
                ImGui::TableNextColumn();
                ImGui::TextDisabled("--");

                if(isopen)
                {
                    for(auto it2 = it->second.end(); it2 != it->second.begin(); --it2)
                    {
                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();

                        ImGui::TextUnformatted(it2->first.data());
                        ImGui::TableNextColumn();
                        if(it2->second.find('\n') != std::string::npos)
                            ImGui::InputTextMultiline(std::string("##" + it2->first).c_str(), it2->second.data(), it2->second.capacity(), {0, 0}, ImGuiInputTextFlags_CtrlEnterForNewLine | ImGuiInputTextFlags_CallbackResize | ImGuiInputTextFlags_EnterReturnsTrue, InputCallback, (void*)&it2->second);
                        else
                            ImGui::InputText(std::string("##" + it2->first).c_str(), it2->second.data(), it2->second.capacity(), ImGuiInputTextFlags_CallbackResize, InputCallback, (void*)&it2->second);
                        it2->second.shrink_to_fit();
                    }

                    ImGui::TreePop();
                }
            }

            ImGui::EndTable();
        }

	    ImGui::EndChild();
    }
}

int ELTM_editor::InputCallback(ImGuiInputTextCallbackData* data)
{
    if(data->EventFlag == ImGuiInputTextFlags_CallbackResize)
    {
        std::string* str = (std::string*)data->UserData;
        str->resize(data->BufTextLen + 1);
        data->Buf = (char*)str->c_str();
    }

    return 0;
}
