#include "pch.h"
#include "PrefabUI.h"

#include <Engine/CGameObject.h>
#include <Engine/CPrefab.h>
#include <Engine/CTransform.h>

#include <Engine/CResMgr.h>
#include <Engine/CMeshRender.h>
#include <Engine/CMesh.h>
#include <Engine/CMaterial.h>
#include <Engine/CScript.h>

#include <Engine/CCollider2D.h>

#include <Engine/CAnimator2D.h>
#include <Engine/CCamera.h>

#include <Script/CScriptMgr.h>
#include "CImGuiMgr.h"
#include "ListUI.h"




PrefabUI::PrefabUI()
	: ResInfoUI("Prefab", RES_TYPE::PREFAB)
	, m_pObj(nullptr)
{
}

PrefabUI::~PrefabUI()
{
}

void PrefabUI::update()
{
	// To Do

	ResInfoUI::update();
}

void PrefabUI::render_update()
{
	ResInfoUI::render_update();


	CPrefab* pPrefab = dynamic_cast<CPrefab*>(GetTargetRes());
	m_pObj = pPrefab->GetProto();



	if(nullptr != m_pObj->Transform())
	AboutTransform(m_pObj);

	if (nullptr != m_pObj->MeshRender())
	AboutMtrl(m_pObj);

	if (nullptr != m_pObj->Collider2D())
	AboutCollider2D(m_pObj);

	if (nullptr != m_pObj->Animator2D())
	AboutAnimation2D(m_pObj);

	AboutScript(m_pObj);
}

void PrefabUI::AboutTransform(CGameObject* _Obj)
{
	CTransform* pTrans = _Obj->Transform();
	Vec3 vPos = pTrans->GetRelativePos();
	Vec3 vScale = pTrans->GetRelativeScale();
	Vec3 vRot = pTrans->GetRelativeRotation();
	vRot.ToDegree();


	ImGui::PushItemWidth(200); // Float3 위젯 간격 설정

	ImGui::Text("Relative Position");
	ImGui::SameLine();
	ImGui::InputFloat3("##Position", vPos,0,ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);
	

	ImGui::Text("Relative Scale   ");
	ImGui::SameLine();
	ImGui::InputFloat3("##Scale", vScale, 0, ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);
	

	ImGui::Text("Relative Rotation");
	ImGui::SameLine();
	ImGui::InputFloat3("##Rotation", vRot, 0, ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);
	vRot.ToRadian();
	

	ImGui::NewLine();
	ImGui::Separator();
}

void PrefabUI::AboutMtrl(CGameObject* _Obj)
{

	CMeshRender* pMeshRender = _Obj->MeshRender();
	CRenderComponent* pRender; _Obj->GetRenderComponent();
	Ptr<CMesh> pMesh = pMeshRender->GetMesh();
	Ptr<CMaterial> pMtrl = pMeshRender->GetSharedMaterial();
	
	

	wstring wMtrlName = pMtrl->GetShader()->GetKey();;
	wstring wMeshName = pMesh->GetKey();

	
	//if (nullptr != pMtrl)
	//	wMtrlName = pMtrl->GetName();
	//else
	//	wMtrlName = L"";

	string MtrlName =string(wMtrlName.begin(),wMtrlName.end());
	string MeshName = string(wMeshName.begin(),wMeshName.end()); 




	
	ImGui::Text("MeshName");
	ImGui::SameLine();
	ImGui::InputText("##Mesh", (char*)MeshName.c_str(), 50, ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);
	
	
	ImGui::Text("MtrlName");
	ImGui::SameLine();
	ImGui::InputText("##Mtrl", (char*)MtrlName.c_str(), 50, ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

	ImGui::NewLine();
	ImGui::Separator();

}

void PrefabUI::AboutCollider2D(CGameObject* _Obj)
{

	CCollider2D* Col = _Obj->Collider2D();

	if (nullptr != Col) {

		float OffsetPos[2] = { Col->GetOffsetPos().x,Col->GetOffsetPos().y };
		float OffsetScale[2] = { Col->GetOffsetScale().x,Col->GetOffsetScale().y };

		ImGui::Text("Collider Type");
		ImGui::SameLine(120);

		COLLIDER2D_TYPE	eType = Col->GetCollider2DType();
		string ColliderType = {};
		if (COLLIDER2D_TYPE::BOX == eType)
			ColliderType = "BOX";
		else
			ColliderType = "CIRCLE";
		ImGui::InputText("##type",(char*)ColliderType.c_str(),ColliderType.size(), ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);
		

		ImGui::Text("OffsetPos");
		ImGui::SameLine(120);
		ImGui::InputFloat2("##OffsetPos", OffsetPos, "%.f", ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);
		


		ImGui::Text("OffsetScale");
		ImGui::SameLine(120);
		ImGui::InputFloat2("##OffsetScale", OffsetScale, "%.f", ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);
		
	}

	ImGui::NewLine();
	ImGui::Separator();
}

void PrefabUI::AboutAnimation2D(CGameObject* _Obj)
{

	CAnimator2D* pAnim = _Obj->Animator2D();

	if (nullptr != pAnim) {
		if (ImGui::Button("AnimationList"))
		{
			// ListUI 활성화한다.
			const map<wstring, CAnimation2D*>& mapAnim = pAnim->GetAnimList();
			ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##ListUI");
			pListUI->Clear();
			pListUI->SetTitle("Animation List");

			for (const auto& pair : mapAnim)
			{
				pListUI->AddList(string(pair.first.begin(), pair.first.end()));
			}

			pListUI->Activate();
		}
	}

	ImGui::NewLine();
	ImGui::Separator();
}

void PrefabUI::AboutScript(CGameObject* _Obj)
{
	vector<CScript*> vScirpt = m_pObj->GetScripts();

	if (vScirpt.size() == 0)
		return;


	vector<string> vScriptMap;

	static int idx = 0;

	for (int i = 0; i < vScirpt.size(); ++i)
	{
		wstring wstr = CScriptMgr::GetScriptName(vScirpt[i]);
		string str = string(wstr.begin(),wstr.end());
		vScriptMap.push_back(str);
	}

	const char* combo_preview_value = vScriptMap[idx].c_str();

	if (ImGui::BeginCombo("##PreviewCombo", combo_preview_value))
	{
		for (int n = 0; n < vScriptMap.size(); n++)
		{
			const bool is_selected = (idx == n);
			if (ImGui::Selectable(vScriptMap[n].c_str(), is_selected))
			{
				idx = n;
			}
		}
		ImGui::EndCombo();
	}
}


