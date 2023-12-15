#include "pch.h"
#include "CMissileScript.h"
#include <Engine/CTimeMgr.h>
#include <Engine/CTransform.h>
#include <Engine/CEventMgr.h>
#include <Engine/CAnimator2D.h>

#include "CPlayerStateScript.h"
#include "CWallScript.h"
#include "CDoorScript.h"


CMissileScript::CMissileScript()
	: CScript((int)SCRIPT_TYPE::MISSILESCRIPT)
	, m_fSpeed(600.f)
	, m_fAccTime(0.f)
	, m_fExTime(0.f)
{
}

CMissileScript::~CMissileScript()
{
}

void CMissileScript::update()
{
	if (!m_bExplosion)
	{
		Animator2D()->Play(L"Idle", true);
		UpdatePosition();
		TimeOver();
		SetRotation();
	}
	else { Bomb();}
}

void CMissileScript::UpdatePosition()
{
	Vec3 vPos = Transform()->GetRelativePos();
	vPos += DT * m_fSpeed * m_vDir;
	Transform()->SetRelativePos(vPos);
}

void CMissileScript::TimeOver()
{
	m_fAccTime += DT;

	if (m_fAccTime >= 3.f)
		GetOwner()->Destroy();
}

void CMissileScript::Bomb()
{
	m_fExTime += DT;
	Animator2D()->Play(L"Bomb", false);
	if (m_fExTime >= 0.8f)
		GetOwner()->Destroy();
}

void CMissileScript::SetRotation()
{
	float m_Angle = atan2f(m_vDir.y, m_vDir.x);
	Vec3 vRot = Transform()->GetRelativeRotation();
	vRot.z = m_Angle;
	Transform()->SetRelativeRotation(vRot);
}

void CMissileScript::OnCollisionEnter(CGameObject* _pOtherObj)
{
	if (!m_bExplosion)
	{
		if (_pOtherObj->GetName() == L"Player")
		{
			CPlayerStateScript* pScript = _pOtherObj->GetScript<CPlayerStateScript>();
			if (pScript->GetPlayerType() == PLAYER_TYPE::IDLE || pScript->GetPlayerType() == PLAYER_TYPE::WALK)
				m_bExplosion = true;
		}
		else if (_pOtherObj->GetScript<CWallScript>() || _pOtherObj->GetScript<CDoorScript>())
			m_bExplosion = true;
	}
}

