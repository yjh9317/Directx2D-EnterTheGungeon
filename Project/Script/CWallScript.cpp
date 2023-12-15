#include "pch.h"
#include "CWallScript.h"

#include <Engine/CCollider2D.h>
#include "CMovementScript.h"

#include <Engine/CCollider2D.h>
#include <Engine/CTileMap.h>


CWallScript::CWallScript()
	: CScript((int)SCRIPT_TYPE::WALLSCRIPT)
{
}

CWallScript::~CWallScript()
{
}

void CWallScript::update()
{

}

void CWallScript::OnCollisionEnter(CGameObject* _OtherObject)
{
	CMovementScript* pScript = _OtherObject->GetScript<CMovementScript>();

	if (nullptr == pScript)	return;

	Vec3 PrevPos = pScript->GetPrevPos();

	float TransXpos = Transform()->GetWorldPos().x;
	float TransYpos = Transform()->GetWorldPos().y;
	float ColWidth = Collider2D()->GetOffsetScale().x / 2.f;
	float ColHeight = Collider2D()->GetOffsetScale().y / 2.f;

	float WallLeft = TransXpos - ColWidth;
	float WallRight = TransXpos + ColWidth;
	float WallUp = TransYpos + ColHeight;
	float WallDown = TransYpos - ColHeight;

	// 벽의 Collider Offset값도 추가해야함 , Table에서 해놨ㅇ므.
	Vec3 OtherPos = _OtherObject->Transform()->GetWorldPos();
	Vec2 OtherColOffset = _OtherObject->Collider2D()->GetOffsetPos();
	Vec2 OtherColScale = _OtherObject->Collider2D()->GetOffsetScale();

	float OtherLeft = OtherPos.x - OtherColScale.x / 2.f + OtherColOffset.x;
	float OtherRight = OtherPos.x + OtherColScale.x / 2.f + OtherColOffset.x;
	float OtherUp = OtherPos.y + OtherColScale.y / 2.f + OtherColOffset.y;
	float OtherDown = OtherPos.y - OtherColScale.y / 2.f + OtherColOffset.y;

	float OtherPrevUp = PrevPos.y + OtherColScale.y / 2.f + OtherColOffset.y;
	float OtherPrevDown = PrevPos.y - OtherColScale.y / 2.f + OtherColOffset.y;
	float OtherPrevLeft = PrevPos.x - OtherColScale.x / 2.f + OtherColOffset.x;
	float OtherPrevRight = PrevPos.x + OtherColScale.x / 2.f + OtherColOffset.x;


	// 벽의 위에 있을 떄
	if((OtherDown <= WallUp) && OtherPrevDown > WallUp)
	{
		_OtherObject->Transform()->SetRelativePos(Vec3(OtherPos.x, TransYpos + ColHeight + OtherColScale.y/2.f - OtherColOffset.y +2.f, OtherPos.z));
	}
	// 벽의 아래에 있을 때
	else if ((OtherUp >= WallDown) && OtherPrevUp <WallDown)
	{
		_OtherObject->Transform()->SetRelativePos(Vec3(OtherPos.x, TransYpos - ColHeight - OtherColScale.y / 2.f - OtherColOffset.y - 2.f, OtherPos.z));
	}

	//벽의 오른쪽에 있을때
	if ((OtherLeft <= WallRight) && OtherPrevLeft > WallRight)
	{
		//if (OtherPos.y < TransYpos)
		_OtherObject->Transform()->SetRelativePos(Vec3(TransXpos + ColWidth + OtherColScale.x / 2.f + OtherColOffset.x + 2.f, OtherPos.y, OtherPos.z));
	}
	//벽의 왼쪽에 있을 때
	else if ((OtherRight >= WallLeft) && OtherPrevRight <WallLeft)
	{
		_OtherObject->Transform()->SetRelativePos(Vec3(TransXpos - ColWidth - OtherColScale.x / 2.f - OtherColOffset.x -2.f, OtherPos.y, OtherPos.z));
	}
}

