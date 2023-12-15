#include "pch.h"
#include "CBossRoomEvent.h"

#include <Engine/CEventMgr.h>
#include <Engine/CAnimator2D.h>
#include <Engine/CAnimation2D.h>

#include "CDoorScript.h"
#include "CCameraMoveScript.h"

CBossRoomEvent::CBossRoomEvent()
	:CScript((int)SCRIPT_TYPE::BOSSROOMEVENT)
{
}

CBossRoomEvent::~CBossRoomEvent()
{
}


void CBossRoomEvent::update()
{

}

void CBossRoomEvent::OnCollisionEnter(CGameObject* _pOtherObj)
{
	if (!m_Start && _pOtherObj->GetName() == L"Player")
	{
		m_Sound = true;
		m_Start = true;
		Ptr<CPrefab> pPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Boss.pref");
		CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
		tEventInfo evninfo;

		CGameObject* pObj = pPrefab->Instantiate();

		pObj->Transform()->SetRelativePos(Vec3(3950, 2650.f, 500.f));
		pObj->GetRenderComponent()->SetSharedMaterial(pObj->GetRenderComponent()->GetDynamicMaterial());

		evninfo.eType = EVENT_TYPE::CREATE_OBJ;
		evninfo.lParam = (DWORD_PTR)pObj;
		evninfo.wParam = 5;

		CEventMgr::GetInst()->AddEvent(evninfo);


		CScene* pScene = CSceneMgr::GetInst()->GetCurScene();
		CLayer* pLayer = pScene->GetLayer(1);
		vector<CGameObject*> vObj = pLayer->GetRootObjects();

		for (int i = 0; i < vObj.size(); ++i)
		{
			if (vObj[i]->GetName() == L"Door_11")
			{
				CDoorScript* pScript = vObj[i]->GetScript<CDoorScript>();
				if (nullptr == pScript)
					return;

				pScript->SetDoorType(DOOR_TYPE::Close);
				m_b = true;
			}
		}
		
		pLayer = pScene->GetLayer(0);
		vObj = pLayer->GetRootObjects();
		for (int i = 0; i < vObj.size(); ++i)
		{
			if (vObj[i]->GetName() == L"MainCamera")
			{
				CCameraMoveScript* pCameraScript = vObj[i]->GetScript<CCameraMoveScript>();
				pCameraScript->SetbEvent(true);
				pCameraScript->SetBossPos(Vec3(3950, 2650.f, 500.f));
			}
		}

		Ptr<CSound> pSound = CResMgr::GetInst()->Load<CSound>(L"sound\\ost\\BossFight.mp3", L"sound\\ost\\BossFight.mp3");
		pSound->Play(1, 0.06f, true);

		pSound = CResMgr::GetInst()->Load<CSound>(L"sound\\ost\\Fight.mp3", L"sound\\ost\\Fight.mp3");
		pSound->Stop();
	}
}

