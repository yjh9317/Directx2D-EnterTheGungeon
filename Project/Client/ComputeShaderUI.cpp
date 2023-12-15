#include "pch.h"
#include "ComputeShaderUI.h"

#include <Engine/CComputeShader.h>

ComputeShaderUI::ComputeShaderUI()
	:ResInfoUI("ComputeShaderUI", RES_TYPE::COMPUTE_SHADER)
{
}

ComputeShaderUI::~ComputeShaderUI()
{
}


void ComputeShaderUI::update()
{
	ResInfoUI::update();
}

void ComputeShaderUI::render_update()
{
	ResInfoUI::render_update();


	CComputeShader* pCS = dynamic_cast<CComputeShader*>(GetTargetRes());


	int a = (int)pCS->GetGroupX();

	ImGui::Text("GroupX");
	ImGui::SameLine();
	ImGui::InputInt("##GroupX", &a, 0, 0, ImGuiInputTextFlags_ReadOnly);

	int b = (int)pCS->GetGroupY();

	ImGui::Text("GroupY");
	ImGui::SameLine();
	ImGui::InputInt("##GroupY", &b, 0, 0, ImGuiInputTextFlags_ReadOnly);

	int c = (int)pCS->GetGroupZ();

	ImGui::Text("GroupZ");
	ImGui::SameLine();
	ImGui::InputInt("##GroupZ", &c, 0, 0, ImGuiInputTextFlags_ReadOnly);
}

