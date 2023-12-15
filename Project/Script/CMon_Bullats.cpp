#include "pch.h"
#include "CMon_Bullats.h"

#include <Engine/CAnimator2D.h>
#include <Engine/CAnimation2D.h>

#include "CWallScript.h"
#include "CTableScript.h"
#include "CDoorScript.h"

CMon_Bullats::CMon_Bullats()
	:CMonsterScript((int)SCRIPT_TYPE::MON_BULLATS)
	, m_vDir(0.f, 0.f)
	, m_bSound(false)
	, m_bDeathSound(false)
{
	m_HP = 15;
	m_DetectRange = 300.f;
	m_AttackRange = 150.f;
	m_fSpeed = 120.f;
}

CMon_Bullats::~CMon_Bullats()
{
}


void CMon_Bullats::Idle()
{
	if (m_fDistance < m_DetectRange)
	{
		ChangeState(MON_STATE::TRACE);
	}

	Animator2D()->Play(L"Bullats_Idle", true);
}

void CMon_Bullats::Trace()
{
	Animator2D()->Play(L"Bullats_Idle", true);

	Vec3 vPlayerPos = CSceneMgr::GetInst()->GetCurScene()->GetPlayerPos();
	Vec3 vMonPos = Transform()->GetWorldPos();

	IsAttackStateAvailable(vMonPos, vPlayerPos);

	SetTracePosition(vMonPos, vPlayerPos);
}

void CMon_Bullats::Attack()
{
	m_fTime += DT;
	CheckAttackTime();
}

void CMon_Bullats::Dead()
{
	Animator2D()->Play(L"Bullats_Dead", false);
	CheckDeathSound();
	TimeDestroyCheck(0.4f);
}


void CMon_Bullats::IsAttackStateAvailable(Vec3& vMonPos, Vec3& vPlayerPos)
{
	if (m_fDistance < m_AttackRange)
	{
		m_Dir = GetDirBetweenObjects(vMonPos, vPlayerPos);
		ChangeState(MON_STATE::ATK);
	}
}

void CMon_Bullats::SetTracePosition(Vec3& vMonPos, Vec3& vPlayerPos)
{
	Vec2 vDir = GetDirBetweenObjects(vMonPos, vPlayerPos);
	vMonPos += Vec3(vDir.x, vDir.y, 0) * m_fSpeed * DT;
	Transform()->SetRelativePos(vMonPos.x, vMonPos.y, 0);
}



void CMon_Bullats::CheckAttackTime()
{
	if (m_fTime >= 0.f && m_fTime < 0.8f)
	{
		Animator2D()->Play(L"Bullats_Atk_Prev", true);
		CheckChargeSound();
	}
	else if (m_fTime >= 0.8f)
	{
		CheckTackleSound();
		Vec3 vMonPos = Transform()->GetWorldPos();
		vMonPos += Vec3(m_Dir.x, m_Dir.y, 0) * m_fSpeed * DT * 2.f;
		Transform()->SetRelativePos(vMonPos.x, vMonPos.y, 0);
		Animator2D()->Play(L"Bullats_Atk", true);
	}
}

void CMon_Bullats::CheckTackleSound()
{
	if (!m_bSound)
	{
		m_bSound = true;
		Ptr<CSound> pSound = CResMgr::GetInst()->Load<CSound>(L"sound\\bullat\\Tackle.wav", L"sound\\bullat\\Tackle.wav");
		pSound->Play(1, 0.2f, true);
	}
}

void CMon_Bullats::CheckChargeSound()
{
	if (!m_bSound)
	{
		m_bSound = true;
		Ptr<CSound> pSound = CResMgr::GetInst()->Load<CSound>(L"sound\\bullat\\Charge.wav", L"sound\\bullat\\Charge.wav");
		pSound->Play(1, 0.2f, true);
	}
}


void CMon_Bullats::TimeDestroyCheck(float Time)
{
	m_fTime += DT;

	if (m_fTime > Time)
	{
		GetOwner()->Destroy();
	}
}

void CMon_Bullats::CheckDeathSound()
{
	if (!m_bDeathSound)
	{
		m_bDeathSound = true;
		Ptr<CSound> pSound = CResMgr::GetInst()->Load<CSound>(L"sound\\bullat\\Death.wav", L"sound\\bullat\\Death.wav");
		pSound->Play(1, 0.15f, true);
	}
}

void CMon_Bullats::OnCollisionEnter(CGameObject* _OtherObject)
{
	MON_STATE _State = GetState();

	if (_OtherObject->GetName() == L"P_Bullet")
	{
		m_HP -= 15;
	}

	if (_OtherObject->GetName() == L"Ice_Bullet")
	{
		m_HP -= 20;
		m_IceCheck = 1;
	}

	CheckDeath();


	if (_State == MON_STATE::ATK)
	{
		if (_OtherObject->GetName() == L"Player" || _OtherObject->GetScript<CWallScript>() || _OtherObject->GetScript<CDoorScript>())
		{
			m_fTime = 0.f;
			Animator2D()->FindAnim(L"Bullats_Dead")->Reset();
			ChangeState(MON_STATE::DEAD);
		}

		if (_OtherObject->GetScript<CTableScript>())
		{
			CTableScript* pScript = _OtherObject->GetScript<CTableScript>();
			if (TABLE_TYPE::Stand == pScript->GetType() && GetState() != MON_STATE::DEAD)
			{
				m_fTime = 0.f;
				Animator2D()->FindAnim(L"Bullats_Dead")->Reset();
				ChangeState(MON_STATE::DEAD);
			}
		}
	}
}

void CMon_Bullats::CheckDeath()
{
	if (m_HP <= 0 && GetState() != MON_STATE::DEAD)
	{
		m_fTime = 0.f;
		Animator2D()->FindAnim(L"Bullats_Dead")->Reset();
		ChangeState(MON_STATE::DEAD);
	}
}
