#include "pch.h"
#include "ParticleUI.h"

#include <Engine/CParticleSystem.h>

ParticleUI::ParticleUI()
	: ComponentUI("Particle", COMPONENT_TYPE::PARTICLESYSTEM)
{
	SetSize(Vec2(0.f, 420.f));
}

ParticleUI::~ParticleUI()
{
}

void ParticleUI::update()
{
}

void ParticleUI::render_update()
{
	ComponentUI::render_update();

	CGameObject* pTargetObject = GetTargetObject();
	CParticleSystem* pParticle = pTargetObject->ParticleSystem();

	ImGui::PushItemWidth(200); // Float3 위젯 간격 설정


	//AliveCount
	int AliveCount = pParticle->GetAliveCount();
	ImGui::Text("AliveCount");
	ImGui::SameLine(100);
	ImGui::InputInt("##AliveCount", &AliveCount);
	pParticle->SetAliveCount(AliveCount);

	//MaxCount
	int MaxCount = (int)pParticle->GetMaxCount();
	ImGui::Text("MaxCount");
	ImGui::SameLine(100);
	ImGui::InputInt("##MaxCount", &MaxCount);
	pParticle->SetMaxCount(MaxCount);

	//CreateDistance
	float CreateDistance = pParticle->GetCreateDis();
	ImGui::Text("CreateDistance");
	ImGui::SameLine(100);
	ImGui::InputFloat("##CreateDistance", &CreateDistance);
	pParticle->SetCreateDis(CreateDistance);

	//CreateTerm
	float CreateTerm = pParticle->GetCreateTerm();
	ImGui::Text("CreateTerm");
	ImGui::SameLine(100);
	ImGui::InputFloat("##CreateTerm", &CreateTerm);
	pParticle->SetCreateTerm(CreateTerm);

	float MinLife = pParticle->GetMinLife();
	ImGui::Text("MinLife");
	ImGui::SameLine(100);
	ImGui::InputFloat("##MinLife", &MinLife);
	pParticle->SetMinLife(MinLife);

	float MaxLife = pParticle->GetMaxLife();
	ImGui::Text("MaxLife");
	ImGui::SameLine(100);
	ImGui::InputFloat("##MaxLife", &MaxLife);
	pParticle->SetMaxLife(MaxLife);

	float StartScale[3] = { pParticle->GetStartScale().x,pParticle->GetStartScale().y,pParticle->GetStartScale().z };
	ImGui::Text("StartScale");
	ImGui::SameLine(100);
	ImGui::InputFloat3("##StartScale", StartScale);
	pParticle->SetStartScale((Vec3)StartScale);

	float EndScale[3] = { pParticle->GetEndScale().x,pParticle->GetEndScale().y,pParticle->GetEndScale().z };
	ImGui::Text("EndScale");
	ImGui::SameLine(100);
	ImGui::InputFloat3("##EndScale", EndScale);
	pParticle->SetEndScale(Vec3(EndScale));

	ImGui::NewLine();

	ImGui::SetCursorPos(ImVec2(320.f, 20.f));
	float Start_Color[4] = { pParticle->GetStartColor()[0],pParticle->GetStartColor()[1],pParticle->GetStartColor()[2],pParticle->GetStartColor()[3] };
	ImGui::Text("StartColor");
	ImGui::SameLine(400);
	ImGui::InputFloat4("##StartColor", Start_Color);
	pParticle->SetStartColor(Vec4(Start_Color));

	ImGui::SetCursorPos(ImVec2(320.f, 40.f));
	float End_Color[4] = { pParticle->GetEndColor()[0],pParticle->GetEndColor()[1],pParticle->GetEndColor()[2],pParticle->GetEndColor()[3] };
	ImGui::Text("EndColor");
	ImGui::SameLine(400);
	ImGui::InputFloat4("##EndColor", End_Color);
	pParticle->SetEndColor(Vec4(End_Color));

	static int CheckColor = 0;

	static int curidx = 0;
	const char* Items[2] = { "StartColor","EndColor" };

	ImGui::SetCursorPos(ImVec2(320.f, 65.f));
	ImGui::Text("ColorList");

	ImGui::SetCursorPos(ImVec2(400.f, 65.f));
	if (ImGui::BeginListBox("##ColorList", ImVec2(150.f, 38.f)))
	{
		for (int n = 0; n < 2; n++)
		{
			const bool is_selected = (curidx == n);
			if (ImGui::Selectable(Items[n], is_selected))
			{
				curidx = n;
				CheckColor = n;
			}

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndListBox();
	}


	ImGui::SetCursorPos(ImVec2(320, 120.f));
	static ImVec4 color = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);

	static bool hdr = false;

	ImGuiColorEditFlags misc_flags = (hdr ? ImGuiColorEditFlags_HDR : 0);

	static bool alpha = true;
	static bool alpha_bar = true;
	static bool ref_color = false;
	static ImVec4 ref_color_v(1.0f, 0.0f, 1.0f, 0.5f);
	static int display_mode = 0;

	ImGuiColorEditFlags flags = misc_flags;
	if (!alpha)            flags |= ImGuiColorEditFlags_NoAlpha;
	if (alpha_bar)         flags |= ImGuiColorEditFlags_AlphaBar;



	ImGui::ColorPicker4("MyColor##4", (float*)&color, flags, ref_color ? &ref_color_v.x : NULL);

	if (CheckColor == 0) {
		pParticle->SetStartColor(Vec4(color.x, color.y, color.z, color.w));
		pParticle->SetEndColor(Vec4(End_Color));
	}
	else if (CheckColor == 1)
	{
		pParticle->SetStartColor(Vec4(Start_Color));
		pParticle->SetEndColor(Vec4(color.x, color.y, color.z, color.w));
	}


}


