#include "pch.h"
#include "CMon_Kin.h"

#include <Engine/CAnimator2D.h>
#include <Engine/CAnimation2D.h>

CMon_Kin::CMon_Kin()
	:CMonsterScript((int)SCRIPT_TYPE::MON_KIN)
{
	m_HP = 50;
	m_DetectRange = 400.f;
	m_AttackRange = 200.f;
	m_fSpeed = 50.f;
}

CMon_Kin::~CMon_Kin()
{
}


void CMon_Kin::start()
{
}

void CMon_Kin::Idle()
{
	if (m_fDistance < m_DetectRange)
	{
		ChangeState(MON_STATE::TRACE);
	}

	DecideIdleAnimation();
}

void CMon_Kin::DecideIdleAnimation()
{
	if (m_Angle <= 0.f && m_Angle >= -181.f)
		Animator2D()->Play(L"Front_Idle", true);
	else if (m_Angle > 0.f && m_Angle <= 181.f)
		Animator2D()->Play(L"Back_Idle", true);
}

void CMon_Kin::Trace()
{
	SetPosition();
	DecideTraceAnimation();
}

void CMon_Kin::SetPosition()
{
	Vec3 vPlayerPos = CSceneMgr::GetInst()->GetCurScene()->GetPlayerPos();
	Vec3 vMonPos = Transform()->GetWorldPos();
	Vec2 vDir = GetDirBetweenObjects(vMonPos, vPlayerPos);

	vMonPos += Vec3(vDir.x, vDir.y, 0) * m_fSpeed * DT;
	Transform()->SetRelativePos(vMonPos.x, vMonPos.y, 0);
}

void CMon_Kin::DecideTraceAnimation()
{
	if ((m_Angle >= 0.f && m_Angle <= 45.f) || (m_Angle <= 0.f && m_Angle >= -30.f))
		Animator2D()->Play(L"Right_Walk", true);
	else if (m_Angle > 45.f && m_Angle <= 90.f)
		Animator2D()->Play(L"Back_R_Walk", true);
	else if (m_Angle > 90.f && m_Angle < 135.f)
		Animator2D()->Play(L"Back_L_Walk", true);
	else if ((m_Angle >= 135.f && m_Angle <= 181.f) || (m_Angle < -150.f && m_Angle >= -181.f))
		Animator2D()->Play(L"Left_Walk", true);
	else if (m_Angle < -90.f && m_Angle >= -150.f)
		Animator2D()->Play(L"Front_L_Walk", true);
	else if (m_Angle <-30.f && m_Angle >= -90.f)
		Animator2D()->Play(L"Front_R_Walk", true);
}

void CMon_Kin::Hit()
{
	m_fTime += DT;
	CheckAndTransitionHitToIdle();
	SetPosition();
	DecideHitAnimation();
}

void CMon_Kin::DecideHitAnimation()
{
	if ((m_Angle >= 0.f && m_Angle <= 45.f) || (m_Angle <= 0.f && m_Angle >= -45.f))
		Animator2D()->Play(L"Right_Hit", true);
	else if ((m_Angle >= 45.f && m_Angle <= 90.f) || (m_Angle <= -45.f && m_Angle >= -90.f))
		Animator2D()->Play(L"FrontBack_R_Hit", true);
	else if ((m_Angle >= 90.f && m_Angle <= 135.f) || (m_Angle <= -90.f && m_Angle >= -135.f))
		Animator2D()->Play(L"FrontBack_L_Hit", true);
	else if ((m_Angle >= 135.f && m_Angle <= 181.f) || (m_Angle <= -135.f && m_Angle >= -181.f))
		Animator2D()->Play(L"Left_Hit", true);
}

void CMon_Kin::CheckAndTransitionHitToIdle()
{
	if (m_fTime > 0.4f)
	{
		m_fTime = 0.f;
		ChangeState(MON_STATE::IDLE);
	}
}

void CMon_Kin::Dead()
{
	m_fTime += DT;
	Animator2D()->Play(L"Dead", false);
	TimeDestroyCheck(0.7f);
}

void CMon_Kin::TimeDestroyCheck(float Time)
{
	if (m_fTime >= Time)
	{
		GetOwner()->Destroy();
	}
}


void CMon_Kin::OnCollisionEnter(CGameObject* _OtherObject)
{
	if (_OtherObject->GetName() == L"P_Bullet")
	{
		m_HP -= 15;

		PlayHurtSound();
		CheckHit();
		CheckDeath();
	}

	if (_OtherObject->GetName() == L"Ice_Bullet")
	{
		m_HP -= 20;
		m_IceCheck = 1;

		PlayHurtSound();
		CheckHit();
		CheckDeath();
	}

}

void CMon_Kin::PlayHurtSound()
{
	Ptr<CSound> pSound = CResMgr::GetInst()->Load<CSound>(L"sound\\Kin\\Hurt.wav", L"sound\\Kin\\Hurt.wav");
	pSound->Play(1, 0.2f, true);
}

void CMon_Kin::CheckHit()
{
	if (GetState() == MON_STATE::IDLE || GetState() == MON_STATE::TRACE)
	{
		ChangeState(MON_STATE::HIT);
	}
}

void CMon_Kin::CheckDeath()
{
	if (m_HP <= 0 && GetState() != MON_STATE::DEAD)
	{
		m_fTime = 0.f;
		Ptr<CSound> pSound = CResMgr::GetInst()->Load<CSound>(L"sound\\Kin\\Dead.wav", L"sound\\Kin\\Dead.wav");
		pSound->Play(1, 0.2f, true);
		ChangeState(MON_STATE::DEAD);
	}
}
