#include "pch.h"
#include "CIceGunScript.h"

#include <Engine/CEventMgr.h>
#include <Engine/CAnimator2D.h>
#include <Engine/CAnimation2D.h>
#include "CPlayerStateScript.h"
#include "CP_Bullet.h"
#include "CIce_Bullet.h"

CIceGunScript::CIceGunScript()
	:CScript((int)SCRIPT_TYPE::ICEGUNSCRIPT)
	, m_fTime(0.f)
	, m_Atk(true)
	, m_Dir(false)
	, m_ShootCheck(false)
	, m_ShootTime(0.f)
	, m_type(GUN_TYPE::LIE)
{
	m_Pos = Vec3(21.f, -20.f, 0.f);
}


CIceGunScript::~CIceGunScript()
{
}


void CIceGunScript::start()
{
	Transform()->SetIgnoreParentScale(true);
}

void CIceGunScript::update()
{
	if (m_type == GUN_TYPE::LIE)
	{
		if (KEY_TAP(KEY::E))
		{
			PickupIceGun();
		}
	}
	else if (m_type == GUN_TYPE::HOLD)
	{
		if (GetOwner()->GetParent() == nullptr) return;
		IsAttackPossible(0.4f);

		Vec3 Win_MousePos = GetWindowMousePos();
		Vec3 Pos = Transform()->GetRelativePos();
		Vec2 Dir = GetDirBetweenObjects(Pos, Win_MousePos);
		float Radian = CalcRadianBetweenObjects(Pos,Win_MousePos);
		float Angle = CalcAngleBetweenObjects(Pos,Win_MousePos);

		SetRotation(Angle, Radian);
		SetScale();
		if (KEY_TAP(KEY::LBTN))
		{
			Attack(Dir);
		}
	}
}

void CIceGunScript::SetScale()
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

	if (pStateScript->GetHandCheck() == false)
		Transform()->Transform()->SetRelativeScale(Vec3(0.f, 0.f, 0.f));
	else
		Transform()->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 0.f));
}

void CIceGunScript::Attack(Vec2& Dir)
{
	if (!m_Atk) return;

	CGameObject* pObj = PrefabInstantiate(L"prefab\\Ice_Bullet.pref");
	if (nullptr == pObj) return;

	if (!m_ShootCheck)
		m_ShootCheck = true;

	Ptr<CSound> pSound = CResMgr::GetInst()->Load<CSound>(L"sound\\IceGun\\Shot.wav", L"sound\\IceGun\\Shot.wav");
	pSound->Play(1, 0.12f, true);
	CIce_Bullet* pBullet = (CIce_Bullet*)CScriptMgr::GetScript((UINT)SCRIPT_TYPE::ICE_BULLET);

	pBullet->SetSpeed(500.f);
	pBullet->SetDir(Vec3(Dir.x, Dir.y, 0.f));
	pObj->Transform()->SetRelativePos(GetOwner()->Transform()->GetWorldPos() + Dir * Vec2(40.f, 40.f));
	pObj->AddComponent(pBullet);
	AddCreateObjectEvent(pObj, 3);

	m_fTime = 0.f;
	m_Atk = false;
}

void CIceGunScript::SetRotation(float Angle, float Radian)
{
	Vec3 vRot = Transform()->GetRelativeRotation();

	if (m_Dir)
	{
		if (Angle > 110.f || Angle < -110.f)
			m_Dir = false;

		Animator2D()->Play(L"R_IceGun", true);
		Transform()->SetRelativePos(Vec3(m_Pos.x, m_Pos.y, m_Pos.z));
		vRot.z = Radian;
	}
	else if (!m_Dir)
	{
		if ((Angle < 70.f && Angle >0.f) || (Angle <0.f && Angle>-70.f))
			m_Dir = true;

		Animator2D()->Play(L"L_IceGun", true);
		Transform()->SetRelativePos(Vec3(-m_Pos.x, m_Pos.y, m_Pos.z));
		vRot.z = Radian - XM_PI;
	}

	Transform()->SetRelativeRotation(vRot);
}

Vec3 CIceGunScript::GetWindowMousePos() const
{
	Vec2 Resolution = CDevice::GetInst()->GetRenderResolution();
	Vec2 MousePos = CKeyMgr::GetInst()->GetMousePos();
	return Vec3(MousePos.x - Resolution.x / 2.f, -MousePos.y + Resolution.y / 2.f, 0.f);
}

void CIceGunScript::IsAttackPossible(float AttackDelay)
{
	if (m_Atk) return;
	m_fTime += DT;
	if (m_fTime > AttackDelay)
	{
		m_Atk = true;
	}
}

void CIceGunScript::PickupIceGun()
{
	Vec3 vPlayerPos = CSceneMgr::GetInst()->GetCurScene()->GetPlayerPos();
	Vec3 vPos = Transform()->GetWorldPos();
	float distance = CalcDistBetweenObjects(vPlayerPos, vPos);

	if (distance <= 80.f)
	{
		Ptr<CSound> pSound = CResMgr::GetInst()->Load<CSound>(L"sound\\item\\Pickup.wav", L"sound\\item\\Pickup.wav");
		pSound->Play(1, 0.2f, true);

		CGameObject* pObj = CSceneMgr::GetInst()->GetCurScene()->GetPlayer();
		AddChildObjectEvent(pObj, GetOwner());
		m_type = GUN_TYPE::HOLD;
		CPlayerStateScript* PlayerScript =pObj->GetScript<CPlayerStateScript>();
		PlayerScript->RegistGun(GetOwner());
	}
}

