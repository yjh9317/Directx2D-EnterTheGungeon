#include "pch.h"
#include "CP_Bullet.h"

#include "CMonsterScript.h"
#include "CWallScript.h"
#include <Engine/CCollider2D.h>
#include "CMovementScript.h"

#include "CDoorScript.h"

CP_Bullet::CP_Bullet()
	: CScript((int)SCRIPT_TYPE::P_BULLET)
	, m_Dir{}
	, m_Speed(0.f)
	, m_fTime(0.f)
{
}

CP_Bullet::~CP_Bullet()
{
}


void CP_Bullet::update()
{
	TimeDestroyCheck(2.f);
	SetPosition();
}

void CP_Bullet::SetPosition()
{
	Vec3 vTrans = Transform()->GetRelativePos();
	vTrans += DT * m_Dir * m_Speed;
	Transform()->SetRelativePos(vTrans);
}

void CP_Bullet::TimeDestroyCheck(float Time)
{
	m_fTime += DT;
	if (m_fTime > Time)
	{
		GetOwner()->Destroy();
	}
}

void CP_Bullet::OnCollisionEnter(CGameObject* _OtherObject)
{
	if (_OtherObject->GetScript<CMonsterScript>())
	{
		CMonsterScript* pScript = _OtherObject->GetScript<CMonsterScript>();
		if (pScript->GetState() != MON_STATE::DEAD)
		{
			GetOwner()->Destroy();
		}
	}
	else if (_OtherObject->GetScript<CWallScript>() || _OtherObject->GetScript<CDoorScript>())
	{
		Ptr<CSound> pSound = CResMgr::GetInst()->Load<CSound>(L"sound\\magnum\\Wall.wav", L"sound\\magnum\\Wall.wav");
		pSound->Play(1, 0.07f, true);
		GetOwner()->Destroy();

		//CMovementScript* pScript = GetOwner()->GetScript<CMovementScript>();

		//if (nullptr == pScript)
		//	return;

		//Vec3 PrevPos = pScript->GetPrevPos();

		//float TransXpos = _OtherObject->Transform()->GetWorldPos().x;
		//float TransYpos = _OtherObject->Transform()->GetWorldPos().y;
		//float ColWidth = _OtherObject->Collider2D()->GetOffsetScale().x / 2.f;
		//float ColHeight = _OtherObject->Collider2D()->GetOffsetScale().y / 2.f;

		//float WallLeft = TransXpos - ColWidth;
		//float WallRight = TransXpos + ColWidth;
		//float WallUp = TransYpos + ColHeight;
		//float WallDown = TransYpos - ColHeight;

		//// 벽의 Collider Offset값도 추가해야함 , Table에서 해놨ㅇ므.
		//Vec3 vPos = Transform()->GetWorldPos();
		//Vec2 vColOffset = Collider2D()->GetOffsetPos();
		//Vec2 vColScale = Collider2D()->GetOffsetScale();

		//float Left = vPos.x - vColScale.x / 2.f + vColOffset.x;
		//float Right = vPos.x + vColScale.x / 2.f + vColOffset.x;
		//float Up = vPos.y + vColScale.y / 2.f + vColOffset.y;
		//float Down = vPos.y - vColScale.y / 2.f + vColOffset.y;

		//float PrevUp = PrevPos.y + vColScale.y / 2.f + vColOffset.y;
		//float PrevDown = PrevPos.y - vColScale.y / 2.f + vColOffset.y;
		//float PrevLeft = PrevPos.x - vColScale.x / 2.f + vColOffset.x;
		//float PrevRight = PrevPos.x + vColScale.x / 2.f + vColOffset.x;


		//// 벽의 위에 있을 떄
		//if ((Down <= WallUp) && PrevDown > WallUp)
		//{
		//
		//}
		//// 벽의 아래에 있을 때
		//else if ((Up >= WallDown) && PrevUp < WallDown)
		//{
		//
		//}

		////벽의 오른쪽에 있을때
		//if ((Left <= WallRight) && PrevLeft > WallRight)
		//{

		//}
		////벽의 왼쪽에 있을 때
		//else if ((Right >= WallLeft) && PrevRight < WallLeft)
		//{

		//}
	}
	else if (_OtherObject->GetName() == L"Skull_Bullet")
	{
		GetOwner()->Destroy();
	}
}