#include "pch.h"
#include "CHeadGunScript.h"

#include <Engine/CAnimator2D.h>

#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>
#include <Engine/CEventMgr.h>
#include "CMissileScript.h"


CHeadGunScript::CHeadGunScript()
	:CScript((int)SCRIPT_TYPE::HEADGUNSCRIPT)
	, m_fTime(0.f)

{
}

CHeadGunScript::~CHeadGunScript()
{
}

void CHeadGunScript::start()
{
}

void CHeadGunScript::update()
{
	m_fTime += DT;

	if (m_fTime >= m_fDeathTime) GetOwner()->Destroy();

	if (m_fTime > 1.6f && !m_bAttack)
	{
		m_bAttack = true;
		Shoot();
	}

	if (m_fTime <= 0.6f)
	{
		Animator2D()->Play(L"HeadGun_Start",false);
	}
	else if(m_fTime <= 2.5f)
	{
		Vec3 vPos = Transform()->GetWorldPos();
		Vec3 vPlayerPos = CSceneMgr::GetInst()->GetCurScene()->GetPlayerPos();
		float Angle = CalcAngleBetweenObjects(vPos, vPlayerPos);
		IdleAnimationBasedOnAngle(Angle);
	}
}

void CHeadGunScript::IdleAnimationBasedOnAngle(float Angle)
{
	if (Angle >= -22.5f && Angle <= 22.5f)
	{
		Animator2D()->Play(L"Right", false);
	}
	else if (Angle > 22.5f && Angle <= 67.5f)
	{
		Animator2D()->Play(L"Up_Right", false);
	}
	else if (Angle > 67.5f && Angle <= 112.5f)
	{
		Animator2D()->Play(L"Up", false);
	}
	else if (Angle > 112.5f && Angle <= 157.5f)
	{
		Animator2D()->Play(L"Up_Left", false);
	}
	else if ((Angle >= 157.5f && Angle <= 181.f) || (Angle >= -181.f && Angle < -157.5f))
	{
		Animator2D()->Play(L"Left", false);
	}
	else if (Angle > -157.5f && Angle <= -112.5f)
	{
		Animator2D()->Play(L"Down_Left", false);
	}
	else if (Angle > -112.5f && Angle <= -67.5f)
	{
		Animator2D()->Play(L"Down", false);
	}
	else if (Angle > -67.5f && Angle <= -22.5f)
	{
		Animator2D()->Play(L"Down_Right", false);
	}
}

void CHeadGunScript::Shoot()
{
	Ptr<CSound> pSound = CResMgr::GetInst()->Load<CSound>(L"sound\\headgun\\Shot.wav", L"sound\\headgun\\Shot.wav");
	pSound->Play(1, 0.12f, true);

	Vec3 pPlayerPos = CSceneMgr::GetInst()->GetCurScene()->GetPlayerPos();
	Vec3 pTrans = Transform()->GetRelativePos();

	CGameObject* pObj = PrefabInstantiate(L"prefab\\BombBullet.pref");
	if (nullptr == pObj) return;

	pObj->Transform()->SetRelativePos(Vec3(pTrans.x, pTrans.y, 0.f));
	Vec2 vDir = GetDirBetweenObjects(pTrans, pPlayerPos); 

	CMissileScript* pBullet = (CMissileScript*)CScriptMgr::GetScript((UINT)SCRIPT_TYPE::MISSILESCRIPT);
	pBullet->SetDir(Vec3(vDir.x,vDir.y,0));
	pObj->AddComponent(pBullet);

	AddCreateObjectEvent(pObj, 6);
}