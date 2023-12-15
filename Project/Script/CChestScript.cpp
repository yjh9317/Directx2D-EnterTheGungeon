#include "pch.h"
#include "CChestScript.h"

#include <Engine/CAnimator2D.h>
#include "CMovementScript.h"
#include <Engine/CCollider2D.h>
#include <Engine/CEventMgr.h>

#include "CIceGunScript.h"

void CChestScript::Check()
{
}

CChestScript::CChestScript()
	:CScript((int)SCRIPT_TYPE::CHESTSCRIPT)
	, m_type(CHEST_TYPE::Idle)
	, m_fTime(0.f)
	, m_bOpen(false)
	, m_Gun(false)
{
}

CChestScript::~CChestScript()
{
}


void CChestScript::update()
{
	AnimationBasedOnState();

	if (m_type == CHEST_TYPE::Idle)
	{
		if (KEY_TAP(KEY::E))
		{
			TryOpen();
		}
	}
	else if (m_type == CHEST_TYPE::Open)
	{
		m_fTime += DT;

		if (m_fTime > 0.3f)
		{
			CreateIceGun();
		}
		else if (m_fTime > 0.5f)
		{
			SetOpened();
		}
	}
}

void CChestScript::SetOpened()
{
	m_type = CHEST_TYPE::Opened;
	m_fTime = 0.f;
}

void CChestScript::CreateIceGun()
{
	if (!m_Gun)
	{
		m_Gun = true;
		CGameObject* pObj = PrefabInstantiate(L"prefab\\IceGun.pref");
		pObj->Transform()->SetRelativePos(Vec3(GetOwner()->Transform()->GetWorldPos().x + 5.f, GetOwner()->Transform()->GetWorldPos().y, 100.f));

		CIceGunScript* pScript = new CIceGunScript;
		pObj->AddComponent(pScript);

		AddCreateObjectEvent(pObj, 0);
	}
}

void CChestScript::TryOpen()
{
	Vec3 vPlayerPos = CSceneMgr::GetInst()->GetCurScene()->GetPlayerPos();
	Vec3 vPos = Transform()->GetWorldPos(); vPos.y += 20.f;
	CalcDistBetweenObjects(vPlayerPos, vPos);
	float distance = CalcDistBetweenObjects(vPlayerPos, vPos);
	if (distance <= 100.f && m_type == CHEST_TYPE::Idle)
	{
		Ptr<CSound> pSound = CResMgr::GetInst()->Load<CSound>(L"sound\\item\\ChestOpen.wav", L"sound\\item\\ChestOpen.wav");
		pSound->Play(1, 0.2f, true);
		m_type = CHEST_TYPE::Open;
	}
}

void CChestScript::AnimationBasedOnState()
{
	switch (m_type)
	{
	case CHEST_TYPE::Idle:
		Animator2D()->Play(L"Chest_Idle", false);
		break;
	case CHEST_TYPE::Open:
		Animator2D()->Play(L"Chest_Open", false);
		break;
	case CHEST_TYPE::Opened:
		Animator2D()->Play(L"Chest_Opened", false);
		break;
	}
}

void CChestScript::OnCollisionEnter(CGameObject* _OtherObject)
{
	CMovementScript* pScript = _OtherObject->GetScript<CMovementScript>();

	if (nullptr == pScript)
		return;

	Vec3 PrevPos = pScript->GetPrevPos();

	float TransXpos = Transform()->GetWorldPos().x;
	float TransYpos = Transform()->GetWorldPos().y;

	float ColWidth = Collider2D()->GetOffsetScale().x / 2.f;
	float ColHeight = Collider2D()->GetOffsetScale().y / 2.f;

	float ColXPos = Collider2D()->GetOffsetPos().x;
	float ColYPos = Collider2D()->GetOffsetPos().y;

	float Left = TransXpos - ColWidth + ColXPos;
	float Right = TransXpos + ColWidth + ColXPos;
	float Up = TransYpos + ColHeight + ColYPos;
	float Down = TransYpos - ColHeight + ColYPos;

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
	if ((OtherDown <= Up)  && OtherPrevDown > Up)
	{
		_OtherObject->Transform()->SetRelativePos(Vec3(OtherPos.x, TransYpos + ColHeight + OtherColScale.y / 2.f - OtherColOffset.y + 1.f + ColYPos, OtherPos.z));
	}
	// 벽의 아래에 있을 때
	else if ((OtherUp >= Down)  && OtherPrevUp < Down)
	{
		_OtherObject->Transform()->SetRelativePos(Vec3(OtherPos.x, TransYpos - ColHeight - OtherColScale.y / 2.f - OtherColOffset.y - 1.f + ColYPos, OtherPos.z));
	}

	//벽의 오른쪽에 있을때
	if ((OtherLeft <= Right)  && OtherPrevLeft > Right) 
	{
		//if (OtherPos.y < TransYpos)
		_OtherObject->Transform()->SetRelativePos(Vec3(TransXpos + ColWidth + OtherColScale.x / 2.f + OtherColOffset.x + 1.f + ColXPos, OtherPos.y, OtherPos.z));
	}
	//벽의 왼쪽에 있을 때
	else if ((OtherRight >= Left)  && OtherPrevRight < Left)
	{
		_OtherObject->Transform()->SetRelativePos(Vec3(TransXpos - ColWidth - OtherColScale.x / 2.f - OtherColOffset.x - 1.f + ColXPos, OtherPos.y, OtherPos.z));
	}
}