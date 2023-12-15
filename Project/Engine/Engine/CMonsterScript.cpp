#include "pch.h"
#include "CMonsterScript.h"

#include <Engine/CEventMgr.h>

#include <Engine/CAnimator2D.h>
#include <Engine/CAnimation2D.h>

#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>
#include <Engine/CLayer.h>



CMonsterScript::CMonsterScript()
	: m_eState(MON_STATE::IDLE)
	, m_fTime(0.f)
	, m_fDistance(0.f)
	, m_fSpeed(0.f)
	, m_Atkable(true)
	, m_Dir(0.f, 0.f)
	, m_Angle(0.f)
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
	Vec3 vPos = pCurScene->GetPlayerPos();
	m_fDistance = (float)sqrtf(powf(vMonPos.x - vPos.x, 2.f) + powf(vMonPos.y - vPos.y, 2.f));
	m_Dir = Vec2(vPos.x - vMonPos.x, vPos.y-vMonPos.y);
	m_Dir.Normalize();
	const double Rad_to_deg = 57.2957951;
	m_Angle = atan2f(m_Dir.y, m_Dir.x);
	m_Angle *= Rad_to_deg;

	switch (m_eState)
	{
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
	case MON_STATE::SPECIAL:
		Special();
		break;
	case MON_STATE::DEAD:
		Dead();
		break;
	}
}

void CMonsterScript::ChangeState(MON_STATE _eState)
{
	if (m_eState == _eState)
		return;

	//이벤트 함수로 처리해야함.
	//tEventInfo info;
	//info.eType = EVENT_TYPE::CHANGE_AI_STATE;
	//info.lParam = (DWORD_PTR)this;
	//info.wParam = (DWORD_PTR)_eState;
	// lParam : FSM Adress, wParam : Next State Type

	//CEventMgr::GetInst()->AddEvent(info);

	m_eState = _eState;
}

void CMonsterScript::Idle()
{
	if (m_fDistance < m_DetectRange)
	{
		ChangeState(MON_STATE::TRACE);
	}
}

void CMonsterScript::Trace()
{
	if (m_fDistance < m_AttackRange)
	{
		m_BehaviorAngle = m_Angle;
		ChangeState(MON_STATE::ATK);
	}

	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	Vec2 vPlayerPos = Vec2(pCurScene->GetPlayerPos().x, pCurScene->GetPlayerPos().y);
	Vec2 vMonPos = Vec2(Transform()->GetWorldPos().x, Transform()->GetWorldPos().y);
	// 몬스터에서 플레이어를 향한 방향을 알아낸다.
	Vec2 vDir = vPlayerPos - vMonPos;
	vDir.Normalize();

	// 몬스터를 플레이어를 향해서 이동시킨다.
	vMonPos += vDir * m_fSpeed * DT;
	Transform()->SetRelativePos(vMonPos.x, vMonPos.y, Transform()->GetRelativePos().y);
}

void CMonsterScript::Attack()
{
	if (m_fDistance < m_DetectRange)
	{
		ChangeState(MON_STATE::TRACE);
	}
}
