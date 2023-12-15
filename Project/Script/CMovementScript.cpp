#include "pch.h"
#include "CMovementScript.h"

#include <Engine/CTileMap.h>

CMovementScript::CMovementScript()
	: CScript((int)SCRIPT_TYPE::MOVEMENTSCRIPT)
	, m_iRow(0)
	, m_iCol(0)
	, m_iTileIdx(0)
{
}

CMovementScript::~CMovementScript()
{
}


void CMovementScript::update()
{
	m_PrevPos = Transform()->GetWorldPos();
}

void CMovementScript::OnCollision(CGameObject* _OtherObj)
{

}