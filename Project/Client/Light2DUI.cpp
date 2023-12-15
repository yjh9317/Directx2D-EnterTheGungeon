#include "pch.h"
#include "Light2DUI.h"

#include <Engine/CLight2D.h>
#include <Engine/CTransform.h>


Light2DUI::Light2DUI()
	:ComponentUI("Light2D", COMPONENT_TYPE::LIGHT2D)
{
	SetSize(Vec2(0.f, 150.f));
}

Light2DUI::~Light2DUI()
{
}


void Light2DUI::update()
{
}

void Light2DUI::render_update()
{
	ComponentUI::render_update();

	CGameObject* pTargetObj = GetTargetObject();
	if (nullptr == pTargetObj)
		return;


	tLightInfo Info = pTargetObj->Light2D()->GetLightInfo();

	float angle = Info.fAngle;
	float range = Info.fRange;
	
	int type = Info.iLightType;
	const char* arr[3] = { "DIRECTIONAL","POINT","SPOT" };

	float Dir[3] = { Info.vLightDir.x,Info.vLightDir.y,Info.vLightDir.z };
	float Color[3] = { Info.color.vDiff.x,Info.color.vDiff.y,Info.color.vDiff.z };

	ImGui::Text("Angle");
	ImGui::SameLine(80);
	ImGui::InputFloat("##Angle", &angle, 1);
	if (angle > 360.f) angle = 360.f;
	if (angle < 0.f) angle = 0.f;
	pTargetObj->Light2D()->SetAngle(angle);

	ImGui::Text("Range");
	ImGui::SameLine(80);
	ImGui::InputFloat("##Range", &range, 1);
	pTargetObj->Light2D()->SetRange(range);

	ImGui::Text("LightDir");
	ImGui::SameLine(80);

	ImGui::SliderFloat3("##LightDir", Dir, -1.0f, 1.0f);
	pTargetObj->Light2D()->SetLightDir(Vec3(Dir[0], Dir[1], Dir[2]));


	ImGui::Text("LightColor");
	ImGui::SameLine(80);
	ImGui::ColorEdit3("##color", Color);
	pTargetObj->Light2D()->SetDiffuse(Vec3(Color[0], Color[1], Color[2]));

	ImGui::Text("LightType");
	ImGui::SameLine(80);
	ImGui::Combo("##LightType", &type, arr, 3);
	pTargetObj->Light2D()->SetLightType((LIGHT_TYPE)type);

}


