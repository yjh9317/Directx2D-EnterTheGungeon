#include "pch.h"
#include "CM_Bullet.h"

#include "CWallScript.h"
#include "CDoorScript.h"
#include "CPlayerStateScript.h"

CM_Bullet::CM_Bullet()
	: CScript((int)SCRIPT_TYPE::M_BULLET)
	, m_vDir{}
	, m_fSpeed(0.f)
	, m_fTime(0.f)
	, m_bWait(false)
	, m_fWaitTime(0.f)
{
}

CM_Bullet::~CM_Bullet()
{

}



void CM_Bullet::update()
{
	m_fTime += DT;
	IsWait();
	TimeDestroyCheck();
	UpdatePosition();
}

void CM_Bullet::UpdatePosition()
{
	Vec3 vTrans = Transform()->GetRelativePos();
	vTrans += DT * m_vDir * m_fSpeed;
	Transform()->SetRelativePos(vTrans);
}

void CM_Bullet::IsWait()
{
	if (m_bWait)
	{
		if (m_fTime > m_fWaitTime)
		{
			m_bWait = false;
			m_fTime = 0.f;
		}
	}
}

void CM_Bullet::TimeDestroyCheck()
{
	if (m_fTime > 2.5f && !m_bWait)
	{
		GetOwner()->Destroy();
	}
}

void CM_Bullet::OnCollisionEnter(CGameObject* _OtherObject)
{
	if (_OtherObject->GetName() == L"Player")
	{
		CPlayerStateScript* pScript = _OtherObject->GetScript<CPlayerStateScript>();

		if(pScript->GetPlayerType() == PLAYER_TYPE::IDLE || pScript->GetPlayerType() == PLAYER_TYPE::WALK)
			GetOwner()->Destroy();
	}
	else if (_OtherObject->GetScript<CWallScript>() || _OtherObject->GetScript<CDoorScript>())
		GetOwner()->Destroy();
}
