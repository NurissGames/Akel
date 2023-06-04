// This file is a part of Akel Studio
// Authors : @kbz_8
// Created : 06/07/2021
// Updated : 05/06/2023

#include <studioComponent.h>
#include <Fonts/material_font.h>
#include <Third_party/imspinner.h>

Ak::Unique_ptr<Ak::ELTM> _lang_eltm(nullptr);

StudioComponent::StudioComponent(Ak::CommandLineArgs args) : Ak::Component("studio_component"), _eltm(Ak::create_shared_ptr_w<Ak::ELTM>())
{
	std::filesystem::path path = args[1];
	if(!std::filesystem::exists(path))
		Ak::FatalError("Akel Studio : invalid project path");
	if(path.extension() != ".akel")
		Ak::FatalError("Akel Studio : invalid project file, unkown extension '%s'", path.extension().string().c_str());
	_project.setDir(path.parent_path());
	_project.setName(path.stem().string());

	std::filesystem::path runtime_path(path.parent_path() / "settings.akrt");
	if(std::filesystem::exists(runtime_path))
	{
		std::ifstream file(std::move(runtime_path), std::ios::binary);
		if(!file.is_open())
			Ak::FatalError("Akel Studio : unable to open the runtime settings file");

		file.unsetf(std::ios::skipws);

		file.seekg(0, std::ios::end);
			std::size_t fileSize = file.tellg();
		file.seekg(0, std::ios::beg);

		std::vector<uint8_t> data;
			data.reserve(fileSize);
			data.insert(data.begin(), std::istream_iterator<uint8_t>(file), std::istream_iterator<uint8_t>());
		file.close();

		_runtime_settings = nlohmann::json::from_msgpack(std::move(data));
	}
	else
	{
		_runtime_settings["projectFilePath"] = path.parent_path().string();
		_runtime_settings["projectFileName"] = path.stem().string();
		_runtime_settings["memManagerEnableFixedAllocator"] = true;
		_runtime_settings["vkEnableMessageValidationLayers"] = false;
		_runtime_settings["useSystemDialogBoxes"] = true;
		_runtime_settings["enableWarningConsoleMessage"] = true;
		_runtime_settings["vkForceDisableValidationLayers"] = false;
		_runtime_settings["useDefaultResourceSystem"] = true;
	}

	Ak::CounterTicks::setTicksGoal(60);
}

void StudioComponent::onAttach()
{
	_lang_eltm = Ak::create_Unique_ptr<Ak::ELTM>();
	_lang_eltm->load(Ak::Core::getMainDirPath() / "resources/texts/langs.eltm");

	if(!Ak::getMainAppProjectFile().keyExists("language"))
		Ak::getMainAppProjectFile().archive()["language"] = _lang_eltm->getText("English");
	if(!Ak::getMainAppProjectFile().keyExists("on_quit_window"))
		Ak::getMainAppProjectFile().archive()["on_quit_window"] = true;
	if(!Ak::getMainAppProjectFile().keyExists("cullmode"))
		Ak::getMainAppProjectFile().archive()["cullmode"] = VK_CULL_MODE_BACK_BIT;
	if(!Ak::getMainAppProjectFile().keyExists("scene_camera_sensy"))
		Ak::getMainAppProjectFile().archive()["scene_camera_sensy"] = 0.7f;

	_eltm->load(std::move(Ak::VFS::resolve(Ak::getMainAppProjectFile().archive()["language"])));

	Ak::WindowComponent* window = Ak::getMainAppComponentStack()->get_component_as<Ak::WindowComponent*>("__window_component");
	if(Ak::getMainAppProjectFile().isFirstTimeRunning())
	{
		window->title = std::move(_eltm->getText("window_title"));
		window->resizable = true;
		window->maximize = true;
		window->fetchSettings();
	}

	_project.initProjFile(true);

	_stack = Ak::create_Unique_ptr<PanelStack>();

	_stack->add_panel<Docks>(_eltm, _project);
	_stack->add_panel<Scene>(_eltm, _project);
	_stack->add_panel<CodeEditor>(_eltm, _project);
	_stack->add_panel<ELTM_editor>(_eltm, _project, &_eltm_editor_input_buffer, &_eltm_editor_save);
	_stack->add_panel<Components>(_eltm, _project);
	_stack->add_panel<Entities>(_eltm, _project);
	_stack->add_panel<RendererManager>(_eltm, _project);
	_stack->add_panel<AudioManager>(_eltm, _project);
	
	Materials* materials = Ak::memAlloc<Materials>(_eltm, _project);
	_stack->add_panel(materials);
	_stack->add_panel<MaterialEditor>(_eltm, _project, materials->getNames());
	_stack->add_panel<EntitiesManager>(_eltm, _project, materials->getNames());

	_stack->add_panel<Browser>(_eltm, _project);
	_stack->add_panel<Console>(_eltm, _project);

	Ak::RendererComponent* renderer = static_cast<Ak::RendererComponent*>(Ak::getMainAppComponentStack()->get_component("__renderer_component"));
	renderer->getClearValue().color.float32[0] = 0.627450980;
	renderer->getClearValue().color.float32[1] = 0.878431373;
	renderer->getClearValue().color.float32[2] = 0.909803922;
	renderer->setMaxFPS(200);

	_logo = AkImGui::LoadImage(Ak::Core::getMainDirPath() / "resources/assets/logo.png");
   // ImGuizmo::SetImGuiContext(ImGui::GetCurrentContext());
}

static bool realquit = false;

void StudioComponent::onImGuiRender()
{
	//ImGuizmo::BeginFrame();
	//ImGuizmo::Enable(true);
	
	static Ak::WindowComponent* window = Ak::getMainAppComponentStack()->get_component_as<Ak::WindowComponent*>("__window_component");

	drawMainMenuBar();

	for(auto elem : _stack->_panels)
		elem->onUpdate(window->size);

	if(_showAbout)
		drawAboutWindow();
	if(_showOpt)
		drawOptionsWindow();

	if(!realquit && !_running)
        ImGui::OpenPopup(_eltm->getText("really").c_str());

	ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if(ImGui::BeginPopupModal(_eltm->getText("really").c_str(), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
	{
		ImGui::Text(_eltm->getText("are_you_sure_quit").c_str());
		if(ImGui::Button(_eltm->getText("yes").c_str(), ImVec2(120, 0)))
		{
			_running = false;
			realquit = true;
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if(ImGui::Button(_eltm->getText("no").c_str(), ImVec2(120, 0)))
		{
			_running = true;
			realquit = false;
			ImGui::CloseCurrentPopup();
		}
		
		ImGui::EndPopup();
	}

	if(_open_build)
		ImGui::OpenPopup(_eltm->getText("buildPopUpTitle").c_str());

	ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(ImVec2(ImGui::CalcTextSize(_eltm->getText("buildPopUpText").c_str()).x + 50, 100), ImGuiCond_Appearing);
	static int timeout = Ak::Maths::randint(2000, 7000);
	if(ImGui::BeginPopupModal(_eltm->getText("buildPopUpTitle").c_str(), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
	{
		float windowWidth = ImGui::GetWindowWidth();
		float textWidth = ImGui::CalcTextSize(_eltm->getText("buildPopUpText").c_str()).x;
		ImGui::SetCursorPosX((windowWidth - textWidth) / 2.0f);
		ImGui::Text(_eltm->getText("buildPopUpText").c_str());
		
		const ImU32 bg = ImGui::GetColorU32(ImGuiCol_WindowBg);
		ImGui::SetCursorPosX((windowWidth - 45) / 2.0f);
		ImSpinner::SpinnerLoadingRing("##buildSpinner", 15, 3, ImSpinner::white, bg);

		if(SDL_GetTicks64() - _build_timer > timeout)
		{
			_open_build = false;
			timeout = Ak::Maths::randint(2000, 7000);
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

void StudioComponent::update()
{
	static Scene* scene = static_cast<Scene*>(_stack->get_panel("__scene"));
	static Components* comps = static_cast<Components*>(_stack->get_panel("__components"));
	if(scene->callRun())
	{
		comps->writeComponents();
		_project.writeFile();
		writeRuntimeSettings();
		std::filesystem::remove(_project.getDir() / "AkelRuntime");
		std::filesystem::copy(Ak::Core::getMainDirPath() / "resources/runtime/AkelRuntime", _project.getDir());
		std::system(std::string(_project.getDir().string() + "/AkelRuntime").c_str());
		std::filesystem::remove(_project.getDir() / "AkelRuntime");
	}
	scene->hangUpRun();
}

void StudioComponent::onImGuiEvent(Ak::Input& input)
{
	_running = _running == true ? !input.isEnded() : _running;
	if(!_running && !Ak::getMainAppProjectFile().archive()["on_quit_window"])
		realquit = true;
	if(realquit)
		input.finish();

	if(input.isEnded() && !realquit)
		input.run();

	for(auto elem : _stack->_panels)
		elem->onEvent(input);
	
	_eltm_editor_input_buffer.clear();
}

void StudioComponent::generateFontTextures(Ak::ImGuiComponent* imgui)
{
	ImGuiIO& io = ImGui::GetIO();
	imgui->addFontFromFile(std::string(Ak::Core::getMainDirPath() / "resources/fonts/opensans/OpenSans-Regular.ttf").c_str(), 18.0f, true);
	static const ImWchar icons_ranges[] = { AKS_ICON_MIN_MD, AKS_ICON_MAX_16_MD, 0 };
	ImFontConfig config;
	config.MergeMode = true;
	config.GlyphOffset.y = 4.0f;

	io.Fonts->AddFontFromFileTTF(std::string(Ak::Core::getMainDirPath() / "resources/fonts/material_icons-regular.ttf").c_str(), 18.0f, &config, icons_ranges);
	io.Fonts->AddFontDefault();
	imgui->generateFonts();
}

void StudioComponent::onQuit()
{
	for(auto elem : _stack->_panels)
		elem->onQuit();
	_stack.reset(nullptr);
	_lang_eltm.reset(nullptr);
	_logo.destroy();
	_project.writeFile();
	writeRuntimeSettings();
}

void StudioComponent::writeRuntimeSettings()
{
	std::filesystem::path runtime_path = _project.getDir() / "settings.akrt";
	std::filesystem::remove(runtime_path);
	std::ofstream runtime_set(std::move(runtime_path),	std::ios::ate | std::ios::binary);
	if(!runtime_set.is_open())
		Ak::FatalError("Akel Studio : unable to create runtime settings file");
	std::vector<uint8_t> data = nlohmann::json::to_msgpack(_runtime_settings);
	for(uint8_t byte : data)
		runtime_set << byte;
}

void StudioComponent::drawMainMenuBar()
{
	static Ak::WindowComponent* window = Ak::getMainAppComponentStack()->get_component_as<Ak::WindowComponent*>("__window_component");
	if(ImGui::BeginMainMenuBar())
	{
		if(ImGui::BeginMenu(std::string(AKS_ICON_MD_FOLDER" " + _eltm->getText("MainMenuBar.file")).c_str()))
		{
			if(ImGui::MenuItem(std::string(AKS_ICON_MD_SAVE" " + _eltm->getText("MainMenuBar.save")).c_str(), "Ctrl+S")) { /* Do stuff */ }
			if(ImGui::MenuItem(std::string(AKS_ICON_MD_CLOSE" " + _eltm->getText("MainMenuBar.quit")).c_str()))
				_running = false;
			ImGui::EndMenu();
		}
		if(ImGui::BeginMenu(std::string(AKS_ICON_MD_TUNE" " + _eltm->getText("MainMenuBar.edit")).c_str()))
		{
			if(ImGui::MenuItem(std::string(AKS_ICON_MD_NOTE_ADD" " + _eltm->getText("MainMenuBar.addFile")).c_str())) { /* Do stuff */ }
			if(ImGui::MenuItem(std::string(AKS_ICON_MD_BUILD_CIRCLE" " + _eltm->getText("MainMenuBar.build")).c_str()))
				buildProject();
			if(ImGui::MenuItem(std::string(AKS_ICON_MD_SETTINGS" " + _eltm->getText("MainMenuBar.options")).c_str()))
				_showOpt = !_showOpt;
			ImGui::EndMenu();
		}
		if(ImGui::BeginMenu(std::string(AKS_ICON_MD_GRID_VIEW" " + _eltm->getText("MainMenuBar.panels")).c_str()))
		{
			if(ImGui::MenuItem(std::string(AKS_ICON_MD_TYPE_SPECIMEN" " + _eltm->getText("ELTM_Editor.name")).c_str()))
				_stack->get_panel("__eltm_editor")->onOpen();
			ImGui::EndMenu();
		}
		if(_stack->get_panel("__eltm_editor")->isOpen() && ImGui::BeginMenu(std::string(AKS_ICON_MD_TYPE_SPECIMEN" " + _eltm->getText("MainMenuBar.eltm_editor")).c_str()))
		{
			if(ImGui::MenuItem(std::string(AKS_ICON_MD_FILE_OPEN" " + _eltm->getText("MainMenuBar.e_load")).c_str()))
			{
				auto file = pfd::open_file(_eltm->getText("MainMenuBar.e_load"), Ak::Core::getMainDirPath(), { "ELTM files (.eltm .tm)", "*.eltm *.tm", "All files", "*"});	
				if(!file.result().empty())
					_eltm_editor_input_buffer = file.result()[0];
			}
			if(ImGui::MenuItem(std::string(AKS_ICON_MD_SAVE" " + _eltm->getText("MainMenuBar.e_save")).c_str()))
				_eltm_editor_save = 1;
			if(ImGui::MenuItem(std::string(AKS_ICON_MD_SAVE_AS" " + _eltm->getText("MainMenuBar.e_save_as")).c_str()))
				_eltm_editor_save = 2;
			ImGui::EndMenu();
		}
		if(ImGui::BeginMenu(std::string(AKS_ICON_MD_HELP" " + _eltm->getText("MainMenuBar.help")).c_str()))
		{
			if(ImGui::MenuItem(std::string(AKS_ICON_MD_INFO" " + _eltm->getText("MainMenuBar.about")).c_str()))
				_showAbout = !_showAbout;
			ImGui::EndMenu();
		}
		ImGui::SameLine(window->size.X - 100);
		Ak::RendererComponent* renderer = static_cast<Ak::RendererComponent*>(Ak::getMainAppComponentStack()->get_component("__renderer_component"));
		ImGui::Text("%d FPS", renderer->getFPS());

		ImGui::EndMainMenuBar();
	}
}

void StudioComponent::buildProject()
{
	Components* comps = static_cast<Components*>(_stack->get_panel("__components"));
	comps->writeComponents();
	_open_build = true;
	_project.writeFile();
	writeRuntimeSettings();
	std::filesystem::remove(_project.getDir() / "AkelRuntime");
	std::filesystem::remove(_project.getDir() / _runtime_settings["projectFileName"]);
	std::filesystem::copy(Ak::Core::getMainDirPath() / "resources/runtime/AkelRuntime", _project.getDir());
	std::filesystem::rename(_project.getDir() / "AkelRuntime", _project.getDir() / _runtime_settings["projectFileName"]);
	_build_timer = SDL_GetTicks64();
}

void StudioComponent::drawAboutWindow()
{
	static Ak::WindowComponent* window = Ak::getMainAppComponentStack()->get_component_as<Ak::WindowComponent*>("__window_component");
	if(ImGui::Begin(std::string(AKS_ICON_MD_INFO" " + _eltm->getText("MainMenuBar.about")).data(), &_showAbout, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse))
	{
		ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() / 8, ImGui::GetWindowHeight() / 8));
		ImGui::Image(_logo.getImGuiID(), ImVec2(ImGui::GetWindowWidth() / 3, ImGui::GetWindowWidth() / 3));

		ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() - ImGui::CalcTextSize(_eltm->getText("MainMenuBar.version").data()).x - 30, ImGui::GetWindowHeight() - ImGui::GetTextLineHeightWithSpacing() * 3));
		ImGui::TextUnformatted(_eltm->getText("MainMenuBar.version").data());

#ifdef AK_STUDIO_DEBUG
		ImGui::TextUnformatted("Debug version");
#else
		ImGui::TextUnformatted("Release version");
#endif
		ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() / 1.75f, window->size.Y - (window->size.Y / 1.2f)));
		ImGui::TextWrapped(_eltm->getText("MainMenuBar.about_text").data());

		AkImGui::WebLink(_eltm->getText("MainMenuBar.license").data(), "https://akel-engine.com/license/", ImVec2(ImGui::GetWindowWidth()/2 - ImGui::CalcTextSize(_eltm->getText("MainMenuBar.website").data()).x * 2 - ImGui::CalcTextSize(_eltm->getText("MainMenuBar.license").data()).x, ImGui::GetWindowHeight() - ImGui::GetWindowHeight()/4));
		AkImGui::WebLink(_eltm->getText("MainMenuBar.website").data(), "https://akel-engine.com/", ImVec2(ImGui::GetWindowWidth()/2 - ImGui::CalcTextSize(_eltm->getText("MainMenuBar.website").data()).x/2, ImGui::GetWindowHeight() - ImGui::GetWindowHeight()/4));
		AkImGui::WebLink(_eltm->getText("MainMenuBar.code").data(), "https://github.com/Spinwaves/Akel", ImVec2(ImGui::GetWindowWidth()/2 + ImGui::CalcTextSize(_eltm->getText("MainMenuBar.website").data()).x * 2, ImGui::GetWindowHeight() - ImGui::GetWindowHeight()/4));

		ImGui::End();
	}
}

extern bool reload_docks;

void StudioComponent::draw_general_settings()
{
	ImGui::Text(std::string(AKS_ICON_MD_LANGUAGE" " + _eltm->getText("Settings.language")).data());
	ImGui::SameLine();
	static std::string selected = _eltm->getText("lang_name");
	if(ImGui::BeginCombo("##combo", selected.c_str()))
	{
		for(auto&& [lang, path] : _lang_eltm->getTexts())
		{
			if(ImGui::Selectable(lang.c_str(), selected == lang))
			{
				if(!_eltm->reload(Ak::Core::getMainDirPath() / path))
					Ak::Core::log::report(FATAL_ERROR, "unable to change language");
				Ak::getMainAppProjectFile().archive()["language"] = Ak::Core::getMainDirPath() / path;
				selected = lang;
				reload_docks = true;
			}
		}

		ImGui::EndCombo();
	}

	ImGui::Separator();

	bool on_quit_window = Ak::getMainAppProjectFile().archive()["on_quit_window"];
	ImGui::Checkbox(_eltm->getText("Settings.onQuit").c_str(), &on_quit_window);
	if(on_quit_window != Ak::getMainAppProjectFile().archive()["on_quit_window"])
		Ak::getMainAppProjectFile().archive()["on_quit_window"] = on_quit_window;
}

void StudioComponent::draw_scene_settings()
{
	ImGui::Text(std::string(AKS_ICON_MD_VIDEOCAM" " + _eltm->getText("Settings.camera")).data());

	ImGui::Separator();

	ImGui::Text(_eltm->getText("Settings.sensitivity").data());
	ImGui::SameLine(0);
	static float sensy = Ak::getMainAppProjectFile().archive()["scene_camera_sensy"];
	ImGui::SliderFloat("##slider_camera_sensy", &sensy, 0.1f, 2.0f, "%.1f");
	if(sensy != Ak::getMainAppProjectFile().archive()["scene_camera_sensy"])
		Ak::getMainAppProjectFile().archive()["scene_camera_sensy"] = sensy;
}

void StudioComponent::draw_project_settings()
{
	static std::array<bool, 6> opts = {
		_runtime_settings["memManagerEnableFixedAllocator"],
		_runtime_settings["vkEnableMessageValidationLayers"],
		_runtime_settings["useSystemDialogBoxes"],
		_runtime_settings["enableWarningConsoleMessage"],
		_runtime_settings["vkForceDisableValidationLayers"],
		_runtime_settings["useDefaultResourceSystem"]
	};

	ImGui::Checkbox(std::string(AKS_ICON_MD_STORAGE" " + _eltm->getText("Settings.mem_man_en_fixed")).c_str(), &opts[0]);
	ImGui::Checkbox(std::string(AKS_ICON_MD_TERMINAL" " + _eltm->getText("Settings.vk_en_msg_layers")).c_str(), &opts[1]);
	ImGui::Checkbox(std::string(AKS_ICON_MD_DOMAIN_VERIFICATION" " + _eltm->getText("Settings.use_sys_dial_boxes")).c_str(), &opts[2]);
	ImGui::Checkbox(std::string(AKS_ICON_MD_LAYERS_CLEAR" " + _eltm->getText("Settings.vk_force_dis_layers")).c_str(), &opts[3]);
	ImGui::Checkbox(std::string(AKS_ICON_MD_FOLDER_COPY" " + _eltm->getText("Settings.use_def_res_sys")).c_str(), &opts[4]);
	ImGui::Checkbox(std::string(AKS_ICON_MD_DATA_OBJECT" " + _eltm->getText("Settings.proj_file_raw_json")).c_str(), &opts[5]);

	_runtime_settings["memManagerEnableFixedAllocator"] = opts[0];
	_runtime_settings["vkEnableMessageValidationLayers"] = opts[1];
	_runtime_settings["useSystemDialogBoxes"] = opts[2];
	_runtime_settings["enableWarningConsoleMessage"] = opts[3];
	_runtime_settings["vkForceDisableValidationLayers"] = opts[4];
	_runtime_settings["useDefaultResourceSystem"] = opts[5];
}

void StudioComponent::drawOptionsWindow()
{
	if(ImGui::Begin(std::string(AKS_ICON_MD_SETTINGS" " + _eltm->getText("MainMenuBar.options")).data(), &_showOpt, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
	{
		static int selected = -1;
		if(ImGui::BeginChild("Panel", ImVec2(200, 0), true))
		{
			if(ImGui::Selectable(std::string(AKS_ICON_MD_TUNE" " + _eltm->getText("Settings.general")).data(), selected == 0))
				selected = 0;
			if(ImGui::Selectable(std::string(AKS_ICON_MD_PANORAMA" " + _eltm->getText("Settings.scene")).data(), selected == 1))
				selected = 1;
			if(ImGui::Selectable(std::string(AKS_ICON_MD_SOURCE" " + _eltm->getText("Settings.project")).data(), selected == 2))
				selected = 2;
			ImGui::EndChild();
		}
		ImGui::SameLine(0);
		if(ImGui::BeginChild("Choices", ImVec2(0, 0), true))
		{
			switch(selected)
			{
				case 0: draw_general_settings(); break;
				case 1: draw_scene_settings(); break;
				case 2: draw_project_settings(); break;
				default : break;
			}
			ImGui::EndChild();
		}
		ImGui::End();
	}	
}
