#include "pch.h"
#include "ComponentUI.h"

#include <Engine/CGameObject.h>
#include <Engine/CComponent.h>

ComponentUI::ComponentUI(const string& _strName, COMPONENT_TYPE _eComType)
	: UI(_strName)
	, m_eComType(_eComType)
	, m_bActive(false)
{
}

ComponentUI::~ComponentUI()
{
}

void ComponentUI::update()
{
	if (nullptr == m_pTargetObject)
		return;

	CComponent* pComponent = m_pTargetObject->GetComponent(m_eComType);
	m_bActive = pComponent->IsActive();

}

void ComponentUI::render_update()
{	
	// ��� Component �̸�
	ImGui::PushID(0);
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f, 0.8f, 0.8f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.f, 0.8f, 0.8f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.f, 0.8f, 0.8f));
	ImGui::Button(ToString(m_eComType));
	ImGui::PopStyleColor(3);
	ImGui::PopID();

	// Component Ȱ��ȭ üũ
	CComponent* pComponent = m_pTargetObject->GetComponent(m_eComType);	

	if (pComponent->GetType() != COMPONENT_TYPE::TRANSFORM)
	{
		bool IsActive = pComponent->IsActive();
		ImGui::SameLine();
		ImGui::Checkbox("##ComponentActive", &IsActive);

		if (pComponent->IsActive() != IsActive)
		{
			if (IsActive)
				pComponent->Activate();			
			else
				pComponent->Deactivate();			
		}
	}
}
