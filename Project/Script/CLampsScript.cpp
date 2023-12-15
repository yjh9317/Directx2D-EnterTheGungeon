#include "pch.h"
#include "CLampsScript.h"

#include <Engine/CAnimator2D.h>

CLampsScript::CLampsScript()
	:CScript((int)SCRIPT_TYPE::LAMPSSCRIPT)
{
}

CLampsScript::~CLampsScript()
{
}

void CLampsScript::start()
{
	Animator2D()->Play(L"Idle", true);
}

void CLampsScript::update()
{
}