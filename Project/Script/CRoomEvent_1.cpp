#include "pch.h"
#include "CRoomEvent_1.h"

#include <Engine/CEventMgr.h>
#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>
#include <Engine/CLayer.h>
#include <Engine/CAnimator2D.h>
#include <Engine/CAnimation2D.h>

#include "CDoorScript.h"




CRoomEvent_1::CRoomEvent_1()
	:CScript((int)SCRIPT_TYPE::ROOMEVENT_1)
	, m_Start(false)
	, m_b(false)
{
}

CRoomEvent_1::~CRoomEvent_1()
{
}


void CRoomEvent_1::update()
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
				if (vObj[i]->GetName() == L"Door_2")
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

void CRoomEvent_1::OnCollisionEnter(CGameObject* _pOtherObj)
{
	if (!m_Start && _pOtherObj->GetName() == L"Player")
	{

		m_b = true;
		m_Start = true;

		Ptr<CPrefab> pPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Bullats.pref");
		CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
		tEventInfo evninfo;

		CGameObject* pObj = pPrefab->Instantiate();

		pObj->Transform()->SetRelativePos(Vec3(1250.f, -200.f, 550.f));


		evninfo.eType = EVENT_TYPE::CREATE_OBJ;
		evninfo.lParam = (DWORD_PTR)pObj;
		evninfo.wParam = 5;

		CEventMgr::GetInst()->AddEvent(evninfo);
		vObj.push_back(pObj);

		pObj = pPrefab->Instantiate();

		pObj->Transform()->SetRelativePos(Vec3(1000.f, 250.f, 550.f));

		evninfo.eType = EVENT_TYPE::CREATE_OBJ;
		evninfo.lParam = (DWORD_PTR)pObj;
		evninfo.wParam = 5;

		CEventMgr::GetInst()->AddEvent(evninfo);
		vObj.push_back(pObj);

		pObj = pPrefab->Instantiate();

		pObj->Transform()->SetRelativePos(Vec3(1000.f, -200.f, 550.f));

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
			if (vObj[i]->GetName() == L"Door_2")
			{
				CDoorScript* pScript = vObj[i]->GetScript<CDoorScript>();
				if (nullptr == pScript)
					return;

				pScript->SetDoorType(DOOR_TYPE::Close);
			}
		}

		Ptr<CSound> pSound = CResMgr::GetInst()->Load<CSound>(L"sound\\ost\\Fight.mp3", L"sound\\ost\\Fight.mp3");
		pSound->Play(1, 0.08f, true);
	}
	else
		return;
}
