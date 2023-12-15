#include "pch.h"
#include "CMagnumScript.h"

#include <Engine/CEventMgr.h>
#include <Engine/CAnimator2D.h>
#include <Engine/CAnimation2D.h>
#include "CPlayerStateScript.h"
#include "CP_Bullet.h"



CMagnumScript::CMagnumScript()
	: CScript((int)SCRIPT_TYPE::MAGNUMSCRIPT)
	, m_fTime(0.f)
	, m_bIsAttack(true)
	, m_bRight(false)
	, m_ShootCheck(false)
	, m_ShootTime(0.f)
{
	m_Pos = Vec3(21.f, -20.f, 0.f);
}

CMagnumScript::~CMagnumScript()
{
}

void CMagnumScript::start()
{
	Transform()->SetIgnoreParentScale(true);


}

void CMagnumScript::SetScale()
{
	CPlayerStateScript* pStateScript = nullptr;

	const vector<CScript*>& vScripts = GetOwner()->GetParent()->GetScripts();
	for (int i = 0; i < vScripts.size(); ++i)
	{
		if (CScriptMgr::GetScriptName(vScripts[i]) == L"CPlayerStateScript")
		{
			pStateScript = (CPlayerStateScript*)vScripts[i];
		}
	}
	if (!pStateScript->GetHandCheck())
	{
		Transform()->SetRelativeScale(Vec3(0.f, 0.f, 0.f));
	}
	else
	{
		Transform()->SetRelativeScale(Vec3(100.f, 100.f, 0.f));
	}
}

void CMagnumScript::update()
{
	if (nullptr == GetOwner()->GetParent())
		return;

	CanAttack(0.4f);

	Vec2 Win_MousePos = GetMousePose();
	Vec3 Pos = Transform()->GetRelativePos();

	Vec2 Dir = GetDirBetweenObjects(Pos, Vec3(Win_MousePos.x, Win_MousePos.y, 0.f));
	float Radian = CalcRadianBetweenObjects(Pos, Vec3(Win_MousePos.x, Win_MousePos.y, 0.f));
	float Angle = CalcAngleBetweenObjects(Pos, Vec3(Win_MousePos.x, Win_MousePos.y, 0.f));

	SetScale();
	SetRotation(Radian, Angle);

	if (KEY_TAP(KEY::LBTN))	TryShoot(Dir);

}

void CMagnumScript::SetRotation(float Radian, float Angle)
{
	Vec3 vRot = Transform()->GetRelativeRotation();

	if (m_ShootCheck)
	{
		m_ShootTime += DT;

		if (m_ShootTime > 0.4f)
		{
			m_ShootTime = 0.f;
			m_ShootCheck = false;
			Animator2D()->FindAnim(L"R_MagnumShoot")->Reset();
			Animator2D()->FindAnim(L"L_MagnumShoot")->Reset();
		}

		if (m_bRight)
		{
			Animator2D()->Play(L"R_MagnumShoot", false);
			Transform()->SetRelativePos(Vec3(m_Pos.x, m_Pos.y, m_Pos.z));
			vRot.z = Radian;
		}
		else if (!m_bRight)
		{
			Animator2D()->Play(L"L_MagnumShoot", false);
			Transform()->SetRelativePos(Vec3(-m_Pos.x, m_Pos.y, m_Pos.z));
			vRot.z = Radian - XM_PI;
		}
	}
	else
	{
		if (m_bRight)
		{
			if (Angle > 110.f || Angle < -110.f)
				m_bRight = false;

			Animator2D()->Play(L"R_Magnum", true);
			Transform()->SetRelativePos(Vec3(m_Pos.x, m_Pos.y, m_Pos.z));
			vRot.z = Radian;
		}
		else if (!m_bRight)
		{
			if ((Angle < 70.f && Angle >0.f) || (Angle <0.f && Angle>-70.f))
				m_bRight = true;

			Animator2D()->Play(L"L_Magnum", true);
			Transform()->SetRelativePos(Vec3(-m_Pos.x, m_Pos.y, m_Pos.z));
			vRot.z = Radian - XM_PI;
		}
	}
	Transform()->SetRelativeRotation(vRot);
}

void CMagnumScript::TryShoot(Vec2& Dir)
{
	if (m_bIsAttack)
	{
		m_bIsAttack = false;
		Ptr<CSound> pSound = CResMgr::GetInst()->Load<CSound>(L"sound\\magnum\\Shot.wav", L"sound\\magnum\\Shot.wav");
		pSound->Play(1, 0.12f, true);

		if (!m_ShootCheck)
			m_ShootCheck = true;

		m_fTime = 0.f;
		
		Ptr<CPrefab> pPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\P_Bullet.pref");
		CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

		CGameObject* pObj = pPrefab->Instantiate();


		CP_Bullet* pBullet = (CP_Bullet*)CScriptMgr::GetScript((UINT)SCRIPT_TYPE::P_BULLET);

		pBullet->SetSpeed(500.f);
		pBullet->SetDir(Vec3(Dir.x, Dir.y, 0.f));
		pObj->Transform()->SetRelativePos(GetOwner()->Transform()->GetWorldPos() + Dir * Vec2(40.f, 40.f));
		pObj->AddComponent(pBullet);

		tEventInfo evninfo;

		evninfo.eType = EVENT_TYPE::CREATE_OBJ;
		evninfo.lParam = (DWORD_PTR)pObj;
		evninfo.wParam = 3;

		CEventMgr::GetInst()->AddEvent(evninfo);
	}
}

void CMagnumScript::CanAttack(float AttackDelay)
{
	if (m_fTime > AttackDelay && !m_bIsAttack) { m_bIsAttack = true; }
	else { m_fTime += DT; }
}


Vec2 CMagnumScript::GetMousePose() const
{
	Vec2 Resolution = CDevice::GetInst()->GetRenderResolution();
	Vec2 MousePos = CKeyMgr::GetInst()->GetMousePos();
	return Vec2(MousePos.x - Resolution.x / 2.f, -MousePos.y + Resolution.y / 2.f);
}
