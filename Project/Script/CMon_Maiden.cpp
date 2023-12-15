#include "pch.h"
#include "CMon_Maiden.h"

#include <Engine/CAnimator2D.h>
#include <Engine/CAnimation2D.h>

#include <Engine/CEventMgr.h>
#include "CMaiden_Bullet.h"

CMon_Maiden::CMon_Maiden()
	:CMonsterScript((int)SCRIPT_TYPE::MON_MAIDEN)
	, m_bAtk(true)
	, m_bAtk2(true)
	, i_Dir(-3.14f)
	, m_Sound(false)
	, m_WalkSound(false)
	, m_WalkTime(0.f)
{
	m_HP = 150;
	m_DetectRange = 600.f;
	m_AttackRange = 300.f;
	m_fSpeed = 100.f;
}

CMon_Maiden::~CMon_Maiden()
{
}

void CMon_Maiden::Idle()
{
	if (m_fDistance < m_DetectRange)
	{
		ChangeState(MON_STATE::TRACE);
	}
	Animator2D()->Play(L"Maiden_Idle", true);
}

void CMon_Maiden::Trace()
{
	CheckWalkSound();
	Move();
	IsAttackPossible();
	CanAttack();
	Animator2D()->Play(L"Maiden_Trace", true);
}

void CMon_Maiden::CanAttack()
{
	if (m_bAttack)
	{
		if (m_fDistance < m_AttackRange)
		{
			m_BehaviorAngle = m_Angle;
			ChangeState(MON_STATE::ATK);
		}
	}
}

void CMon_Maiden::IsAttackPossible()
{
	if (m_bAttack) return;
	m_fAttackTime += DT;
	if (m_fAttackTime >= 2.5f)
	{
		m_bAtk = true;
		m_bAtk2 = true;
		m_bAttack = true;
		m_fAttackTime = 0.f;
	}
}

void CMon_Maiden::Move()
{
	Vec3 vPlayerPos = CSceneMgr::GetInst()->GetCurScene()->GetPlayerPos();
	Vec3 vMonPos = Transform()->GetWorldPos();
	Vec2 vDir = GetDirBetweenObjects(vMonPos, vPlayerPos);

	vMonPos += Vec3(vDir.x, vDir.y, 0) * m_fSpeed * DT;
	Transform()->SetRelativePos(vMonPos.x, vMonPos.y, Transform()->GetRelativePos().z);
}

void CMon_Maiden::CheckWalkSound()
{
	m_WalkTime += DT;
	if (m_WalkTime > 0.2f)
	{
		m_WalkSound = true;
	}
	if (m_WalkSound)
	{
		m_WalkTime = 0.f;
		m_WalkSound = false;
		Ptr<CSound> pSound = CResMgr::GetInst()->Load<CSound>(L"sound\\leadmaiden\\Walk.wav", L"sound\\leadmaiden\\Walk.wav");
		pSound->Play(1, 0.03f, true);
	}

}

void CMon_Maiden::Attack()
{
	m_bAttack = false;
	m_fTime += DT;

	AttackSound();
	Attack1();
	Attack2();
	IsFinishAttack();
}

void CMon_Maiden::IsFinishAttack()
{
	if (m_fTime >= 0.f && m_fTime <= 3.0f)
	{
		Animator2D()->Play(L"Maiden_Atk", false);
	}
	else
	{
		m_fTime = 0.f;
		i_Dir = -3.14f;
		m_Sound = false;
		Animator2D()->FindAnim(L"Maiden_Atk")->Reset();
		ChangeState(MON_STATE::IDLE);
	}
}

void CMon_Maiden::AttackSound()
{
	if (!m_Sound)
	{
		Ptr<CSound> pSound = CResMgr::GetInst()->Load<CSound>(L"sound\\leadmaiden\\Open.wav", L"sound\\leadmaiden\\Open.wav");
		pSound->Play(1, 0.07f, true);
		m_Sound = true;
	}
}

void CMon_Maiden::Attack2()
{
	if (m_fTime > 1.1f && m_bAtk2)
	{
		m_bAtk2 = false;
		Shoot2();
	}
}

void CMon_Maiden::Attack1()
{
	if (m_fTime > 0.5f && m_bAtk)
	{
		m_bAtk = false;
		Shoot();
	}
}

void CMon_Maiden::Dead()
{
	m_fTime += DT;
	Animator2D()->Play(L"Maiden_Dead", true);
	ShowDeathAnimation();
}

void CMon_Maiden::ShowDeathAnimation()
{
	if (m_fTime > 0.4f)
	{
		Vec3 pTrans = Transform()->GetRelativePos();
		CGameObject* pObj = PrefabInstantiate(L"prefab\\Maiden_Dead.pref");

		pObj->Transform()->SetRelativePos(Vec3(pTrans.x, pTrans.y, 100.f));
		pObj->Transform()->SetRelativeScale(Vec3(250.f, 250.f, 100.f));

		AddCreateObjectEvent(pObj, 0);
		GetOwner()->Destroy();
	}
}

void CMon_Maiden::OnCollisionEnter(CGameObject* _OtherObject)
{
	HitBaseBullet(_OtherObject);
	HitIceBullet(_OtherObject);
}

void CMon_Maiden::HitIceBullet(CGameObject* _OtherObject)
{
	if (_OtherObject->GetName() == L"Ice_Bullet")
	{
		m_HP -= 20;
		m_IceCheck = 1;

		if (m_HP <= 0 && GetState() != MON_STATE::DEAD)
		{
			Ptr<CSound> pSound = CResMgr::GetInst()->Load<CSound>(L"sound\\leadmaiden\\Dead.wav", L"sound\\leadmaiden\\Dead.wav");
			pSound->Play(1, 0.12f, true);
			m_fTime = 0.f;
			ChangeState(MON_STATE::DEAD);
		}
	}
}

void CMon_Maiden::HitBaseBullet(CGameObject* _OtherObject)
{
	if (_OtherObject->GetName() == L"P_Bullet" && !(GetState() == MON_STATE::DEAD))
	{
		m_HP -= 15;
		if (m_HP <= 0 && GetState() != MON_STATE::DEAD)
		{
			Ptr<CSound> pSound = CResMgr::GetInst()->Load<CSound>(L"sound\\leadmaiden\\Dead.wav", L"sound\\leadmaiden\\Dead.wav");
			pSound->Play(1, 0.12f, true);
			m_fTime = 0.f;
			ChangeState(MON_STATE::DEAD);
		}
	}
}

void CMon_Maiden::Shoot()
{
	Vec3 pTrans = Transform()->GetRelativePos();

	CGameObject* pObj;
	for (int i = 0; i < 10; ++i)
	{
		pObj = PrefabInstantiate(L"prefab\\Maiden_Bullet.pref");
		pObj->Transform()->SetRelativePos(Vec3(pTrans.x, pTrans.y, 0.f));

		i_Dir += 0.628f;
		float cos = cosf(i_Dir);
		float sin = sinf(i_Dir);
		Vec3 vDir = Vec3(cos, sin, 0.f); vDir.Normalize();

		CMaiden_Bullet* pBullet = (CMaiden_Bullet*)CScriptMgr::GetScript((UINT)SCRIPT_TYPE::MAIDEN_BULLET);
		pBullet->SetDir(vDir);
		pBullet->SetSpeed(200.f);
		pObj->AddComponent(pBullet);

		AddCreateObjectEvent(pObj, 6);
	}
}

void CMon_Maiden::Shoot2()
{
	i_Dir = 0.4f;
	Vec3 pTrans = Transform()->GetRelativePos();

	CGameObject* pObj;

	for (int i = 0; i < 10; ++i)
	{
		pObj = PrefabInstantiate(L"prefab\\Maiden_Bullet.pref");
		pObj->Transform()->SetRelativePos(Vec3(pTrans.x, pTrans.y, 0.f));

		i_Dir += 0.628f;
		float cos = cosf(i_Dir);
		float sin = sinf(i_Dir);
		Vec3 vDir = Vec3(cos, sin, 0.f); vDir.Normalize();

		CMaiden_Bullet* pBullet = (CMaiden_Bullet*)CScriptMgr::GetScript((UINT)SCRIPT_TYPE::MAIDEN_BULLET);
		pBullet->SetDir(vDir);
		pBullet->SetSpeed(200.f);
		pObj->AddComponent(pBullet);

		AddCreateObjectEvent(pObj, 6);

	}
}
