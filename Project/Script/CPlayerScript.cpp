#include "pch.h"
#include "CPlayerScript.h"
#include "CMissileScript.h"

#include <Engine/CDevice.h>
#include <Engine/CAnimator2D.h>
#include <Engine/CKeyMgr.h>
#include "CPlayerStateScript.h"

CPlayerScript::CPlayerScript()
	: CScript((int)SCRIPT_TYPE::PLAYERSCRIPT)
	, m_fSpeed(300.f)
{
}

CPlayerScript::~CPlayerScript()
{


}

void CPlayerScript::start()
{

}

void CPlayerScript::update()
{


}

void CPlayerScript::lateupdate()
{
}

void CPlayerScript::OnCollision(CGameObject* _OtherObject)
{
}



void CPlayerScript::SaveToScene(FILE* _pFile)
{
}

void CPlayerScript::LoadFromScene(FILE* _pFile)
{
}