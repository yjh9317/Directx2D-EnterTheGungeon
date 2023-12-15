#include "pch.h"
#include "CBoss_Bullet.h"

#include "CWallScript.h"

#include "CPlayerStateScript.h"

CBoss_Bullet::CBoss_Bullet()
	:CScript((int)SCRIPT_TYPE::BOSS_BULLET)
	, m_Dir{}
	, m_Speed(100)
	, m_fTime(0.f)
{
}

CBoss_Bullet::~CBoss_Bullet()
{
}

void CBoss_Bullet::update()
{
	m_fTime += DT;

	if (!m_bStop)
	{
		SetPosition();
		checkIfTimeIsUp(2.5f);
	}
	else { CheckIsStop(); }
}

void CBoss_Bullet::checkIfTimeIsUp(float Time)
{
	if (m_fTime > Time)
	{
		GetOwner()->Destroy();
	}
}

void CBoss_Bullet::CheckIsStop()
{
	if (m_fTime >= m_fWaitTime) 
	{
		m_bStop = false;
		m_fTime = 0.f;
	}	
}

void CBoss_Bullet::SetPosition()
{
	Vec3 vTrans = Transform()->GetRelativePos();
	vTrans += DT * m_Dir * m_Speed;
	Transform()->SetRelativePos(vTrans);
}

void CBoss_Bullet::OnCollisionEnter(CGameObject* _OtherObject)
{
	if (_OtherObject->GetName() == L"Player")
	{
		CPlayerStateScript* pScript = _OtherObject->GetScript<CPlayerStateScript>();

		if (pScript->GetPlayerType() == PLAYER_TYPE::IDLE || pScript->GetPlayerType() == PLAYER_TYPE::WALK)
			GetOwner()->Destroy();
	}
	else if (_OtherObject->GetScript<CWallScript>())
	{
		GetOwner()->Destroy();
	}
}