#include "pch.h"
#include "CAKScript.h"

#include "CMonsterScript.h"

#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>

#include <Engine/CAnimator2D.h>
#include <Engine/CEventMgr.h>

#include "CM_Bullet.h"

CAKScript::CAKScript()
	: CScript((int)SCRIPT_TYPE::AKSCRIPT)
{
	m_Pos = Vec3(20.f, 0.f, 100.f);
}

CAKScript::~CAKScript()
{
}


void CAKScript::start()
{
	Transform()->SetIgnoreParentScale(true);
}

void CAKScript::update()
{
	IsAttackPossible(1.5f);

	CGameObject* pPlayer = CSceneMgr::GetInst()->GetCurScene()->GetPlayer();
	if (pPlayer == nullptr)	return;

	Vec3 PlayerPos = pPlayer->Transform()->GetWorldPos();
	Vec3 MonPos = GetOwner()->GetParent()->Transform()->GetWorldPos();
	Vec3 vPos = Transform()->GetWorldPos();

	float Dist = CalcDistBetweenObjects(PlayerPos,vPos);
	float Angle = CalcAngleBetweenObjects(vPos, PlayerPos);
	float Radian = CalcRadianBetweenObjects(vPos, PlayerPos);
	
	Vec2 Dir = GetDirBetweenObjects(vPos, PlayerPos);
	SetRotation(Angle, Radian);

	if (m_Atk)
	{
		Attack(Dist, Dir);
	}
}

void CAKScript::Attack(float Dist, Vec2& Dir)
{
	if (Dist <250.f)
	{
		Ptr<CSound> pSound = CResMgr::GetInst()->Load<CSound>(L"sound\\ak\\Shot.wav", L"sound\\ak\\Shot.wav");
		pSound->Play(1, 0.12f, true);

		CGameObject* pObj = PrefabInstantiate(L"prefab\\M_Bullet.pref");
		pObj->Transform()->SetRelativePos(GetOwner()->Transform()->GetWorldPos());

		CM_Bullet* pBullet = (CM_Bullet*)CScriptMgr::GetScript((UINT)SCRIPT_TYPE::M_BULLET);
		pBullet->SetSpeed(300.f);
		pBullet->SetDir(Vec3(Dir.x, Dir.y, 0.f));
		pObj->AddComponent(pBullet);

		AddCreateObjectEvent(pObj, 6);

		m_fTime = 0.f;
		m_Atk = false;
	}
}

void CAKScript::SetRotation(float Angle, float Radian)
{
	Vec3 vRot = Transform()->GetRelativeRotation();

	if (m_bRight)
	{
		if (Angle > 100.f || Angle < -100.f) m_bRight = false;

		Animator2D()->Play(L"R_AK", true);
		Transform()->SetRelativePos(Vec3(m_Pos.x, m_Pos.y, m_Pos.z));
		vRot.z = Radian;
	}
	else if (!m_bRight)
	{
		if ((Angle < 80.f && Angle >0.f) || (Angle <0.f && Angle>-80.f)) m_bRight = true;

		Animator2D()->Play(L"L_AK", true);
		Transform()->SetRelativePos(Vec3(-m_Pos.x, m_Pos.y, m_Pos.z));
		vRot.z = Radian - XM_PI;
	}
	Transform()->SetRelativeRotation(vRot);
}

void CAKScript::IsAttackPossible(float Time)
{
	if (m_Atk) return;
	m_fTime += DT;

	if (m_fTime > Time)
	{
		m_Atk = true;
	}
}

