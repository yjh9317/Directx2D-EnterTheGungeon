#include "pch.h"
#include "CSkull_Bullet.h"

#include <Engine/CAnimator2D.h>
#include <Engine/CCollider2D.h>

#include "CM_Bullet.h"
#include <Engine/CEventMgr.h>

CSkull_Bullet::CSkull_Bullet()
	:CScript((int)SCRIPT_TYPE::SKULL_BULLET)
	, m_RotateSpeed(20.f)
	, m_Speed(1.f)
	, m_fTime(0.f)
	, f_Dir(0)
{
}

CSkull_Bullet::~CSkull_Bullet()
{
}


void CSkull_Bullet::start()
{
	Animator2D()->Play(L"Skull_Bullet", true);
}

void CSkull_Bullet::update()
{
	m_fTime += DT;
	SetPosition();
	TimeDestroyCheck(3.f);
}

void CSkull_Bullet::SetPosition()
{
	CGameObject* pObj = CSceneMgr::GetInst()->GetCurScene()->GetPlayer();
	Vec3 vPlayerPos = CSceneMgr::GetInst()->GetCurScene()->GetPlayerPos();
	Vec2 vColPos = pObj->Collider2D()->GetOffsetPos();

	Vec3 vPos = Transform()->GetWorldPos();
	Vec3 TargetDir = Vec3(vPlayerPos.x + vColPos.x - vPos.x, vPlayerPos.y + vColPos.x - vPos.y, 0);
	TargetDir.Normalize();

	Vec3 vTrans = Transform()->GetRelativePos();
	vTrans += DT * TargetDir * m_Speed;
	Transform()->SetRelativePos(vTrans);
}

void CSkull_Bullet::TimeDestroyCheck(float Time)
{
	if (m_fTime > Time)
		GetOwner()->Destroy();
}


void CSkull_Bullet::OnCollisionEnter(CGameObject* _OtherObject)
{
	if (_OtherObject->GetName() == L"P_Bullet" || _OtherObject->GetName() == L"Ice_Bullet" || _OtherObject->GetName() == L"Player")
	{
		Vec3 vTrans = Transform()->GetWorldPos();
		for (int i = 0; i < 8; ++i)
		{
			CGameObject* pObj = PrefabInstantiate(L"prefab\\M_Bullet.pref");
			pObj->Transform()->SetRelativePos(Vec3(vTrans.x, vTrans.y, 0.f)); 

			CM_Bullet* pBullet = (CM_Bullet*)CScriptMgr::GetScript((UINT)SCRIPT_TYPE::M_BULLET);
			f_Dir += 0.785f;

			float cos = cosf(f_Dir);
			float sin = sinf(f_Dir);

			Vec3 vDir = Vec3(cos, sin, 0.f); vDir.Normalize();

			pBullet->SetDir(vDir);
			pBullet->SetSpeed(300.f);
			pObj->AddComponent(pBullet);

			AddCreateObjectEvent(pObj, 6);
		}
		GetOwner()->Destroy();
	}
}
