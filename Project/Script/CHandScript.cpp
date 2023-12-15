#include "pch.h"
#include "CHandScript.h"

#include "CPlayerStateScript.h"

CHandScript::CHandScript()
	: CScript((int)SCRIPT_TYPE::HANDSCRIPT)
	, m_MousePos{}
	, m_Pos{}
{
	m_Pos = Vec3(17.f, -28.f, 0.f);
}

CHandScript::~CHandScript()
{
}

void CHandScript::start()
{
	Transform()->SetIgnoreParentScale(true);
}

void CHandScript::update()
{
	if (nullptr == GetOwner()->GetParent())
		return;

	SetPos();
	SetScale();
}

void CHandScript::SetPos()
{
	Vec2 Resolution = CDevice::GetInst()->GetRenderResolution();
	Vec2 MousePos = CKeyMgr::GetInst()->GetMousePos();
	m_MousePos = Vec2(MousePos.x - Resolution.x / 2.f, -MousePos.y + Resolution.y / 2.f);
	Vec2 Dir = Vec2(m_MousePos.x, -m_MousePos.y);

	if (Dir.x >= 0.f)
		Transform()->SetRelativePos(Vec3(m_Pos.x, m_Pos.y, m_Pos.z));
	else if (Dir.x < 0.f)
		Transform()->SetRelativePos(Vec3(-m_Pos.x, m_Pos.y, m_Pos.z));
}

void CHandScript::SetScale()
{
	vector<CScript*> vScripts = GetOwner()->GetParent()->GetScripts();

	if (vScripts.size() == 0) return;

	CPlayerStateScript* pStateScript = nullptr;

	for (int i = 0; i < vScripts.size(); ++i)
	{
		if (CScriptMgr::GetScriptName(vScripts[i]) == L"CPlayerStateScript")
		{
			pStateScript = (CPlayerStateScript*)vScripts[i];
		}
	}
	if (pStateScript == nullptr) return;
	if (!pStateScript->GetHandCheck())
		Transform()->Transform()->SetRelativeScale(Vec3(0.f, 0.f, 0.f));
	else
		Transform()->Transform()->SetRelativeScale(Vec3(23.f, 30.f, 0.f));
}
