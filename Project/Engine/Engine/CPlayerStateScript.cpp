#include "pch.h"
#include "CPlayerStateScript.h"

#include <Engine/CDevice.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CRenderMgr.h>
#include <Engine/CAnimator2D.h>
#include <Engine/CAnimation2D.h>
//#define DEGREE_TO_RADIAN(degree) ((PI/180)*(degree)) // 60분법 -> 호도법
//#define RADIAN_TO_DEGREE(radian) ((180/PI)*(radian)) // 호도법 -> 60분법


CPlayerStateScript::CPlayerStateScript()
	, m_pOwner(nullptr)
	, m_MousePos{}
	, m_fSpeed(240.f)
	, m_fTime(0.f)
	, m_ClickMousePos(0.f,0.f)
{
	m_eStateType = PLAYER_TYPE::IDLE;
	
}

CPlayerStateScript::~CPlayerStateScript()
{
}

void CPlayerStateScript::start()
{
}

void CPlayerStateScript::update()
{
	
	Vec2 Resolution = CDevice::GetInst()->GetRenderResolution();//Vec2(CRenderMgr::GetInst()->GetMainCamWidth(), CRenderMgr::GetInst()->GetMainCamHeight());
	Vec2 MousePos = CKeyMgr::GetInst()->GetMousePos();
	m_MousePos = Vec2(MousePos.x -Resolution.x/2.f, -MousePos.y + Resolution.y/2.f);


	switch (m_eStateType)
	{
	case PLAYER_TYPE::IDLE:
	{
		Idle();
		break;
	}
	case PLAYER_TYPE::WALK:
	{
		Walk();
		break;
	}
	case PLAYER_TYPE::ROLL:
	{
		Roll();
		break;
	}
	case PLAYER_TYPE::FALL:
	{
		Fall();
		break;
	}
	case PLAYER_TYPE::HIT:
		break;
	case PLAYER_TYPE::DEAD:
	{
		Dead();
		break;
	}
	}
}

void CPlayerStateScript::lateupdate()
{

}

void CPlayerStateScript::ChangeState(PLAYER_TYPE _etype)
{
	if (_etype == m_eStateType)
		return;

	switch (m_eStateType)
	{
	case PLAYER_TYPE::IDLE:
		break;
	case PLAYER_TYPE::WALK:
		break;
	case PLAYER_TYPE::ROLL:
	{
		if (m_fTime >= 0.81f)
		{
			m_pOwner->Animator2D()->GetCurAnim()->Reset();
			m_ClickMousePos = Vec2(0.f, 0.f);
			break;
		}
		else return;
	}
	case PLAYER_TYPE::FALL:
	{
		if (m_fTime >= 2.f)
		{
			m_pOwner->Animator2D()->GetCurAnim()->Reset();
			break;
		}
		else return;
	}
	case PLAYER_TYPE::HIT:
		break;
	case PLAYER_TYPE::DEAD:
	{
		if (m_fTime >= 2.f)
		{
			m_pOwner->Animator2D()->GetCurAnim()->Reset();
			break;
		}
		else return;	
	}
	}
	m_fTime = 0.f;
	
	//이벤트 처리로 바꿔야함
	m_eStateType = _etype;
}

void CPlayerStateScript::Idle()
{
	Vec3 MainCamPos = CRenderMgr::GetInst()->GetMainCamPos();
	Vec3 ObjPos = m_pOwner->Transform()->GetWorldPos();
	Vec2 Dir = Vec2(m_MousePos.x, -m_MousePos.y);
	Dir.Normalize();
	const double Rad_to_deg = 57.2957951;
	double d= atan2(Dir.y, Dir.x);
	d*= Rad_to_deg;

	if (d < 60 && d >= -30)
	{
		m_pOwner->Animator2D()->Play(L"Right_Idle", true);
	}
	else if (d>=60 && d<120)
	{
		if(Dir.x >= 0.f)
		m_pOwner->Animator2D()->Play(L"Front_R_Idle", true);
		else
		m_pOwner->Animator2D()->Play(L"Front_L_Idle", true);
	}
	else if ((d >= 120 && d < 180) || (d >= -180 && d < -150))
	{
		m_pOwner->Animator2D()->Play(L"Left_Idle", true);
	}
	else if (d >= -150 && d < -120)
	{
		m_pOwner->Animator2D()->Play(L"Left_B_Idle", true);
	}
	else if (d >= -120 && d < -60)
	{
		if (Dir.x >= 0.f)
			m_pOwner->Animator2D()->Play(L"Back_L_Idle", true);
		else
			m_pOwner->Animator2D()->Play(L"Back_R_Idle", true);
	}
	else if (d >= -60 && d < -30)
	{
		m_pOwner->Animator2D()->Play(L"Right_B_Idle", true);
	}
	
}

void CPlayerStateScript::Walk()
{
	Vec3 MainCamPos = CRenderMgr::GetInst()->GetMainCamPos();
	Vec3 ObjPos = m_pOwner->Transform()->GetWorldPos();
	Vec2 Dir = Vec2(m_MousePos.x, -m_MousePos.y);
	Dir.Normalize();
	const double Rad_to_deg = 57.2957951;
	double d = atan2(Dir.y, Dir.x);
	d *= Rad_to_deg;


	if (d < 60 && d >= -30)
	{
		m_pOwner->Animator2D()->Play(L"Right_Walk", true);
	}
	else if (d >= 60 && d < 120)
	{
		if (Dir.x >= 0.f)
			m_pOwner->Animator2D()->Play(L"Front_R_Walk", true);
		else
			m_pOwner->Animator2D()->Play(L"Front_L_Walk", true);
	}
	else if ((d >= 120 && d < 180) || (d >= -180 && d < -150))
	{
		m_pOwner->Animator2D()->Play(L"Left_Walk", true);
	}
	else if (d >= -150 && d < -120)
	{
		m_pOwner->Animator2D()->Play(L"Left_B_Walk", true);
	}
	else if (d >= -120 && d < -60)
	{
		if (Dir.x >= 0.f)
			m_pOwner->Animator2D()->Play(L"Back_L_Walk", true);
		else
			m_pOwner->Animator2D()->Play(L"Back_R_Walk", true);
	}
	else if (d >= -60 && d < -30)
	{
		m_pOwner->Animator2D()->Play(L"Right_B_Walk", true);
	}


	Vec3 vPos = m_pOwner->Transform()->GetRelativePos();

	Vec3 vWorldPos = m_pOwner->Transform()->GetWorldPos();

	if (KEY_PRESSED(KEY::A))
		vPos.x -= DT * m_fSpeed;

	if (KEY_PRESSED(KEY::D))
		vPos.x += DT * m_fSpeed;

	if (KEY_PRESSED(KEY::W))
		vPos.y += DT * m_fSpeed;

	if (KEY_PRESSED(KEY::S))
		vPos.y -= DT * m_fSpeed;

	m_pOwner->Transform()->SetRelativePos(vPos);
}

void CPlayerStateScript::Roll()
{
	m_fTime += DT;

	Vec3 MainCamPos = CRenderMgr::GetInst()->GetMainCamPos();
	Vec3 ObjPos = m_pOwner->Transform()->GetWorldPos();
	Vec2 Dir = Vec2(m_ClickMousePos.x, m_ClickMousePos.y);
	Dir.Normalize();
	const double Rad_to_deg = 57.2957951;
	double d = atan2(Dir.y, Dir.x);
	d *= Rad_to_deg;

	if (d < 60 && d >= 30)
	{
		m_pOwner->Animator2D()->Play(L"Back_R_Roll", false);
	}
	else if (d >= 60 && d < 120)
	{
		m_pOwner->Animator2D()->Play(L"Back_Roll", false);
	}
	else if ((d >= 120 && d < 150))// ||(d >= -180 && d <-150)
	{
		m_pOwner->Animator2D()->Play(L"Back_L_Roll", false);
	}
	else if ((d >= -180 && d < -120) || (d >= 150 && d <= 180))
	{
		m_pOwner->Animator2D()->Play(L"Left_Roll", false);
	}
	else if (d >= -120 && d < -60)
	{
		m_pOwner->Animator2D()->Play(L"Front_Roll", false);
	}
	else if ((d >= -60 && d < 0) || (d>=0 && d<30))
	{
		m_pOwner->Animator2D()->Play(L"Right_Roll", false);
	}


	Vec3 vPos = m_pOwner->Transform()->GetRelativePos();

	Vec3 vWorldPos = m_pOwner->Transform()->GetWorldPos();

	vPos.x += Dir.x * DT * (m_fSpeed+100.f);
	vPos.y += Dir.y * DT * (m_fSpeed+100.f);


	m_pOwner->Transform()->SetRelativePos(vPos);
}

void CPlayerStateScript::Dead()
{
	m_fTime += DT;
	m_pOwner->Animator2D()->Play(L"Dead", false);

	if (m_fTime > 2.f)
	{
		ChangeState(PLAYER_TYPE::IDLE);
	}
}

void CPlayerStateScript::Fall()
{
	m_fTime += DT;
	m_pOwner->Animator2D()->Play(L"Fall", false);
	if (m_fTime > 2.f)
	{
		ChangeState(PLAYER_TYPE::IDLE);
	}
}
