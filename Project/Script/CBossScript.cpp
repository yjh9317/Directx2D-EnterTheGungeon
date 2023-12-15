#include "pch.h"
#include "CBossScript.h"

#include <Engine/CAnimator2D.h>
#include <Engine/CAnimation2D.h>
#include <Engine/CEventMgr.h>

#include "CScriptMgr.h"
#include "CM_Bullet.h"
#include "CBoss_Bullet.h"
#include "CSkull_Bullet.h"
#include "CBarrier_Bullet.h"

#include <time.h>
#include <CHeadGunScript.h>

#define W 32
#define R 32
#define M1 3
#define M2 24
#define M3 10

#define MAT0POS(t,v) (v^(v>>t))
#define MAT0NEG(t,v) (v^(v<<(-(t))))
#define Identity(v) (v)

#define V0            STATE[state_i                   ]
#define VM1           STATE[(state_i+M1) & 0x0000001fU]
#define VM2           STATE[(state_i+M2) & 0x0000001fU]
#define VM3           STATE[(state_i+M3) & 0x0000001fU]
#define VRm1          STATE[(state_i+31) & 0x0000001fU]
#define newV0         STATE[(state_i+31) & 0x0000001fU]
#define newV1         STATE[state_i                   ]

#define FACT 2.32830643653869628906e-10

static unsigned int state_i = 0;
static unsigned int STATE[R];
static unsigned int z0, z1, z2;

void InitWELLRNG1024a(unsigned int* init) {
	int j;
	state_i = 0;
	for (j = 0; j < R; j++)
		STATE[j] = init[j];
}

double WELLRNG1024a(void) {
	z0 = VRm1;
	z1 = Identity(V0) ^ MAT0POS(8, VM1);
	z2 = MAT0NEG(-19, VM2) ^ MAT0NEG(-14, VM3);
	newV1 = z1 ^ z2;
	newV0 = MAT0NEG(-11, z0) ^ MAT0NEG(-7, z1) ^ MAT0NEG(-13, z2);
	state_i = (state_i + 31) & 0x0000001fU;
	return ((double)STATE[state_i] * FACT);
}

CBossScript::CBossScript()
	:CMonsterScript((int)SCRIPT_TYPE::BOSSSCRIPT)
	, m_fBossBulletTime(0.f)
	, i_BossBulletCnt(0)
	, f_Dir(0)
	, b_Dir(false)
	, m_bSummonHeadGun(false)
	, m_iOrder(0)
{
	SetState(MON_STATE::START);
	m_HP = 300;
	m_DetectRange = 1000.f;
	m_AttackRange = 500.f;
	m_fSpeed = 70.f;
}

CBossScript::~CBossScript()
{
}

void CBossScript::start()
{
	srand((unsigned)time(NULL));
	unsigned int init[32];
	for (int i = 0; i < 32; i++) {
		init[i] = rand() << 16 | rand();
		// WELL Random 을 초기화 하기 위해, C 표준 rand() 함수를 이용하여 init 값을 생성합니다
	}
	InitWELLRNG1024a(init); // WELL Random 초기화
}

void CBossScript::StartState()
{
	m_fTime += DT;
	if(m_fTime >= 2.4f) SummonHeadGun();
	StartBossAnimation();
}

void CBossScript::Idle()
{
	Animator2D()->Play(L"Idle",true);
	ChangeState(MON_STATE::TRACE);
}

void CBossScript::Trace()
{
	int RanInt = GetRandomInt();

	Animator2D()->Play(L"Idle", true);

	IsAttackPossible(4.f);
	if (CanAttack())
		ChangeToAttackState();

	TracePlayer();
}


void CBossScript::Attack()
{
	m_fTime += DT;
	if (!m_Sound)
	{
		m_Sound = true;
		Ptr<CSound> pSound = CResMgr::GetInst()->Load<CSound>(L"sound\\highpriest\\Atk.wav", L"sound\\highpriest\\Atk.wav");
		pSound->Play(1, 0.07f, true);
	}
	if (m_fTime >= 0.1f && m_bAttack)
	{
		Shoot_Atk();
	}

	if ((m_BehaviorAngle <= 181.f && m_BehaviorAngle >= 90.f) || (m_BehaviorAngle <= -90.f && m_BehaviorAngle >= -181.f))
	{
		if (m_fTime <= 1.2f && m_fTime >= 0.f)
		{
			Animator2D()->Play(L"Left_Atk", false);
		}
		else
		{
			Animator2D()->FindAnim(L"Left_Atk")->Reset();
			i_BossBulletCnt = 0;
			m_fBossBulletTime = 0.f;
			m_BehaviorAngle = 0.f;
			m_fTime = 0.f;
			m_bAttack = false;
			m_fAttackTime = 0.f;
			m_Sound = false;
			ChangeState(MON_STATE::IDLE);
		}
	}
	else if ((m_BehaviorAngle <= 90.f && m_BehaviorAngle >= 0.f) || (m_BehaviorAngle <= 0.f && m_BehaviorAngle >= -90.f))
	{
		if (m_fTime <= 1.2f && m_fTime >= 0.f)
		{
			Animator2D()->Play(L"Right_Atk", false);
		}
		else
		{
			Animator2D()->FindAnim(L"Right_Atk")->Reset();
			i_BossBulletCnt = 0;
			m_fBossBulletTime = 0.f;
			m_BehaviorAngle = 0.f;
			m_fTime = 0.f;
			m_bAttack = false;
			m_fAttackTime = 0.f;
			m_Sound = false;
			ChangeState(MON_STATE::IDLE);
		}
	}
}

void CBossScript::Skill1()
{
	if (!m_Sound)
	{
		m_Sound = true;
		Ptr<CSound> pSound = CResMgr::GetInst()->Load<CSound>(L"sound\\highpriest\\Atk.wav", L"sound\\highpriest\\Atk.wav");
		pSound->Play(1, 0.07f, true);
	}
	m_fTime += DT;

	if (m_fTime >= 0.1f && m_bAttack)
	{
		Execute_Skill1();
	}

	Animator2D()->Play(L"Both_Atk", false);

	if (m_fTime > 0.95f)
	{
		m_bAttack = false;
		m_fAttackTime = 1.3f;
		Animator2D()->FindAnim(L"Both_Atk")->Reset();
		m_fTime = 0.f;
		i_BossBulletCnt = 0.f;
		m_Sound = false;
		ChangeState(MON_STATE::IDLE);
	}
}

void CBossScript::Skill2()
{
	if (!m_Sound)
	{
		m_Sound = true;
		Ptr<CSound> pSound = CResMgr::GetInst()->Load<CSound>(L"sound\\highpriest\\Machinegun.wav", L"sound\\highpriest\\Machinegun.wav");
		pSound->Play(1, 0.07f, true);
	}
	m_fTime += DT;

	if (m_fTime >= 0.65f)
	{
		Execute_Skill2();
	}

	if (m_fTime <= 0.65f && m_fTime >= 0.f)
	{
		Animator2D()->Play(L"Atk_Prev", false);
	}
	else if (m_fTime >= 0.65f)
		Animator2D()->Play(L"MachineGun", true);

	if (m_fTime >= 5.f)
	{
		m_fBossBulletTime = 0.f;
		m_bAttack = false;
		Animator2D()->FindAnim(L"Atk_Prev")->Reset();
		m_fTime = 0.f;
		m_Sound = false;
		ChangeState(MON_STATE::IDLE);
	}
}

void CBossScript::Skill3()
{
	m_fTime += DT;

	if (m_fTime > 0.65f)
	{
		Execute_Skill3();
	}

	if (m_fTime <= 0.65f && m_fTime >= 0.f)
	{
		Animator2D()->Play(L"Atk_Prev", false);
	}
	else if (m_fTime >= 0.65f)
	{
		Animator2D()->Play(L"Barrier", true);
	}

	if (m_fTime >= 3.f)
	{
		m_fBossBulletTime = 0.f;
		m_bAttack = false;
		Animator2D()->FindAnim(L"Atk_Prev")->Reset();
		m_fTime = 0.f;
		ChangeState(MON_STATE::IDLE);
	}
}

void CBossScript::Skill4()
{
	m_fTime += DT;

	if (m_fTime >= 0.7f && !m_bSummonHeadGun)
	{
		Execute_Skill4();
	}

	if (m_fTime <= 0.8f && m_fTime >= 0.f)
	{
		Animator2D()->Play(L"HeadOpen", false);
	}
	else if (m_fTime >= 0.8f && m_fTime<3.f)
	{
		Animator2D()->Play(L"HeadOpening", true);
	}
	else if (m_fTime >= 3.f)
	{
		Animator2D()->Play(L"HeadClose", false);
	}

	if (m_fTime >= 3.5f) {
		m_bAttack = false;
		Animator2D()->FindAnim(L"HeadOpen")->Reset();
		Animator2D()->FindAnim(L"HeadClose")->Reset();
		m_fTime = 0.f;
		m_bSummonHeadGun = false;
		ChangeState(MON_STATE::IDLE);
	}
}

void CBossScript::Skill5()
{
	if (!m_Sound)
	{
		m_Sound = true;
		Ptr<CSound> pSound = CResMgr::GetInst()->Load<CSound>(L"sound\\highpriest\\Whips.wav", L"sound\\highpriest\\Whips.wav");
		pSound->Play(1, 0.07f, true);
	}

	m_fTime += DT;

	if (m_fTime >= 0.65f)
	{
		Execute_Skill5();
	}

	if (m_fTime <= 0.65f && m_fTime >= 0.f)
	{
		Animator2D()->Play(L"Atk_Prev", false);
	}
	else if (m_fTime >= 0.65f)
		Animator2D()->Play(L"MachineGun", true);

	if (m_fTime >= 4.f)
	{
		m_fBossBulletTime = 0.f;
		m_bAttack = false;
		Animator2D()->FindAnim(L"Atk_Prev")->Reset();
		m_fTime = 0.f;
		m_Sound = false;
		ChangeState(MON_STATE::IDLE);
	}
}

void CBossScript::Dead()
{
	Animator2D()->Play(L"Dead", false);
}

void CBossScript::Shoot_Atk()
{
	m_fBossBulletTime += DT;

	if ((m_BehaviorAngle <= 181.f && m_BehaviorAngle >= 90.f) || (m_BehaviorAngle <= -90.f && m_BehaviorAngle >= -181.f))
	{
		if (m_fBossBulletTime >= 0.033f && i_BossBulletCnt < 10)
		{
			m_fBossBulletTime = 0.f;
			Vec3 pPlayerPos = CSceneMgr::GetInst()->GetCurScene()->GetPlayerPos();

			CBoss_Bullet* pBullet = CreateBossBulletScript();
			CGameObject* pObj = CreateBullet();
			Vec3 pTrans = Transform()->GetWorldPos();
			Vec2 vDir = GetDirBetweenObjects(pTrans,pPlayerPos);

			pObj->Transform()->SetRelativeScale(Vec3(40.f, 40.f, 10.f));
			pBullet->SetDir(Vec3(vDir.x,vDir.y,0));
			pBullet->SetSpeed(300.f);
			pBullet->WaitCheck(true);
			pBullet->WaitTime(1.f - i_BossBulletCnt * 0.1f);

			if (i_BossBulletCnt < 5)
				pObj->Transform()->SetRelativePos(Vec3(-50.f + pTrans.x - 15.f * i_BossBulletCnt, -60.f + pTrans.y + 20.f * i_BossBulletCnt, 0.f));
			else
				pObj->Transform()->SetRelativePos(Vec3(-50.f + pTrans.x - 15.f * (10 - i_BossBulletCnt), -60.f + pTrans.y + 20.f * i_BossBulletCnt, 0.f));

			pObj->AddComponent(pBullet);

			++i_BossBulletCnt;
			AddCreateObjectEvent(pObj, 6);
		}
	}
	else if ((m_BehaviorAngle <= 90.f && m_BehaviorAngle >= 0.f) || (m_BehaviorAngle <= 0.f && m_BehaviorAngle >= -90.f))
	{
		if (m_fBossBulletTime >= 0.033f && i_BossBulletCnt < 10)
		{
			m_fBossBulletTime = 0.f;
			Vec3 pPlayerPos = CSceneMgr::GetInst()->GetCurScene()->GetPlayerPos();

			CBoss_Bullet* pBullet = CreateBossBulletScript();
			CGameObject* pObj = CreateBullet();

			Vec3 pTrans = Transform()->GetWorldPos();
			Vec2 vDir = GetDirBetweenObjects(pTrans, pPlayerPos);

			pObj->Transform()->SetRelativeScale(Vec3(40.f, 40.f, 10.f));
			pBullet->SetDir(Vec3(vDir.x, vDir.y, 0));
			pBullet->SetSpeed(300.f);
			pBullet->WaitCheck(true);
			pBullet->WaitTime(1.f - i_BossBulletCnt * 0.1f);

			if (i_BossBulletCnt < 5)
				pObj->Transform()->SetRelativePos(Vec3(70.f + pTrans.x + 15.f * i_BossBulletCnt, -60.f + pTrans.y + 20.f * i_BossBulletCnt, 0.f));
			else
				pObj->Transform()->SetRelativePos(Vec3(70.f + pTrans.x + 15.f * (10 - i_BossBulletCnt), -60.f + pTrans.y + 20.f * i_BossBulletCnt, 0.f));

			pObj->AddComponent(pBullet);

			++i_BossBulletCnt;
			AddCreateObjectEvent(pObj, 6);
		}
	}
}

void CBossScript::Execute_Skill1()
{
	m_fBossBulletTime += DT;

	if (m_fBossBulletTime >= 0.033f && i_BossBulletCnt < 10)
	{
		Vec3 pPlayerPos = CSceneMgr::GetInst()->GetCurScene()->GetPlayerPos();
		CBoss_Bullet* pBullet = CreateBossBulletScript();

		Vec3 pTrans = Transform()->GetWorldPos();
		CGameObject* pObj = PrefabInstantiate(L"prefab\\M_Bullet.pref");

		Vec2 vDir = GetDirBetweenObjects(pTrans, pPlayerPos);
		pObj->Transform()->SetRelativeScale(Vec3(40.f, 40.f, 10.f));
		pBullet->SetDir(Vec3(vDir.x,vDir.y,0));
		pBullet->SetSpeed(300.f);
		pBullet->WaitCheck(true);
		pBullet->WaitTime(1.f - i_BossBulletCnt * 0.1f);

		if (i_BossBulletCnt < 5)
			pObj->Transform()->SetRelativePos(Vec3(-50.f + pTrans.x - 15.f * i_BossBulletCnt, -60.f + pTrans.y + 20.f * i_BossBulletCnt, 0.f));
		else
			pObj->Transform()->SetRelativePos(Vec3(-50.f + pTrans.x - 15.f * (10 - i_BossBulletCnt), -60.f + pTrans.y + 20.f * i_BossBulletCnt, 0.f));
		pObj->AddComponent(pBullet);
		AddCreateObjectEvent(pObj, 6);

		pBullet = CreateBossBulletScript();
		pObj = PrefabInstantiate(L"prefab\\M_Bullet.pref");

		pObj->Transform()->SetRelativeScale(Vec3(40.f, 40.f, 10.f));
		pBullet->SetDir(Vec3(vDir.x, vDir.y, 0));
		pBullet->SetSpeed(300.f);
		pBullet->WaitCheck(true);
		pBullet->WaitTime(1.f - i_BossBulletCnt * 0.1f);

		if (i_BossBulletCnt < 5)
			pObj->Transform()->SetRelativePos(Vec3(70.f + pTrans.x + 15.f * i_BossBulletCnt, -60.f + pTrans.y + 20.f * i_BossBulletCnt, 0.f));
		else
			pObj->Transform()->SetRelativePos(Vec3(70.f + pTrans.x + 15.f * (10 - i_BossBulletCnt), -60.f + pTrans.y + 20.f * i_BossBulletCnt, 0.f));
		pObj->AddComponent(pBullet);

		AddCreateObjectEvent(pObj, 6);

		m_fBossBulletTime = 0.f;
		++i_BossBulletCnt;
	}
}

void CBossScript::Execute_Skill2()
{
	m_fBossBulletTime += DT;
	WELLRNG1024a();

	int rnd =  (int)((WELLRNG1024a() * 100));
	int rnd2 = (int)((WELLRNG1024a() * 200) - 100);

	int rnd3 = (int)((WELLRNG1024a() * 100));
	int rnd4 = (int)((WELLRNG1024a() * 200) - 100);

	if (m_fBossBulletTime >= 0.04f)
	{
		CGameObject* pObj = CreateBullet();

		Vec3 pTrans = Transform()->GetWorldPos();
		pObj->Transform()->SetRelativePos(Vec3(pTrans.x+120.f, pTrans.y +40.f , 0.f));

		float _frandom = (float)rnd / 100.f; float _frandom2 = (float)rnd2 / 100.f;

		Vec3 vDir = Vec3(_frandom - 0.3f, _frandom2, 0.f);
		vDir.Normalize();

		CM_Bullet* pBullet = (CM_Bullet*)CScriptMgr::GetScript((UINT)SCRIPT_TYPE::M_BULLET);
		pBullet->SetDir(vDir);
		pBullet->SetSpeed(300.f);
		pObj->AddComponent(pBullet);

		tEventInfo evninfo;
		evninfo.eType = EVENT_TYPE::CREATE_OBJ;
		evninfo.lParam = (DWORD_PTR)pObj;
		evninfo.wParam = 6;

		CEventMgr::GetInst()->AddEvent(evninfo);


		// 왼쪽
		pBullet = (CM_Bullet*)CScriptMgr::GetScript((UINT)SCRIPT_TYPE::M_BULLET);

		pObj = CreateBullet();
		pObj->Transform()->SetRelativePos(Vec3(pTrans.x - 120.f, pTrans.y + 40.f, 0.f));

		_frandom = (float)rnd3 / 100.f;
		_frandom2 = (float)rnd4 / 100.f;

		vDir = Vec3(-_frandom +0.3f, _frandom2, 0.f);
		vDir.Normalize();

		pBullet->SetDir(vDir);
		pBullet->SetSpeed(300.f);

		pObj->AddComponent(pBullet);

		evninfo.eType = EVENT_TYPE::CREATE_OBJ;
		evninfo.lParam = (DWORD_PTR)pObj;
		evninfo.wParam = 6;

		CEventMgr::GetInst()->AddEvent(evninfo);
		m_fBossBulletTime = 0.f;
	}
}

void CBossScript::Execute_Skill3()
{
	m_fBossBulletTime += DT;

	int rnd = (int)((double)WELLRNG1024a() * 200) - 100;
	int rnd2 = (int)((double)WELLRNG1024a() * 200) - 100;

	if (m_fBossBulletTime >= 0.5f)
	{
		// 오른쪽
		Ptr<CPrefab> pPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Skull_Bullet.pref");
		CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();


		CSkull_Bullet* pBullet = (CSkull_Bullet*)CScriptMgr::GetScript((UINT)SCRIPT_TYPE::SKULL_BULLET);

		Vec3 pTrans = Transform()->GetWorldPos();
		CGameObject* pObj = pPrefab->Instantiate();
		pObj->Transform()->SetRelativePos(Vec3(pTrans.x, pTrans.y, 0.f));

		float _frandom = (float)rnd / 100.f;
		float _frandom2 = (float)rnd2 / 100.f;

		Vec3 vDir = Vec3(_frandom, _frandom2, 0.f);
		vDir.Normalize();


		pBullet->SetDir(vDir);
		pBullet->SetSpeed(170.f);

		pObj->AddComponent(pBullet);

		tEventInfo evninfo;
		evninfo.eType = EVENT_TYPE::CREATE_OBJ;
		evninfo.lParam = (DWORD_PTR)pObj;
		evninfo.wParam = 6;

		CEventMgr::GetInst()->AddEvent(evninfo);
		m_fBossBulletTime = 0.f;
	}

}

void CBossScript::Execute_Skill4()
{
	m_bSummonHeadGun = true;
	CGameObject* pObj = PrefabInstantiate(L"prefab\\HeadGun.pref");

	Vec3 pTrans = Transform()->GetRelativePos();
	Vec3 pTran = Transform()->GetWorldPos();
	pObj->Transform()->SetRelativePos(Vec3(pTrans.x, pTrans.y + 50.f, pTrans.z-1.f));
	pObj->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 0.f));
	CHeadGunScript* HeadGunScript = pObj->GetScript<CHeadGunScript>();
	if (HeadGunScript) HeadGunScript->SetDestroyTime(2.6f);
	AddCreateObjectEvent(pObj, 5);
}

void CBossScript::Execute_Skill5()
{
	m_fBossBulletTime += DT;

	if (m_fBossBulletTime >= 0.05f)
	{
		// 오른쪽
		Ptr<CPrefab> pPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\M_Bullet.pref");
		CM_Bullet* pBullet = (CM_Bullet*)CScriptMgr::GetScript((UINT)SCRIPT_TYPE::M_BULLET);

		Vec3 pTrans = Transform()->GetWorldPos();
		CGameObject* pObj = pPrefab->Instantiate();
		pObj->Transform()->SetRelativePos(Vec3(pTrans.x + 120.f, pTrans.y + 40.f, 0.f));

		if (f_Dir < XM_PI)
			f_Dir += 0.15f;
		else
			f_Dir = -XM_PI;
		
		float cos = cosf(f_Dir);
		float sin = sinf(f_Dir);

		Vec3 vDir = Vec3(cos,sin, 0.f);
		vDir.Normalize();


		pBullet->SetDir(vDir);
		pBullet->SetSpeed(300.f);

		pObj->AddComponent(pBullet);

		tEventInfo evninfo;
		evninfo.eType = EVENT_TYPE::CREATE_OBJ;
		evninfo.lParam = (DWORD_PTR)pObj;
		evninfo.wParam = 6;

		CEventMgr::GetInst()->AddEvent(evninfo);


		// 왼쪽

		pBullet = (CM_Bullet*)CScriptMgr::GetScript((UINT)SCRIPT_TYPE::M_BULLET);

		pObj = pPrefab->Instantiate();

		pObj->Transform()->SetRelativePos(Vec3(pTrans.x - 120.f, pTrans.y + 40.f, 0.f));



		vDir = Vec3(-cos, sin, 0.f);
		vDir.Normalize();


		pBullet->SetDir(vDir);
		pBullet->SetSpeed(300.f);

		pObj->AddComponent(pBullet);

		evninfo.eType = EVENT_TYPE::CREATE_OBJ;
		evninfo.lParam = (DWORD_PTR)pObj;
		evninfo.wParam = 6;

		CEventMgr::GetInst()->AddEvent(evninfo);
		m_fBossBulletTime = 0.f;
		
	}
}


void CBossScript::StartBossAnimation()
{
	if (m_fTime <= 2.f)
	{
		Animator2D()->Play(L"Idle", true);
	}
	else if (m_fTime < 2.8f)
	{
		Animator2D()->Play(L"HeadOpen", false);
	}
	else if (m_fTime < 4.6f)
	{
		Animator2D()->Play(L"HeadOpening", true);
	}
	else if (m_fTime < 5.1f)
	{
		Animator2D()->Play(L"HeadClose", false);
	}
	else if (m_fTime < 6.1f)
	{
		Animator2D()->Play(L"Idle", true);
	}
	else
	{
		Animator2D()->FindAnim(L"HeadOpen")->Reset();
		Animator2D()->FindAnim(L"HeadClose")->Reset();
		m_fTime = 0.f;
		m_bAttack = false;
		m_bSummonHeadGun = false;
		ChangeState(MON_STATE::IDLE);
	}
}

void CBossScript::SummonHeadGun()
{
	if (!m_bSummonHeadGun)
	{
		m_bSummonHeadGun = true;
		m_HeadGun = PrefabInstantiate(L"prefab\\HeadGun_Start.pref");
		m_HeadGun->Animator2D()->Play(L"Start", false);

		Vec3 pTrans = Transform()->GetRelativePos();
		m_HeadGun->Transform()->SetRelativePos(Vec3(pTrans.x, pTrans.y + 50.f, pTrans.z - 1.f));
		AddCreateObjectEvent(m_HeadGun, 5);
	}
	
	if (m_HeadGun)
	{
		if (m_HeadGun->Animator2D()->GetCurAnim()->IsFinish())
		{
			static float Time = 0;
			Time = Time + DT;
			if (Time > 0.8f)
			{
				m_HeadGun->Destroy();
				m_HeadGun = nullptr;
			}
		}
	}

}

void CBossScript::TracePlayer()
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	Vec2 vPlayerPos = Vec2(pCurScene->GetPlayerPos().x, pCurScene->GetPlayerPos().y);
	Vec2 vMonPos = Vec2(Transform()->GetWorldPos().x, Transform()->GetWorldPos().y);

	vMonPos += GetDirBetweenObjects(vMonPos, vPlayerPos) * m_fSpeed * DT;
	Transform()->SetRelativePos(vMonPos.x, vMonPos.y, 800.f);
}

void CBossScript::ChangeToAttackState()
{
	m_BehaviorAngle = m_Angle;
	m_fBossBulletTime = 0.f;
	
	++m_iOrder;
	m_iOrder = 6;
	if (m_iOrder <= 2)
	{
		ChangeState(MON_STATE::ATK);
	}
	else if (m_iOrder <= 3)
	{
		ChangeState(MON_STATE::SKILL1);
	}
	else if (m_iOrder <= 4)
	{
		ChangeState(MON_STATE::SKILL2);
	}
	else if (m_iOrder <= 5)
	{
		ChangeState(MON_STATE::SKILL3);
	}
	else if (m_iOrder <= 6)
	{
		ChangeState(MON_STATE::SKILL4);
	}
	else if (m_iOrder <= 7)
	{
		ChangeState(MON_STATE::SKILL5);
	}
	else { m_iOrder = 0; }
}

bool CBossScript::CanAttack() const
{
	return m_fDistance < m_AttackRange && m_bAttack;
}

void CBossScript::IsAttackPossible(float AttackDelay)
{
	if (!m_bAttack)
	{
		if (m_fAttackTime >= AttackDelay)
		{
			m_bAttack = true;
			m_fAttackTime = 0.f;
		}
		else
			m_fAttackTime += DT;
	}
}

int CBossScript::GetRandomInt()
{
	WELLRNG1024a();
	return (int)((double)WELLRNG1024a() * (8));
}

void CBossScript::OnCollisionEnter(CGameObject* _OtherObject)
{
	if (_OtherObject->GetName() == L"P_Bullet" && !(GetState() == MON_STATE::DEAD))
	{
		m_HP -= 15;

		if (m_HP <= 0)
		{
			m_fTime = 0.f;
			ChangeState(MON_STATE::DEAD);
		}
	}

	if (_OtherObject->GetName() == L"Ice_Bullet" && !(GetState() == MON_STATE::DEAD))
	{
		m_HP -= 20;

		if (m_HP <= 0)
		{
			m_fTime = 0.f;
			ChangeState(MON_STATE::DEAD);
		}
	}
}

CBoss_Bullet* CBossScript::CreateBossBulletScript()
{
	return (CBoss_Bullet*)CScriptMgr::GetScript((UINT)SCRIPT_TYPE::BOSS_BULLET); 
}