#include "pch.h"
#include "Collider2DUI.h"

#include <Engine/CCollider2D.h>
#include <Engine/CMeshRender.h>

Collider2DUI::Collider2DUI()
	: ComponentUI("Collider2D", COMPONENT_TYPE::COLLIDER2D)
{
	SetSize(Vec2(0.f, 120.f));
}

Collider2DUI::~Collider2DUI()
{
}


void Collider2DUI::update()
{
}

void Collider2DUI::render_update()
{
	ComponentUI::render_update();

	CGameObject* pTargetObject = GetTargetObject();
	CCollider2D* Col = pTargetObject->Collider2D();

	float OffsetPos[2] = { Col->GetOffsetPos().x,Col->GetOffsetPos().y };
	float OffsetScale[2] = { Col->GetOffsetScale().x,Col->GetOffsetScale().y };

	ImGui::PushItemWidth(200); // Float3 위젯 간격 설정

	ImGui::Text("Projection Type");
	ImGui::SameLine(120);

	COLLIDER2D_TYPE eType = Col->GetCollider2DType();

	const char* szColType[2] = { "BOX", "CIRCLE" };

	if (ImGui::BeginCombo("##ColType", szColType[(int)eType]))
	{
		for (int i = 0; i < 2; i++)
		{
			if (ImGui::Selectable(szColType[i]))
			{
				Col->SetCollider2DType((COLLIDER2D_TYPE)i);
			}
		}

		ImGui::EndCombo();
	}
	

	ImGui::Text("OffsetPos");
	ImGui::SameLine(120);
	ImGui::InputFloat2("##OffsetPos", OffsetPos);
	Col->SetOffsetPos(Vec2(OffsetPos[0], OffsetPos[1]));


	ImGui::Text("OffsetScale");
	ImGui::SameLine(120);
	ImGui::InputFloat2("##OffsetScale", OffsetScale);
	Col->SetOffsetScale(Vec2(OffsetScale[0], OffsetScale[1]));


}

