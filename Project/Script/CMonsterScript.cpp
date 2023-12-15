#include "pch.h"
#include "CMonsterScript.h"

#include <Engine/CEventMgr.h>

#include <Engine/CAnimator2D.h>
#include <Engine/CAnimation2D.h>

#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>
#include <Engine/CLayer.h>

CMonsterScript::CMonsterScript()
	: CScript((int)SCRIPT_TYPE::MONSTERSCRIPT)
	, m_eState(MON_STATE::IDLE)
	, m_fTime(0.f)
	, m_fDistance(0.f)
	, m_fSpeed(0.f)
	, m_bAttack(true)
	, m_fAttackTime(0.f)
	, m_Dir(0.f,0.f)
	, m_Angle(0.f)
	, m_Atk(false)
	, m_IceCheck(0)
	, m_IceTime(0.f)
{
}

CMonsterScript::CMonsterScript(int _iScript)
	:CScript((int)_iScript)
	,m_eState(MON_STATE::IDLE)
	, m_fTime(0.f)
	, m_fDistance(0.f)
	, m_fSpeed(0.f)
	, m_bAttack(true)
	, m_fAttackTime(0.f)
	, m_Dir(0.f, 0.f)
	, m_Angle(0.f)
	, m_Atk(false)
	, m_IceCheck(0)
	, m_IceTime(0.f)
{
}

CMonsterScript::~CMonsterScript()
{
}

void CMonsterScript::start()
{

}

void CMonsterScript::update()
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	Vec3 vMonPos = Transform()->GetWorldPos();
	Vec3 vPlayerPos = pCurScene->GetPlayerPos();

	m_fDistance = CalcDistBetweenObjects(vMonPos, vPlayerPos);
	m_Angle = CalcAngleBetweenObjects(vMonPos, vPlayerPos);

	if (IsDead()) Dead();

	switch (m_eState)
	{
	case MON_STATE::START:
		StartState();
		break;
	case MON_STATE::IDLE:
		Idle();
		break;
	case MON_STATE::TRACE:
		Trace();
		break;
	case MON_STATE::ATK:
		Attack();
		break;
	case MON_STATE::HIT:
		Hit();
		break;
	case MON_STATE::SKILL1:
		Skill1();
		break;
	case MON_STATE::SKILL2:
		Skill2();
		break;
	case MON_STATE::SKILL3:
		Skill3();
		break;
	case MON_STATE::SKILL4:
		Skill4();
		break;
	case MON_STATE::SKILL5:
		Skill5();
		break;
	}
	IsFrozen();
}

void CMonsterScript::IsFrozen()
{
	GetOwner()->GetRenderComponent()->GetSharedMaterial()->SetScalarParam(L"IceCheck", &m_IceCheck);

	if (1 == m_IceCheck)
	{
		if (!m_bPrevSpeed)
		{
			m_bPrevSpeed = true;
			m_fPrevSpeed = m_fSpeed;
			m_fSpeed /= 2.f;
		}
		m_IceTime += DT;

		if (m_IceTime > 2.f)
		{
			m_IceTime = 0.f;
			m_IceCheck = 0;

			m_bPrevSpeed = false;
			m_fSpeed = m_fPrevSpeed;
		}
	}
}

bool CMonsterScript::IsDead() const
{
	return m_eState == MON_STATE::DEAD;
}

void CMonsterScript::ChangeState(MON_STATE _eState)
{
	tEventInfo evn = {};
	evn.eType = EVENT_TYPE::CHANGE_AI_STATE;
	evn.lParam = (DWORD_PTR)this;
	evn.wParam = (DWORD_PTR)_eState;

	CEventMgr::GetInst()->AddEvent(evn);
}

void CMonsterScript::SetState(MON_STATE _eState)
{
	if (m_eState == _eState)
		return;

	m_eState = _eState;
}

CGameObject* CMonsterScript::CreateBullet() const
{
	Ptr<CPrefab> pPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\M_Bullet.pref");
	return pPrefab->Instantiate();
}

