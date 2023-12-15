#include "pch.h"
#include "CBlankBulletEffect.h"

#include <Engine/CAnimator2D.h>



CBlankBulletEffect::CBlankBulletEffect()
	:CScript((int)SCRIPT_TYPE::BLANKBULLETEFFECT)
	, m_fTime(0.f)
{
}

CBlankBulletEffect::~CBlankBulletEffect()
{
}

void CBlankBulletEffect::start()
{
	Animator2D()->Play(L"BlankBulletEffect", false);
}

void CBlankBulletEffect::update()
{
	m_fTime += DT;
	if (m_fTime > 0.75f)
		GetOwner()->Destroy();
}
