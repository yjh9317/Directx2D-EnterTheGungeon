#include "pch.h"
#include "GraphicsShaderUI.h"

#include <Engine/CGraphicsShader.h>

GraphicsShaderUI::GraphicsShaderUI()
	: ResInfoUI("GraphicsShader", RES_TYPE::GRAPHICS_SHADER)
{
}

GraphicsShaderUI::~GraphicsShaderUI()
{
}


void GraphicsShaderUI::update()
{



	ResInfoUI::update();
}

void GraphicsShaderUI::render_update()
{
	ResInfoUI::render_update();

	CGraphicsShader* pGraphicsShader = dynamic_cast<CGraphicsShader*>(GetTargetRes());

	const char* RS_TYPE[4] = { "CULL_BACK","CULL_FRONT","CULL_NONE","WIRE_FRAME" };
	const char* DS_TYPE[7] = { "LESS","LESS_EQUAL","GREATER","GREATER_EQUAL","NO_TEST","NO_WRITE","NO_TEST_NO_WRITE" };
	const char* BS_TYPE[2] = { "DEFAULT","ALPHA_BLEND" };
	const char* SHADER_DOMAIN[4] = { "DOMAIN_FORWARD","DOMAIN_MASKED","DOMAIN_TRANSLUCENT","DOMAIN_POSTPROCESS" };




	string strShaderName;
	if (nullptr != pGraphicsShader)
		strShaderName = string(pGraphicsShader->GetKey().begin(), pGraphicsShader->GetKey().end());


	ImGui::Text("Shader");
	ImGui::SameLine(100);
	ImGui::InputText("##ShaderName", (char*)strShaderName.c_str(), strShaderName.capacity(), ImGuiInputTextFlags_ReadOnly);

	char* tmp = (char*)RS_TYPE[(int)pGraphicsShader->GetRSType()];

	ImGui::Text("RSType");
	ImGui::SameLine(100);
	ImGui::InputText("##RSType", tmp, 20, ImGuiInputTextFlags_ReadOnly);

	tmp = (char*)DS_TYPE[(int)pGraphicsShader->GetDSType()];

	ImGui::Text("DSType");
	ImGui::SameLine(100);
	ImGui::InputText("##DSType", tmp, 20, ImGuiInputTextFlags_ReadOnly);

	tmp = (char*)BS_TYPE[(int)pGraphicsShader->GetBSType()];

	ImGui::Text("BSType");
	ImGui::SameLine(100);
	ImGui::InputText("##BSType", tmp, 20, ImGuiInputTextFlags_ReadOnly);

	tmp = (char*)SHADER_DOMAIN[(int)pGraphicsShader->GetShaderDomain()];

	ImGui::Text("ShaderDomain");
	ImGui::SameLine(100);
	ImGui::InputText("##ShaderDomain", tmp, 20, ImGuiInputTextFlags_ReadOnly);

	//pGraphicsShader->GetTopology();
}
