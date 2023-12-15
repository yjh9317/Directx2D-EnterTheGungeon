#include "pch.h"
#include "CDoorScript.h"

#include <Engine/CAnimator2D.h>
#include <Engine/CAnimation2D.h>

#include <Engine/CCollider2D.h>
#include "CMovementScript.h"

CDoorScript::CDoorScript()
	: CScript((int)SCRIPT_TYPE::DOORSCRIPT)
	, m_type(DOOR_TYPE::Idle)
	, m_fTime(0.f)
	, m_Open(false)
{
}

CDoorScript::~CDoorScript()
{
}

void CDoorScript::start()
{
	Animator2D()->FindAnim(L"Door_Open")->Reset();
	Animator2D()->FindAnim(L"Door_Opened")->Reset();
	Animator2D()->FindAnim(L"Door_Close")->Reset();
}

void CDoorScript::update()
{
	AnimationBasedOnState();
	IsOpened();
}

void CDoorScript::IsOpened()
{
	if (m_type == DOOR_TYPE::Open)
	{
		m_fTime += DT;
		if (m_fTime > 0.5f)
		{
			m_type = DOOR_TYPE::Opened;
			m_fTime = 0.f;
		}
	}
}

void CDoorScript::AnimationBasedOnState()
{
	switch (m_type)
	{
	case DOOR_TYPE::Idle:
	{
		Animator2D()->Play(L"Door_Idle", false);
		break;
	}
	case DOOR_TYPE::Open:
	{
		Animator2D()->Play(L"Door_Open", false);
		break;
	}
	case DOOR_TYPE::Opened:
		Animator2D()->Play(L"Door_Opened", false);
		break;
	case DOOR_TYPE::Close:
		Animator2D()->Play(L"Door_Close", false);
		break;
	}
}

void CDoorScript::OnCollisionEnter(CGameObject* _OtherObject)
{
	if (_OtherObject->GetName() == L"Player" && m_type == DOOR_TYPE::Idle)
	{
		SetDoorOpen();
	}
	else if (m_type == DOOR_TYPE::Close)
	{
		CMovementScript* pScript = _OtherObject->GetScript<CMovementScript>();
		if (nullptr == pScript) return;

		IsCollide(pScript, _OtherObject);
	}
}

void CDoorScript::IsCollide(CMovementScript* pScript, CGameObject* _OtherObject)
{
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
	if ((OtherDown <= WallUp) && OtherPrevDown > WallUp)
	{
		_OtherObject->Transform()->SetRelativePos(Vec3(OtherPos.x, TransYpos + ColHeight + OtherColScale.y / 2.f - OtherColOffset.y + 2.f, OtherPos.z));
	}
	// 벽의 아래에 있을 때
	else if ((OtherUp >= WallDown) && OtherPrevUp < WallDown)
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
	else if ((OtherRight >= WallLeft) && OtherPrevRight < WallLeft)
	{
		_OtherObject->Transform()->SetRelativePos(Vec3(TransXpos - ColWidth - OtherColScale.x / 2.f - OtherColOffset.x - 2.f, OtherPos.y, OtherPos.z));
	}
}

void CDoorScript::SetDoorOpen()
{
	Ptr<CSound> pSound = CResMgr::GetInst()->Load<CSound>(L"sound\\item\\DoorOpen.wav", L"sound\\item\\DoorOpen.wav");
	pSound->Play(1, 0.2f, true);
	m_type = DOOR_TYPE::Open;
}
