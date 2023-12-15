#include "pch.h"
#include "CRoomEvent_2.h"
#include <Engine/CEventMgr.h>

#include <Engine/CEventMgr.h>
#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>
#include <Engine/CLayer.h>
#include <Engine/CAnimator2D.h>
#include <Engine/CAnimation2D.h>

#include "CDoorScript.h"


CRoomEvent_2::CRoomEvent_2()
	:CScript((int)SCRIPT_TYPE::ROOMEVENT_2)
	, m_Start(false)
	,m_b(false)
{
}

CRoomEvent_2::~CRoomEvent_2()
{
}


void CRoomEvent_2::update()
{
	if (m_b)
	{
		for (int i = 0; i < vObj.size(); ++i)
		{
			if (vObj[i]->IsDead())
				vObj.erase(vObj.begin() + i);
		}

		if (vObj.size() == 0)
		{
			CScene* pScene = CSceneMgr::GetInst()->GetCurScene();
			CLayer* pLayer = pScene->GetLayer(1);
			vector<CGameObject*> vObj = pLayer->GetRootObjects();

			for (int i = 0; i < vObj.size(); ++i)
			{
				if (vObj[i]->GetName() == L"Door_3")
				{
					CDoorScript* pScript = vObj[i]->GetScript<CDoorScript>();
					if (nullptr == pScript)
						return;

					vObj[i]->Animator2D()->FindAnim(L"Door_Open")->Reset();
					Ptr<CSound> pSound = CResMgr::GetInst()->Load<CSound>(L"sound\\item\\DoorOpen.wav", L"sound\\item\\DoorOpen.wav");
					pSound->Play(1, 0.2f, true);
					pScript->SetDoorType(DOOR_TYPE::Open);
				}
			}

			m_b = false;
		}
	}
}

void CRoomEvent_2::OnCollisionEnter(CGameObject* _pOtherObj)
{
	if (!m_Start && _pOtherObj->GetName() == L"Player")
	{
		if (!m_Start && _pOtherObj->GetName() == L"Player")
		{
			m_Start = true;
			Ptr<CPrefab> pPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Kin.pref");
			CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
			tEventInfo evninfo;

			CGameObject* pObj = pPrefab->Instantiate();

			pObj->Transform()->SetRelativePos(Vec3(1100, 900.f, 0.f));
			pObj->GetRenderComponent()->SetSharedMaterial(pObj->GetRenderComponent()->GetDynamicMaterial());

			evninfo.eType = EVENT_TYPE::CREATE_OBJ;
			evninfo.lParam = (DWORD_PTR)pObj;
			evninfo.wParam = 5;

			CEventMgr::GetInst()->AddEvent(evninfo);
			vObj.push_back(pObj);

			pObj = pPrefab->Instantiate();

			pObj->Transform()->SetRelativePos(Vec3(1000.f, 750.f, 0.f));
			pObj->GetRenderComponent()->SetSharedMaterial(pObj->GetRenderComponent()->GetDynamicMaterial());

			evninfo.eType = EVENT_TYPE::CREATE_OBJ;
			evninfo.lParam = (DWORD_PTR)pObj;
			evninfo.wParam = 5;

			CEventMgr::GetInst()->AddEvent(evninfo);
			vObj.push_back(pObj);

			pPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Bookllets.pref");

			pObj = pPrefab->Instantiate();

			pObj->Transform()->SetRelativePos(Vec3(1650.f, 800.f, 0.f));
			pObj->GetRenderComponent()->SetSharedMaterial(pObj->GetRenderComponent()->GetDynamicMaterial());

			evninfo.eType = EVENT_TYPE::CREATE_OBJ;
			evninfo.lParam = (DWORD_PTR)pObj;
			evninfo.wParam = 5;

			CEventMgr::GetInst()->AddEvent(evninfo);
			vObj.push_back(pObj);

			CScene* pScene = CSceneMgr::GetInst()->GetCurScene();
			CLayer* pLayer = pScene->GetLayer(1);
			vector<CGameObject*> vObj = pLayer->GetRootObjects();

			for (int i = 0; i < vObj.size(); ++i)
			{
				if (vObj[i]->GetName() == L"Door_3")
				{
					CDoorScript* pScript = vObj[i]->GetScript<CDoorScript>();
					if (nullptr == pScript)
						return;

					pScript->SetDoorType(DOOR_TYPE::Close);
					m_b = true;
				}
			}
		}
	}
}