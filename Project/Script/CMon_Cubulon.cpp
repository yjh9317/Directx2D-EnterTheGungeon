#include "pch.h"
#include "CMon_Cubulon.h"

#include <Engine/CAnimator2D.h>
#include <Engine/CAnimation2D.h>
#include <Engine/CScript.h>
#include <Engine/CEventMgr.h>
#include "CM_Bullet.h"

#include "CAStarScript.h"

CMon_Cubulon::CMon_Cubulon()
	:CMonsterScript((int)SCRIPT_TYPE::MON_CUBULON)
{
	m_HP = 110;
	m_DetectRange = 400.f;
	m_AttackRange = 300.f;
	m_fSpeed = 100.f;
}

CMon_Cubulon::~CMon_Cubulon()
{
}


void CMon_Cubulon::start()
{
	SetState(MON_STATE::IDLE);
}

void CMon_Cubulon::Idle()
{
	if (m_fDistance < m_DetectRange)
	{
		ChangeState(MON_STATE::TRACE);
	}
	DecideIdleAnimation();
}

void CMon_Cubulon::Trace()
{
	IsAttackPossible();
	DecideTraceAnimation();
	IsAttackStateAvailable();
	SetPosition();
}

void CMon_Cubulon::Attack()
{
	m_fTime += DT;
	CheckAttackTime();
	DecideAttackAnimation();
}

void CMon_Cubulon::Hit()
{
	DecideHitAnimation();
	CheckAndTransitionHitToIdle();
}

void CMon_Cubulon::Dead()
{
	Animator2D()->Play(L"Dead", false);
	CheckDeadSound();
	TimeDestroyCheck(0.4f);
}

void CMon_Cubulon::OnCollisionEnter(CGameObject* _OtherObject)
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

void CMon_Cubulon::CheckAttackTime()
{
	if (m_fTime >= 0.7f)
	{
		Ptr<CSound> pSound = CResMgr::GetInst()->Load<CSound>(L"sound\\cubulon\\Atk.wav", L"sound\\cubulon\\Atk.wav");
		pSound->Play(1, 0.12f, true);
		Shoot();
	}
}

void CMon_Cubulon::DecideIdleAnimation()
{
	if (m_Angle <= 90.f && m_Angle >= 45.f)
		Animator2D()->Play(L"Back_R_Idle", true);
	else if (m_Angle <= 135.f && m_Angle >= 90.f)
		Animator2D()->Play(L"Back_L_Idle", true);
	else if ((m_Angle <= 181.f && m_Angle >135.f) || (m_Angle <= -90.f && m_Angle >= -181.f))
		Animator2D()->Play(L"Left_Idle", true);
	else if ((m_Angle <= 45.f && m_Angle > 0.f) || (m_Angle <= 0.f && m_Angle >= -90.f))
		Animator2D()->Play(L"Right_Idle", true);
}

void CMon_Cubulon::IsAttackStateAvailable()
{
	if (m_bAttack)
	{
		if (m_fDistance < m_AttackRange && m_bAttack)
		{
			m_fTime = 0.f;
			m_BehaviorAngle = m_Angle;
			ChangeState(MON_STATE::ATK);
		}
	}
}

void CMon_Cubulon::SetPosition()
{
	Vec3 vPlayerPos = CSceneMgr::GetInst()->GetCurScene()->GetPlayerPos();
	Vec3 vMonPos = Transform()->GetWorldPos();
	Vec2 vDir = GetDirBetweenObjects(vMonPos, vPlayerPos);

	CAStarScript* pAstarScript = GetOwner()->GetScript<CAStarScript>();
	if (pAstarScript != nullptr)
	{
		vDir = pAstarScript->GetDir();
		vMonPos += Vec3(vDir.x, vDir.y, 0) * m_fSpeed * DT;
	}
	else
		vMonPos += Vec3(vDir.x, vDir.y, 0) * m_fSpeed * DT;

	Transform()->SetRelativePos(vMonPos.x, vMonPos.y, Transform()->GetRelativePos().y);
}

void CMon_Cubulon::DecideTraceAnimation()
{
	if (m_Angle <= 90.f && m_Angle >= 45.f)
		Animator2D()->Play(L"Back_R_Idle", true);
	else if (m_Angle <= 135.f && m_Angle >= 90.f)
		Animator2D()->Play(L"Back_L_Idle", true);
	else if ((m_Angle <= 181.f && m_Angle > 135.f) || (m_Angle <= -90.f && m_Angle >= -181.f))
		Animator2D()->Play(L"Left_Idle", true);
	else if ((m_Angle <= 45.f && m_Angle > 0.f) || (m_Angle <= 0.f && m_Angle >= -90.f))
		Animator2D()->Play(L"Right_Idle", true);
}
void CMon_Cubulon::IsAttackPossible()
{
	if (m_bAttack) return;

	m_fAttackTime += DT;
	if (m_fAttackTime >= 1.5f)
	{
		m_bAttack = true;
		m_fAttackTime = 0.f;
	}
}

void CMon_Cubulon::DecideAttackAnimation()
{
	if ((m_BehaviorAngle <= 90.f && m_BehaviorAngle >= 0.f) || (m_BehaviorAngle >= -90.f && m_BehaviorAngle <= 0.f))
	{
		if (m_fTime <= 1.f && m_fTime >= 0.f)
			Animator2D()->Play(L"Right_Atk", false);
		else
		{
			Animator2D()->FindAnim(L"Right_Atk")->Reset();
			ChangeAttackToIdle();
		}
	}
	else if ((m_BehaviorAngle <= 181.f && m_BehaviorAngle > 90.f) || (m_BehaviorAngle >= -181.f && m_BehaviorAngle < 90.f))
	{
		if (m_fTime <= 1.f && m_fTime >= 0.f)
			Animator2D()->Play(L"Left_Atk", false);
		else
		{
			Animator2D()->FindAnim(L"Left_Atk")->Reset();
			ChangeAttackToIdle();
		}
	}
}

void CMon_Cubulon::ChangeAttackToIdle()
{
	m_BehaviorAngle = 0.f;
	m_fTime = 0.f;
	m_bAttack = false;
	ChangeState(MON_STATE::IDLE);
}


void CMon_Cubulon::CheckAndTransitionHitToIdle()
{
	m_fTime += DT;
	if (m_fTime > 0.5f)
	{
		Animator2D()->FindAnim(L"Right_Hit")->Reset();
		Animator2D()->FindAnim(L"Left_Hit")->Reset();
		m_fTime = 0.f;
		ChangeState(MON_STATE::IDLE);
	}
}

void CMon_Cubulon::DecideHitAnimation()
{
	if ((m_Angle >= 0.f && m_Angle <= 90.f) || (m_Angle <= 0.f && m_Angle >= -90.f))
		Animator2D()->Play(L"Right_Hit", true);
	else if ((m_Angle >= 90.f && m_Angle <= 181.f) || (m_Angle <= -90.f && m_Angle >= -181.f))
		Animator2D()->Play(L"Left_Hit", true);
}



void CMon_Cubulon::TimeDestroyCheck(float Time)
{
	m_fTime += DT;
	if (m_fTime> Time)
		GetOwner()->Destroy();
}

void CMon_Cubulon::CheckDeadSound()
{
	if (!m_bSound)
	{
		m_bSound = true;
		Ptr<CSound> pSound = CResMgr::GetInst()->Load<CSound>(L"sound\\cubulon\\Dead.wav", L"sound\\cubulon\\Dead.wav");
		pSound->Play(1, 0.12f, false);
	}
}


void CMon_Cubulon::CheckDeath()
{
	if (m_HP <= 0 && GetState() != MON_STATE::DEAD)
	{
		m_fTime = 0.f;
		ChangeState(MON_STATE::DEAD);
	}
}

void CMon_Cubulon::CheckHit()
{
	if (GetState() == MON_STATE::IDLE || GetState() == MON_STATE::TRACE)
		ChangeState(MON_STATE::HIT);
}

void CMon_Cubulon::Shoot()
{
	if (!m_bAttack) return;
	m_bAttack = false;

	Ptr<CPrefab> pPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\M_Bullet.pref");
	if(pPrefab !=nullptr)
		CreateAllBullet(pPrefab);
}

void CMon_Cubulon::CreateAllBullet(Ptr<CPrefab>& pPrefab)
{
	CGameObject* pObj;

	for (int i = 0; i <= 4; ++i)
	{
		pObj = pPrefab->Instantiate();
		CreateBullet(pObj, GetOwner()->Transform()->GetWorldPos(), 200.f, Vec3(1.f - (i * 0.25f), (i * 0.25f), 0.f), i);
		AddCreateObjectEvent(pObj, 6);
	}

	for (int i = 1; i <= 4; ++i)
	{
		pObj = pPrefab->Instantiate();
		CreateBullet(pObj, GetOwner()->Transform()->GetWorldPos(), 200.f, Vec3(1.f - (i * 0.25f), (i * 0.25f), 0.f), i);
		AddCreateObjectEvent(pObj, 6);
	}

	for (int i = 0; i <= 4; ++i)
	{
		pObj = pPrefab->Instantiate();
		CreateBullet(pObj, GetOwner()->Transform()->GetWorldPos(), 200.f, Vec3((i * 0.25f) - 1.f, (i * 0.25f), 0.f), i);
		AddCreateObjectEvent(pObj, 6);
	}

	for (int i = 1; i <= 4; ++i)
	{
		pObj = pPrefab->Instantiate();
		CreateBullet(pObj, GetOwner()->Transform()->GetWorldPos(), 200.f, Vec3((i * 0.25f) - 1.f, -(i * 0.25f), 0.f), i);
		AddCreateObjectEvent(pObj, 6);
	}
}

void CMon_Cubulon::CreateBullet(CGameObject* pBullet, const Vec3& Pos, float Speed, const Vec3& Dir,int Count)
{
	pBullet->Transform()->SetRelativePos(GetOwner()->Transform()->GetWorldPos());
	CM_Bullet* pBulletScript = (CM_Bullet*)CScriptMgr::GetScript((UINT)SCRIPT_TYPE::M_BULLET);
	pBulletScript->SetSpeed(200.f);
	pBulletScript->SetDir(Vec3(1.f - (Count * 0.25f), (Count * 0.25f), 0.f));
	pBullet->AddComponent(pBulletScript);
}
