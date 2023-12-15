#include "pch.h"
#include "CMaiden_Bullet.h"

#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>

#include "CPlayerStateScript.h"
#include "CWallScript.h"

CMaiden_Bullet::CMaiden_Bullet()
	:CScript((int)SCRIPT_TYPE::MAIDEN_BULLET)
	, m_vDir{}
	, m_fSpeed(100.f)
	, m_fTime(0.f)
	, m_bWait(false)
	, m_fWaitTime(0.f)
{
}

CMaiden_Bullet::~CMaiden_Bullet()
{
}


void CMaiden_Bullet::update()
{
	IsDestroy(4.5f);
	SetPosition();
	SetRotation();
}

void CMaiden_Bullet::SetRotation()
{
	float m_Angle = atan2f(m_vDir.y, m_vDir.x);
	Vec3 vRot = Transform()->GetRelativeRotation();
	vRot.z = m_Angle;
	Transform()->SetRelativeRotation(vRot);
}

void CMaiden_Bullet::SetPosition()
{
	Vec3 vTrans = Transform()->GetRelativePos();
	vTrans += DT * m_vDir * m_fSpeed;
	Transform()->SetRelativePos(vTrans);
}

void CMaiden_Bullet::IsDestroy(float Time)
{
	m_fTime += DT;
	if (m_fTime > Time)
	{
		GetOwner()->Destroy();
	}
}

void CMaiden_Bullet::OnCollisionEnter(CGameObject* _OtherObject)
{
	if (_OtherObject->GetName() == L"Player")
	{
		CPlayerStateScript* pScript = _OtherObject->GetScript<CPlayerStateScript>();

		if (pScript->GetPlayerType() == PLAYER_TYPE::IDLE || pScript->GetPlayerType() == PLAYER_TYPE::WALK)
			GetOwner()->Destroy();
	}
	else if (_OtherObject->GetScript<CWallScript>())
	{
		Vec3 vPlayerPos = CSceneMgr::GetInst()->GetCurScene()->GetPlayerPos();
		Vec3 vPos = Transform()->GetWorldPos();
		m_fSpeed = 300.f;
		m_vDir = vPlayerPos - vPos;	m_vDir.Normalize();
	}
}

