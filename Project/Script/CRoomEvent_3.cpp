#include "pch.h"
#include "CRoomEvent_3.h"
#include <Engine/CEventMgr.h>
#include <Engine/CAnimator2D.h>
#include <Engine/CAnimation2D.h>


#include "CDoorScript.h"

CRoomEvent_3::CRoomEvent_3()
	:CScript((int)SCRIPT_TYPE::ROOMEVENT_3)
	,m_Start(false)
	,m_b(false)
{
}

CRoomEvent_3::~CRoomEvent_3()
{
}


void CRoomEvent_3::update()
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
				if (vObj[i]->GetName() == L"Door_7")
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

void CRoomEvent_3::OnCollisionEnter(CGameObject* _pOtherObj)
{
	if (!m_Start && _pOtherObj->GetName() == L"Player")
	{
		if (!m_Start && _pOtherObj->GetName() == L"Player")
		{
			m_Start = true;
			Ptr<CPrefab> pPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Gigi.pref");
			CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
			tEventInfo evninfo;

			CGameObject* pObj = pPrefab->Instantiate();

			pObj->Transform()->SetRelativePos(Vec3(3200, 220.f, 0.f));
			pObj->GetRenderComponent()->SetSharedMaterial(pObj->GetRenderComponent()->GetDynamicMaterial());

			evninfo.eType = EVENT_TYPE::CREATE_OBJ;
			evninfo.lParam = (DWORD_PTR)pObj;
			evninfo.wParam = 5;

			CEventMgr::GetInst()->AddEvent(evninfo);
			vObj.push_back(pObj);

			pObj = pPrefab->Instantiate();
			pObj->GetRenderComponent()->SetSharedMaterial(pObj->GetRenderComponent()->GetDynamicMaterial());

			pObj->Transform()->SetRelativePos(Vec3(3200.f, -100.f, 0.f));

			evninfo.eType = EVENT_TYPE::CREATE_OBJ;
			evninfo.lParam = (DWORD_PTR)pObj;
			evninfo.wParam = 5;

			CEventMgr::GetInst()->AddEvent(evninfo);
			vObj.push_back(pObj);

			pPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Cubulon.pref");

			pObj = pPrefab->Instantiate();

			pObj->Transform()->SetRelativePos(Vec3(3400.f, 60.f, 0.f));
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
				if (vObj[i]->GetName() == L"Door_7")
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

