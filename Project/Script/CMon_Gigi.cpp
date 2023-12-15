#include "pch.h"
#include "CMon_Gigi.h"

#include <Engine/CAnimator2D.h>
#include <Engine/CAnimation2D.h>
#include <Engine/CEventMgr.h>

#include "CM_Bullet.h"


CMon_Gigi::CMon_Gigi()
	:CMonsterScript((int)SCRIPT_TYPE::MON_GIGI)
{
	m_HP = 75;
	m_DetectRange = 500.f;
	m_AttackRange = 350.f;
	m_fSpeed = 80.f;
}

CMon_Gigi::~CMon_Gigi()
{
}

void CMon_Gigi::start()
{
	SetState(MON_STATE::IDLE);
}

void CMon_Gigi::Idle()
{
	if (m_fDistance < m_DetectRange)
	{
		ChangeState(MON_STATE::TRACE);
	}

	DecideIdleAnimation();
}

void CMon_Gigi::Trace()
{
	IsAttackPossible();
	IsAttackStateAvailable();
	SetPosition();
	DecideTraceAnimation();
}

void CMon_Gigi::Attack()
{
	m_fTime += DT;
	CheckAttackTime();
	DecideAttackDirection();
}

void CMon_Gigi::Hit()
{
	m_fTime += DT;
	DecideHitDirection();
	CheckAndTransitionHitToIdle();
}

void CMon_Gigi::Dead()
{
	m_fTime += DT;
	CheckDeathSound();
	Animator2D()->Play(L"Left_Dead", true);
	TimeDestroyCheck(0.4f);
}

void CMon_Gigi::DecideTraceAnimation()
{
	if ((m_Angle <= 181.f && m_Angle >= 90.f) || (m_Angle <= -90.f && m_Angle >= -181.f))
		Animator2D()->Play(L"Left_Idle", true);
	else if ((m_Angle < 90.f && m_Angle > 0.f) || (m_Angle <= 0.f && m_Angle >= -90.f))
		Animator2D()->Play(L"Right_Idle", true);
}

void CMon_Gigi::SetPosition()
{
	Vec3 vPlayerPos = CSceneMgr::GetInst()->GetCurScene()->GetPlayerPos();
	Vec3 vMonPos = Transform()->GetWorldPos();
	Vec2 vDir = GetDirBetweenObjects(vMonPos, vPlayerPos);
	vMonPos += Vec3(vDir.x, vDir.y, 0.f) * m_fSpeed * DT;
	Transform()->SetRelativePos(vMonPos.x, vMonPos.y, Transform()->GetRelativePos().y);
}

void CMon_Gigi::IsAttackStateAvailable()
{
	if (m_bAttack)
	{
		if (m_fDistance < m_AttackRange)
		{
			m_BehaviorAngle = m_Angle;
			m_fTime = 0.f;
			ChangeState(MON_STATE::ATK);
		}
	}
}

void CMon_Gigi::IsAttackPossible()
{
	if (m_bAttack) return;

	m_fAttackTime += DT;
	if (m_fAttackTime >= 1.5f)
	{
		m_bAttack = true;
		m_fAttackTime = 0.f;
	}
}

void CMon_Gigi::DecideAttackDirection()
{
	if ((m_BehaviorAngle <= 181.f && m_BehaviorAngle >= 90.f) || (m_BehaviorAngle <= -90.f && m_BehaviorAngle >= -181.f))
	{
		if (m_fTime <= 1.2f && m_fTime >= 0.f)
		{
			Animator2D()->Play(L"Left_Atk", false);
		}
		else
		{
			Animator2D()->FindAnim(L"Left_Atk")->Reset();
			m_BehaviorAngle = 0.f;
			m_fTime = 0.f;
			ChangeState(MON_STATE::IDLE);
		}
	}
	else if ((m_BehaviorAngle < 90.f && m_BehaviorAngle > 0.f) || (m_BehaviorAngle <= 0.f && m_BehaviorAngle >= -90.f))
	{
		if (m_fTime <= 1.2f && m_fTime >= 0.f)
		{
			Animator2D()->Play(L"Right_Atk", false);
		}
		else
		{
			Animator2D()->FindAnim(L"Right_Atk")->Reset();
			m_BehaviorAngle = 0.f;
			m_fTime = 0.f;
			ChangeState(MON_STATE::IDLE);
		}
	}
}

void CMon_Gigi::CheckAttackTime()
{
	if (m_fTime >= 1.f && m_bAttack)
	{
		Shoot();
		m_bAttack = false;
	}
}


void CMon_Gigi::CheckAndTransitionHitToIdle()
{
	if (m_fTime > 0.5f)
	{
		Animator2D()->FindAnim(L"Right_Hit")->Reset();
		Animator2D()->FindAnim(L"Left_Hit")->Reset();
		m_fTime = 0.f;
		ChangeState(MON_STATE::IDLE);
	}
}

void CMon_Gigi::DecideHitDirection()
{
	if ((m_Angle >= 0.f && m_Angle <= 90.f) || (m_Angle <= 0.f && m_Angle >= -90.f))
		Animator2D()->Play(L"Right_Hit", true);
	else if ((m_Angle >= 90.f && m_Angle <= 181.f) || (m_Angle <= -90.f && m_Angle >= -181.f))
		Animator2D()->Play(L"Left_Hit", true);
}

void CMon_Gigi::TimeDestroyCheck(float Time)
{
	if (m_fTime >= Time)
	{
		GetOwner()->Destroy();
	}
}

void CMon_Gigi::CheckDeathSound()
{
	if (!m_Sound)
	{
		m_Sound = true;
		Ptr<CSound> pSound = CResMgr::GetInst()->Load<CSound>(L"sound\\gigi\\Dead.wav", L"sound\\gigi\\Dead.wav");
		pSound->Play(1, 0.12f, true);
	}
}

void CMon_Gigi::Shoot()
{
	CGameObject* pObj = PrefabInstantiate(L"prefab\\M_Bullet.pref");
	Vec3 pPlayerPos = CSceneMgr::GetInst()->GetCurScene()->GetPlayerPos();
	Vec3 pTrans = Transform()->GetRelativePos();
	Vec2 vDir =  GetDirBetweenObjects(pTrans, pPlayerPos);
	float m_Angle = CalcRadianBetweenObjects(pTrans, pPlayerPos);

	InitBulletAndCreate(pObj, pTrans, vDir);

	pObj = PrefabInstantiate(L"prefab\\M_Bullet.pref");
	float fsin = sinf(m_Angle+0.4f);
	float fcos = cosf(m_Angle+0.4f);
	vDir = Vec3(fcos, fsin, 0.f); vDir.Normalize();
	InitBulletAndCreate(pObj, pTrans, vDir);


	pObj = PrefabInstantiate(L"prefab\\M_Bullet.pref");
	fsin = sinf(m_Angle-0.4f);
	fcos = cosf(m_Angle-0.4f);
	vDir = Vec3(fcos, fsin, 0.f); vDir.Normalize();
	InitBulletAndCreate(pObj, pTrans, vDir);
}

void CMon_Gigi::InitBulletAndCreate(CGameObject* pObj, Vec3& pTrans, Vec2& vDir)
{
	pObj->Transform()->SetRelativePos(Vec3(pTrans.x, pTrans.y, 0.f));
	CM_Bullet* pBullet = (CM_Bullet*)CScriptMgr::GetScript((UINT)SCRIPT_TYPE::M_BULLET);
	pBullet->SetDir(Vec3(vDir.x, vDir.y, 0));
	pBullet->SetSpeed(400.f);
	pObj->AddComponent(pBullet);

	AddCreateObjectEvent(pObj, 6);
}


void CMon_Gigi::OnCollisionEnter(CGameObject* _OtherObject)
{
	if (_OtherObject->GetName() == L"P_Bullet")
	{
		m_HP -= 15;

		CheckHit();
		CheckDeath();
	}
	if (_OtherObject->GetName() == L"Ice_Bullet")
	{
		m_HP -= 20;
		m_IceCheck = 1;

		CheckHit();
		CheckDeath();
	}
}

void CMon_Gigi::CheckHit()
{
	if (GetState() == MON_STATE::IDLE || GetState() == MON_STATE::TRACE)
		ChangeState(MON_STATE::HIT);
}

void CMon_Gigi::CheckDeath()
{
	if (m_HP <= 0 && GetState() != MON_STATE::DEAD)
	{
		m_fTime = 0.f;
		Animator2D()->FindAnim(L"Left_Dead")->Reset();
		ChangeState(MON_STATE::DEAD);
	}
}

void CMon_Gigi::DecideIdleAnimation()
{
	if ((m_Angle <= 181.f && m_Angle >= 90.f) || (m_Angle <= -90.f && m_Angle >= -181.f))
		Animator2D()->Play(L"Left_Idle", true);
	else if ((m_Angle < 90.f && m_Angle > 0.f) || (m_Angle <= 0.f && m_Angle >= -90.f))
		Animator2D()->Play(L"Right_Idle", true);
}
