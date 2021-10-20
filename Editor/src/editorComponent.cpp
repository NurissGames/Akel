// This file is a part of the Akel editor
// CREATED : 06/07/2021
// UPDATED : 15/10/2021

#include <editorComponent.h>

EditorComponent::EditorComponent() : Ak::ImGuiComponent("Akel Editor")
{
	_eltm = Ak::make_unique_ptr_w<Ak::ELTM>(Ak::custom_malloc<Ak::ELTM>());
}

void EditorComponent::onAttach()
{
	Ak::ImGuiComponent::setSettingsFilePath(std::string(Ak::Core::getMainDirPath() + "Editor/settings/editor.ini").c_str());
	Ak::ImGuiComponent::onAttach();
	_eltm->load(Ak::Core::getMainDirPath() + "Editor/texts/En/main.eltm");
	_console = Ak::make_unique_ptr_w<Console>(Ak::custom_malloc<Console>(_eltm->getText("Console.name")));
}

void EditorComponent::onImGuiRender()
{
	drawMainMenuBar();
	_console->render(WindowComponent::getSize().X, WindowComponent::getSize().Y);
	if(_showAbout)
		drawAboutWindow();
}

void EditorComponent::onEvent(Ak::Input& input)
{
	Ak::ImGuiComponent::onEvent(input);
	if(!_running || _console->_sh.quit())
		input.finish();
}

void EditorComponent::onQuit()
{
	Ak::ImGuiComponent::onQuit();
}

void EditorComponent::drawMainMenuBar()
{
	if(ImGui::BeginMainMenuBar())
	{
		if(ImGui::BeginMenu(_eltm->getText("MainMenuBar.file").c_str()))
		{
			if(ImGui::MenuItem(_eltm->getText("MainMenuBar.open").c_str(), "Ctrl+O")) { /* Do stuff */ }
			if(ImGui::MenuItem(_eltm->getText("MainMenuBar.save").c_str(), "Ctrl+S")) { /* Do stuff */ }
			if(ImGui::MenuItem(_eltm->getText("MainMenuBar.quit").c_str()))
				_running = false;
			ImGui::EndMenu();
		}
		if(ImGui::BeginMenu(_eltm->getText("MainMenuBar.edit").c_str()))
		{
			if(ImGui::MenuItem(_eltm->getText("MainMenuBar.addFile").c_str())) { /* Do stuff */ }
			if(ImGui::MenuItem(_eltm->getText("MainMenuBar.build").c_str()))   { /* Do stuff */ }
			ImGui::EndMenu();
		}
		if(ImGui::BeginMenu(_eltm->getText("MainMenuBar.help").c_str()))
		{
			if(ImGui::MenuItem(_eltm->getText("MainMenuBar.about").c_str()))
				_showAbout = _showAbout ? false : true;
			ImGui::EndMenu();
		}
		ImGui::SameLine(ImGui::GetColumnWidth(-1));
		ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);

		ImGui::EndMainMenuBar();
	}
}

void EditorComponent::drawAboutWindow()
{
	if(ImGui::Begin(_eltm->getText("MainMenuBar.about").data(), nullptr, ImGuiWindowFlags_NoResize))
	{
		ImGui::TextUnformatted(_eltm->getText("MainMenuBar.version").data());
		ImGui::End();
	}
}
