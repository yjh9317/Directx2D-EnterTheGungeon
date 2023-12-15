#include "pch.h"
#include "CMon_Booklets.h"

#include <Engine/CAnimator2D.h>
#include <Engine/CAnimation2D.h>
#include <Engine/CEventMgr.h>
#include "CM_Bullet.h"

CMon_Booklets::CMon_Booklets()
	: CMonsterScript((int)SCRIPT_TYPE::MON_BOOKLETS)
	,m_AtkCycle(0.f)
	, m_Sound(false)
{
	m_HP = 70;
	m_DetectRange = 400.f;
	m_AttackRange = 250.f;
	m_fSpeed = 50.f;
}

CMon_Booklets::~CMon_Booklets()
{
}


void CMon_Booklets::Idle()
{
	Animator2D()->Play(L"Bookllets_Idle", true);
	if (m_fDistance < m_DetectRange)
	{
		ChangeState(MON_STATE::TRACE);
	}
}

void CMon_Booklets::Trace()
{
	Animator2D()->Play(L"Bookllets_Idle", true);
	IsAttackPossible();

	if (m_bAttack)
	{
		IsAttackStateAvailable();
	}
	SetPosition();
	
}

void CMon_Booklets::IsAttackPossible()
{
	if (m_bAttack) return;

	m_fAttackTime += DT;
	if (m_fAttackTime >= 1.5f)
	{
		m_bAttack = true;
		m_fAttackTime = 0.f;
	}
}

void CMon_Booklets::IsAttackStateAvailable()
{
	if (m_fDistance < m_AttackRange)
	{
		m_BehaviorAngle = m_Angle;
		ChangeState(MON_STATE::ATK);
	}
}

void CMon_Booklets::SetPosition()
{
	Vec3 vPlayerPos = CSceneMgr::GetInst()->GetCurScene()->GetPlayerPos();
	Vec3 vMonPos = Transform()->GetWorldPos();
	Vec2 vDir = GetDirBetweenObjects(vMonPos, vPlayerPos);
	vMonPos += Vec3(vDir.x, vDir.y, 0.f) * m_fSpeed * DT;
	Transform()->SetRelativePos(vMonPos.x, vMonPos.y, 0);
}

void CMon_Booklets::Attack()
{
	SetAttackSound();
	m_fTime += DT;

	AttackCycle();

}

void CMon_Booklets::AttackCycle()
{
	if (m_fTime > 0.6f && m_fTime <= 1.8f)
	{
		m_AtkCycle += DT;
		if (m_AtkCycle >0.1f)
		{
			m_AtkCycle = 0.f;
			Shoot();
		}
	}

	if (m_fTime < 1.9f && m_fTime >= 0.f)
		Animator2D()->Play(L"Bookllets_Atk", false);
	else if (m_fTime <= 2.8f && m_fTime >= 1.9f)
		Animator2D()->Play(L"Bookllets_Atk_Finish", false);
	else 
		AttackToIdle();
}

void CMon_Booklets::AttackToIdle()
{
	m_fTime = 0.f;
	m_AtkCycle = 0.f;
	m_bAttack = false;
	m_Sound = false;
	Animator2D()->FindAnim(L"Bookllets_Atk")->Reset();
	Animator2D()->FindAnim(L"Bookllets_Atk_Finish")->Reset();
	ChangeState(MON_STATE::IDLE);
}

void CMon_Booklets::SetAttackSound()
{
	if (!m_Sound)
	{
		m_Sound = true;
		Ptr<CSound> pSound = CResMgr::GetInst()->Load<CSound>(L"sound\\bookllet\\Blast.wav", L"sound\\bookllet\\Blast.wav");
		pSound->Play(1, 0.2f, true);
	}
}

void CMon_Booklets::Dead()
{
	Animator2D()->Play(L"Bookllets_Death", false);

	SetDeathSound();
	TimeDestroyCheck(0.4f);
}

void CMon_Booklets::TimeDestroyCheck(float Time)
{
	m_fTime += DT;

	if (m_fTime > Time)
	{
		GetOwner()->Destroy();
	}
}

void CMon_Booklets::SetDeathSound()
{
	if (!m_DeathSound)
	{
		m_DeathSound = true;
		Ptr<CSound> pSound = CResMgr::GetInst()->Load<CSound>(L"sound\\bookllet\\Dead.wav", L"sound\\bookllet\\Dead.wav");
		pSound->Play(1, 0.1f, true);
	}
}

void CMon_Booklets::Shoot()
{
	CGameObject* pObj = PrefabInstantiate(L"prefab\\M_Bullet.pref");
	Vec3 pTrans = Transform()->GetRelativePos();
	Vec3 pPlayerPos = CSceneMgr::GetInst()->GetCurScene()->GetPlayerPos();
	pObj->Transform()->SetRelativePos(Vec3(pTrans.x, pTrans.y, 0.f));
	Vec2 vDir = GetDirBetweenObjects(pTrans, pPlayerPos);

	CM_Bullet* pBullet = (CM_Bullet*)CScriptMgr::GetScript((UINT)SCRIPT_TYPE::M_BULLET);
	pBullet->SetDir(Vec3(vDir.x,vDir.y,0));
	pBullet->SetSpeed(300.f);
	pBullet->WaitCheck(true);
	pBullet->WaitTime(0.1f);
	pObj->AddComponent(pBullet);

	AddCreateObjectEvent(pObj, 6);
}


void CMon_Booklets::OnCollisionEnter(CGameObject* _OtherObject)
{
	if (_OtherObject->GetName() == L"P_Bullet")
		m_HP -= 15;

	if (_OtherObject->GetName() == L"Ice_Bullet")
	{
		m_HP -= 20;
		m_IceCheck = 1;
	}
	CheckDeath();

}

void CMon_Booklets::CheckDeath()
{
	if (m_HP <= 0 && GetState() != MON_STATE::DEAD)
	{
		m_fTime = 0.f;
		ChangeState(MON_STATE::DEAD);
	}
}
