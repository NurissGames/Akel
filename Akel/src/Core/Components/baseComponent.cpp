// This file is a part of Akel
// Authors : @kbz_8
// Created : 23/06/2021
// Updated : 16/05/2023

#include <Core/Components/baseComponent.h>
#include <Core/projectFile.h>

namespace Ak
{
	Component::Component(std::string name) : _name(std::move(name))
	{
		getMainAppProjectFile().archive()[_name] = true;
	}

	void Component::onAttach() {}
	void Component::update() {}
	void Component::onRender() {}
	void Component::onImGuiRender() {}
	void Component::onImGuiEvent(Input& input) {}
	void Component::onEvent(Input& input) {}
	void Component::onQuit() {}

	Component::~Component() {}
}

