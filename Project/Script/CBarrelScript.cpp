#include "pch.h"
#include "CBarrelScript.h"

#include <Engine/CAnimator2D.h>
#include <Engine/CAnimation2D.h>

#include "CMonsterScript.h"


CBarrelScript::CBarrelScript()
	:CScript((int)SCRIPT_TYPE::BARRELSCRIPT)
	, m_bBreak(false)
{
}

CBarrelScript::~CBarrelScript()
{
}


void CBarrelScript::start()
{
	Animator2D()->FindAnim(L"Break")->Reset();
}

void CBarrelScript::update()
{
	if (!m_bBreak)
		Animator2D()->Play(L"Idle", true);
	else
		Animator2D()->Play(L"Break", false);
}

void CBarrelScript::OnCollisionEnter(CGameObject* _OtherObj)
{
	if (_OtherObj->GetName() == L"Player" || _OtherObj->GetScript<CMonsterScript>() || _OtherObj->GetName() == L"P_Bullet" || _OtherObj->GetName() == L"M_Bullet")
	{
		if (!m_bBreak)
		{
			m_bBreak = true;
			Ptr<CSound> pSound = CResMgr::GetInst()->Load<CSound>(L"sound\\barrel\\Break.wav", L"sound\\barrel\\Break.wav");
			pSound->Play(1, 0.12f, true);
		}
	}
}
