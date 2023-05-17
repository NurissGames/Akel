// This file is a part of Akel Studio
// Authors : @kbz_8
// Created : 10/03/2022
// Updated : 17/05/2023

#include <Panels/entities.h>
#include <Fonts/material_font.h>

Entities::Entities(std::shared_ptr<Ak::ELTM> eltm, Ak::Core::ProjectFile& project) : Panel("__entities", project)
{
    _eltm = std::move(eltm);
}

void Entities::onUpdate(Ak::Maths::Vec2<int>& size)
{
	_width = (15 * size.X)/100;
    _height = size.Y - 25 - (50 * size.Y)/100;
	if(ImGui::Begin(std::string(AKS_ICON_MD_VIEW_IN_AR" " + _eltm->getText("Entities.name")).c_str(), nullptr, ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
    {
		ImGui::SetWindowPos(ImVec2(0, 25));
		ImGui::SetWindowSize(ImVec2(_width, _height));

		ImGui::End();
    }
}
