#include "pch.h"
#include "CCameraMoveScript.h"

#include <Engine/CKeyMgr.h>
#include <Engine/CTimeMgr.h>
#include <Engine/CTransform.h>
#include <Engine/CCamera.h>

#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>
#include <Engine/CLayer.h>

#include <Engine/CEventMgr.h>
#include <Engine/CRenderMgr.h>

#include "CPlayerStateScript.h"
#include "CHeartScript.h"
#include "CBlankBulletScript.h"



CCameraMoveScript::CCameraMoveScript()
	: CScript((int)SCRIPT_TYPE::CAMERAMOVESCRIPT)
	, m_Filter(nullptr)
	, m_fCamSpeed(700.f)
	, m_PlayerTrace(false)
	, m_DistortionTime(0.f)
	, m_DistortionDist(0.f)
	, m_Event(false)
	, m_fTime(0.f)
	, m_BossPos{}
{
}

CCameraMoveScript::~CCameraMoveScript()
{
}

void CCameraMoveScript::start()
{
	RegisterPostProcess();

	if (CSceneMgr::GetInst()->GetCurScene()->GetSceneState() == SCENE_STATE::PLAY)
	{
		m_PlayerTrace = true;
	}
}

void CCameraMoveScript::update()
{
	Vec3 vPos = Transform()->GetRelativePos();
	float CamWidth = Camera()->GetWidth();
	float fSpeed = m_fCamSpeed;

	KeyInput(vPos);

	if (KEY_TAP(KEY::Q))
		ExecuteDistortion();

	if(m_bDistortion)
		RenewFilterValue();

	CheckPlayerTrace(vPos);

	
	if (m_Event)
	{
		m_PlayerTrace = false;
		m_fTime += DT;

		if (m_fTime > 6.f)
		{
			m_fTime = 0.f;
			m_Event = false;
			m_BossPos = {};
			m_PlayerTrace = true;
		}
		Vec3 vPlayerPos = CSceneMgr::GetInst()->GetCurScene()->GetPlayerPos();

		Vec2 vDir = Vec2(m_BossPos.x - vPlayerPos.x, m_BossPos.y - vPlayerPos.y);
		vDir.Normalize();

		Vec3 vPos = Transform()->GetRelativePos();

		if (m_fTime < 2.f)
		{
			vPos.x += vDir.x * 250.f * DT;
			vPos.y += vDir.y * 250.f * DT;
		}

		Transform()->SetRelativePos(vPos);
	}
	else
		Transform()->SetRelativePos(vPos);


}

void CCameraMoveScript::RenewFilterValue()
{
	if (m_DistortionTime > 0.7f)
	{
		m_Filter->Deactivate();
		m_bDistortion = false;
		m_DistortionTime = 0.f;
		m_DistortionDist = 0.f;
	}
	else
	{
		m_DistortionTime += DT;
		m_DistortionDist += DT;
		m_Filter->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0, &m_DistortionTime);
		m_Filter->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_1, &m_DistortionDist);
	}
}

void CCameraMoveScript::CheckPlayerTrace(Vec3& vPos)
{
	//if (KEY_TAP(KEY::_9))
	//{
	//	m_PlayerTrace = !m_PlayerTrace;
	//}
	if (m_PlayerTrace)
	{
		Vec3 pPlayerPos = CSceneMgr::GetInst()->GetCurScene()->GetPlayerPos();
		Vec2 vResolution = CDevice::GetInst()->GetRenderResolution();
		Vec2 vMousePos = CKeyMgr::GetInst()->GetMousePos();

		Vec2 vWinMouse = Vec2(vMousePos.x - vResolution.x / 2.f, vMousePos.y - vResolution.y / 2.f);
		vWinMouse /= 6.f;
		vPos = Vec3(pPlayerPos.x + vWinMouse.x, pPlayerPos.y - vWinMouse.y, 0.f);
	}
}

void CCameraMoveScript::ExecuteDistortion()
{
	if (m_Filter == nullptr || !CSceneMgr::GetInst()->GetCurScene()->GetPlayer()) return;

	bool HasBullet = CheckBulletCount();
	if (!HasBullet) return;

	RemoveAllMonsterBullet();
	CreateDistortion();
	TurnOnFilter();
}

void CCameraMoveScript::TurnOnFilter()
{
	m_bDistortion = true;
	m_Filter->Activate();
}

void CCameraMoveScript::CreateDistortion() const
{
	Ptr<CPrefab> pPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\BlankBulletEffect.pref");
	CGameObject* pObj = pPrefab->Instantiate();

	tEventInfo tevninfo;
	pObj->Transform()->SetRelativePos(CSceneMgr::GetInst()->GetCurScene()->GetPlayerPos());

	tevninfo.eType = EVENT_TYPE::CREATE_OBJ;
	tevninfo.lParam = (DWORD_PTR)pObj;
	tevninfo.wParam = 7;

	CEventMgr::GetInst()->AddEvent(tevninfo);

	Ptr<CSound> pSound = CResMgr::GetInst()->Load<CSound>(L"sound\\BlankBullet\\Sound.wav", L"sound\\BlankBullet\\Sound.wav");
	pSound->Play(1, 0.1f, true);
}

void CCameraMoveScript::RemoveAllMonsterBullet() const
{
	CScene* pScene = CSceneMgr::GetInst()->GetCurScene();
	vector<CGameObject*> vObj = pScene->GetLayer(6)->GetRootObjects();
	for (int i = 0; i < vObj.size(); ++i)
	{
		vObj[i]->Destroy();
	}
}

bool CCameraMoveScript::CheckBulletCount() const
{
	CGameObject* Player = CSceneMgr::GetInst()->GetCurScene()->GetPlayer();
	CPlayerStateScript* pScript = Player->GetScript<CPlayerStateScript>();
	vector<CGameObject*>& vBlankObj = pScript->GetBlankBullet();
	if (vBlankObj.size() == 0) return false;

	CGameObject* Obj = vBlankObj[vBlankObj.size() - 1];
	vBlankObj.pop_back();
	Obj->Destroy();
	return true;
}


void CCameraMoveScript::lateupdate()
{
	CGameObject* pPlayer = CSceneMgr::GetInst()->GetCurScene()->GetPlayer();
	if (nullptr == pPlayer)	return;

	CPlayerStateScript* pScript = pPlayer->GetScript<CPlayerStateScript>();
	if (nullptr == pScript)	return;

	SetHeartPos(pScript);
	SetBulletPos(pScript);
}

void CCameraMoveScript::SetBulletPos(CPlayerStateScript* pScript)
{
	const vector<CGameObject*>& v2 = pScript->GetBlankBullet();

	for (int i = 0; i < v2.size(); ++i)
	{
		CBlankBulletScript* _pScript = v2[i]->GetScript<CBlankBulletScript>();
		Vec3 vPos = Transform()->GetRelativePos();

		float vCamWidth = CRenderMgr::GetInst()->GetMainCamWidth();
		float vCamHeight = CRenderMgr::GetInst()->GetMainCamHeight();
		vPos.x -= vCamWidth / 2.f;
		vPos.y += vCamHeight / 2.f - 100.f;

		vPos.x += 50 + (50 * _pScript->GetNum());
		v2[i]->Transform()->SetRelativePos(vPos);
	}
}

void CCameraMoveScript::SetHeartPos(CPlayerStateScript* pScript)
{
	const vector<CGameObject*>& v = pScript->GetHeart();

	for (int i = 0; i < v.size(); ++i)
	{
		CHeartScript* _pScript = v[i]->GetScript<CHeartScript>();
		Vec3 vPos = Transform()->GetRelativePos();

		float vCamWidth = CRenderMgr::GetInst()->GetMainCamWidth();
		float vCamHeight = CRenderMgr::GetInst()->GetMainCamHeight();
		vPos.x -= vCamWidth / 2.f;
		vPos.y += vCamHeight / 2.f - 50.f;

		vPos.x += 50 + (50 * _pScript->GetNum());

		v[i]->Transform()->SetRelativePos(vPos);
	}
}

void CCameraMoveScript::RegisterPostProcess()
{
	if (m_Filter == nullptr)
	{
		CGameObject* pPostProcess = new CGameObject;
		pPostProcess->SetName(L"PostProcessObject");

		pPostProcess->AddComponent(new CTransform);
		pPostProcess->AddComponent(new CMeshRender);

		pPostProcess->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pPostProcess->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\PostProcessMtrl.mtrl"));

		tEventInfo tevninfo;

		tevninfo.eType = EVENT_TYPE::CREATE_OBJ;
		tevninfo.lParam = (DWORD_PTR)pPostProcess;
		tevninfo.wParam = 0;

		CEventMgr::GetInst()->AddEvent(tevninfo);
		pPostProcess->Deactivate();

		((CCameraMoveScript*)GetOwner()->GetScript(0))->SetFilter(pPostProcess);
	}
}

void CCameraMoveScript::KeyInput(Vec3& ObjectPos)
{
	float Speed = m_fCamSpeed;

	if (KEY_PRESSED(KEY::LSHFT))
	{
		Speed *= 2.f;
	}

	PersCamera(ObjectPos, Speed);
	OrthoCamera(ObjectPos);
}

void CCameraMoveScript::PersCamera(Vec3& ObjectPos, float Speed)
{
	if (PROJ_TYPE::PERSPECTIVE == Camera()->GetProjType())
	{
		if (KEY_PRESSED(KEY::UP))
		{
			Vec3 vFront = Transform()->GetWorldFrontDir();
			ObjectPos += DT * vFront * Speed;
		}

		if (KEY_PRESSED(KEY::DOWN))
		{
			Vec3 vFront = Transform()->GetWorldFrontDir();
			ObjectPos -= DT * vFront * Speed;
		}

		if (KEY_PRESSED(KEY::LEFT))
		{
			Vec3 vRight = Transform()->GetWorldRightDir();
			ObjectPos -= DT * vRight * Speed;
		}

		if (KEY_PRESSED(KEY::RIGHT))
		{
			Vec3 vRight = Transform()->GetWorldRightDir();
			ObjectPos += DT * vRight * Speed;
		}
	}
}

void CCameraMoveScript::OrthoCamera(Vec3& vPos)
{
	float CamWidth = Camera()->GetWidth();
	if (PROJ_TYPE::ORTHOGRAPHIC == Camera()->GetProjType())
	{
		if (KEY_PRESSED(KEY::W))
		{
			vPos.y += DT * m_fCamSpeed;
		}

		if (KEY_PRESSED(KEY::S))
		{
			vPos.y -= DT * m_fCamSpeed;
		}

		if (KEY_PRESSED(KEY::A))
		{
			vPos.x -= DT * m_fCamSpeed;
		}

		if (KEY_PRESSED(KEY::D))
		{
			vPos.x += DT * m_fCamSpeed;
		}
		if (KEY_PRESSED(KEY::_4))
		{
			CamWidth += DT * m_fCamSpeed;
			Camera()->SetWidth(CamWidth);
		}
		if (KEY_PRESSED(KEY::_5))
		{
			CamWidth -= DT * m_fCamSpeed;
			Camera()->SetWidth(CamWidth);
		}
	}
}


void CCameraMoveScript::SaveToScene(FILE* _pFile)
{

}

void CCameraMoveScript::LoadFromScene(FILE* _pFile)
{
}