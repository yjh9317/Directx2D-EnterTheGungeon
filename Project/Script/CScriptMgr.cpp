#include "pch.h"
#include "CScriptMgr.h"

#include "CAKScript.h"
#include "CAStarScript.h"
#include "CBarrelScript.h"
#include "CBarrier_Bullet.h"
#include "CBlankBulletEffect.h"
#include "CBlankBulletScript.h"
#include "CBossRoomEvent.h"
#include "CBossScript.h"
#include "CBoss_Bullet.h"
#include "CCameraMoveScript.h"
#include "CChestScript.h"
#include "CDoorScript.h"
#include "CGlobeScript.h"
#include "CHandScript.h"
#include "CHeadGunScript.h"
#include "CHeartScript.h"
#include "CIceGunScript.h"
#include "CIce_Bullet.h"
#include "CLampsScript.h"
#include "CMagnumScript.h"
#include "CMaidenDead.h"
#include "CMaiden_Bullet.h"
#include "CMissileScript.h"
#include "CMonsterScript.h"
#include "CMon_Booklets.h"
#include "CMon_Bullats.h"
#include "CMon_Cubulon.h"
#include "CMon_Gigi.h"
#include "CMon_Kin.h"
#include "CMon_Maiden.h"
#include "CMovementScript.h"
#include "CM_Bullet.h"
#include "CParticleScript.h"
#include "CPlayerScript.h"
#include "CPlayerStateScript.h"
#include "CP_Bullet.h"
#include "CRoomEvent_1.h"
#include "CRoomEvent_2.h"
#include "CRoomEvent_3.h"
#include "CRoomEvent_4.h"
#include "CSkull_Bullet.h"
#include "CTableScript.h"
#include "CWallScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CAKScript");
	_vec.push_back(L"CAStarScript");
	_vec.push_back(L"CBarrelScript");
	_vec.push_back(L"CBarrier_Bullet");
	_vec.push_back(L"CBlankBulletEffect");
	_vec.push_back(L"CBlankBulletScript");
	_vec.push_back(L"CBossRoomEvent");
	_vec.push_back(L"CBossScript");
	_vec.push_back(L"CBoss_Bullet");
	_vec.push_back(L"CCameraMoveScript");
	_vec.push_back(L"CChestScript");
	_vec.push_back(L"CDoorScript");
	_vec.push_back(L"CGlobeScript");
	_vec.push_back(L"CHandScript");
	_vec.push_back(L"CHeadGunScript");
	_vec.push_back(L"CHeartScript");
	_vec.push_back(L"CIceGunScript");
	_vec.push_back(L"CIce_Bullet");
	_vec.push_back(L"CLampsScript");
	_vec.push_back(L"CMagnumScript");
	_vec.push_back(L"CMaidenDead");
	_vec.push_back(L"CMaiden_Bullet");
	_vec.push_back(L"CMissileScript");
	_vec.push_back(L"CMonsterScript");
	_vec.push_back(L"CMon_Booklets");
	_vec.push_back(L"CMon_Bullats");
	_vec.push_back(L"CMon_Cubulon");
	_vec.push_back(L"CMon_Gigi");
	_vec.push_back(L"CMon_Kin");
	_vec.push_back(L"CMon_Maiden");
	_vec.push_back(L"CMovementScript");
	_vec.push_back(L"CM_Bullet");
	_vec.push_back(L"CParticleScript");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"CPlayerStateScript");
	_vec.push_back(L"CP_Bullet");
	_vec.push_back(L"CRoomEvent_1");
	_vec.push_back(L"CRoomEvent_2");
	_vec.push_back(L"CRoomEvent_3");
	_vec.push_back(L"CRoomEvent_4");
	_vec.push_back(L"CSkull_Bullet");
	_vec.push_back(L"CTableScript");
	_vec.push_back(L"CWallScript");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CAKScript" == _strScriptName)
		return new CAKScript;
	if (L"CAStarScript" == _strScriptName)
		return new CAStarScript;
	if (L"CBarrelScript" == _strScriptName)
		return new CBarrelScript;
	if (L"CBarrier_Bullet" == _strScriptName)
		return new CBarrier_Bullet;
	if (L"CBlankBulletEffect" == _strScriptName)
		return new CBlankBulletEffect;
	if (L"CBlankBulletScript" == _strScriptName)
		return new CBlankBulletScript;
	if (L"CBossRoomEvent" == _strScriptName)
		return new CBossRoomEvent;
	if (L"CBossScript" == _strScriptName)
		return new CBossScript;
	if (L"CBoss_Bullet" == _strScriptName)
		return new CBoss_Bullet;
	if (L"CCameraMoveScript" == _strScriptName)
		return new CCameraMoveScript;
	if (L"CChestScript" == _strScriptName)
		return new CChestScript;
	if (L"CDoorScript" == _strScriptName)
		return new CDoorScript;
	if (L"CGlobeScript" == _strScriptName)
		return new CGlobeScript;
	if (L"CHandScript" == _strScriptName)
		return new CHandScript;
	if (L"CHeadGunScript" == _strScriptName)
		return new CHeadGunScript;
	if (L"CHeartScript" == _strScriptName)
		return new CHeartScript;
	if (L"CIceGunScript" == _strScriptName)
		return new CIceGunScript;
	if (L"CIce_Bullet" == _strScriptName)
		return new CIce_Bullet;
	if (L"CLampsScript" == _strScriptName)
		return new CLampsScript;
	if (L"CMagnumScript" == _strScriptName)
		return new CMagnumScript;
	if (L"CMaidenDead" == _strScriptName)
		return new CMaidenDead;
	if (L"CMaiden_Bullet" == _strScriptName)
		return new CMaiden_Bullet;
	if (L"CMissileScript" == _strScriptName)
		return new CMissileScript;
	if (L"CMonsterScript" == _strScriptName)
		return new CMonsterScript;
	if (L"CMon_Booklets" == _strScriptName)
		return new CMon_Booklets;
	if (L"CMon_Bullats" == _strScriptName)
		return new CMon_Bullats;
	if (L"CMon_Cubulon" == _strScriptName)
		return new CMon_Cubulon;
	if (L"CMon_Gigi" == _strScriptName)
		return new CMon_Gigi;
	if (L"CMon_Kin" == _strScriptName)
		return new CMon_Kin;
	if (L"CMon_Maiden" == _strScriptName)
		return new CMon_Maiden;
	if (L"CMovementScript" == _strScriptName)
		return new CMovementScript;
	if (L"CM_Bullet" == _strScriptName)
		return new CM_Bullet;
	if (L"CParticleScript" == _strScriptName)
		return new CParticleScript;
	if (L"CPlayerScript" == _strScriptName)
		return new CPlayerScript;
	if (L"CPlayerStateScript" == _strScriptName)
		return new CPlayerStateScript;
	if (L"CP_Bullet" == _strScriptName)
		return new CP_Bullet;
	if (L"CRoomEvent_1" == _strScriptName)
		return new CRoomEvent_1;
	if (L"CRoomEvent_2" == _strScriptName)
		return new CRoomEvent_2;
	if (L"CRoomEvent_3" == _strScriptName)
		return new CRoomEvent_3;
	if (L"CRoomEvent_4" == _strScriptName)
		return new CRoomEvent_4;
	if (L"CSkull_Bullet" == _strScriptName)
		return new CSkull_Bullet;
	if (L"CTableScript" == _strScriptName)
		return new CTableScript;
	if (L"CWallScript" == _strScriptName)
		return new CWallScript;
	return nullptr;
}

CScript * CScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::AKSCRIPT:
		return new CAKScript;
		break;
	case (UINT)SCRIPT_TYPE::ASTARSCRIPT:
		return new CAStarScript;
		break;
	case (UINT)SCRIPT_TYPE::BARRELSCRIPT:
		return new CBarrelScript;
		break;
	case (UINT)SCRIPT_TYPE::BARRIER_BULLET:
		return new CBarrier_Bullet;
		break;
	case (UINT)SCRIPT_TYPE::BLANKBULLETEFFECT:
		return new CBlankBulletEffect;
		break;
	case (UINT)SCRIPT_TYPE::BLANKBULLETSCRIPT:
		return new CBlankBulletScript;
		break;
	case (UINT)SCRIPT_TYPE::BOSSROOMEVENT:
		return new CBossRoomEvent;
		break;
	case (UINT)SCRIPT_TYPE::BOSSSCRIPT:
		return new CBossScript;
		break;
	case (UINT)SCRIPT_TYPE::BOSS_BULLET:
		return new CBoss_Bullet;
		break;
	case (UINT)SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return new CCameraMoveScript;
		break;
	case (UINT)SCRIPT_TYPE::CHESTSCRIPT:
		return new CChestScript;
		break;
	case (UINT)SCRIPT_TYPE::DOORSCRIPT:
		return new CDoorScript;
		break;
	case (UINT)SCRIPT_TYPE::GLOBESCRIPT:
		return new CGlobeScript;
		break;
	case (UINT)SCRIPT_TYPE::HANDSCRIPT:
		return new CHandScript;
		break;
	case (UINT)SCRIPT_TYPE::HEADGUNSCRIPT:
		return new CHeadGunScript;
		break;
	case (UINT)SCRIPT_TYPE::HEARTSCRIPT:
		return new CHeartScript;
		break;
	case (UINT)SCRIPT_TYPE::ICEGUNSCRIPT:
		return new CIceGunScript;
		break;
	case (UINT)SCRIPT_TYPE::ICE_BULLET:
		return new CIce_Bullet;
		break;
	case (UINT)SCRIPT_TYPE::LAMPSSCRIPT:
		return new CLampsScript;
		break;
	case (UINT)SCRIPT_TYPE::MAGNUMSCRIPT:
		return new CMagnumScript;
		break;
	case (UINT)SCRIPT_TYPE::MAIDENDEAD:
		return new CMaidenDead;
		break;
	case (UINT)SCRIPT_TYPE::MAIDEN_BULLET:
		return new CMaiden_Bullet;
		break;
	case (UINT)SCRIPT_TYPE::MISSILESCRIPT:
		return new CMissileScript;
		break;
	case (UINT)SCRIPT_TYPE::MONSTERSCRIPT:
		return new CMonsterScript;
		break;
	case (UINT)SCRIPT_TYPE::MON_BOOKLETS:
		return new CMon_Booklets;
		break;
	case (UINT)SCRIPT_TYPE::MON_BULLATS:
		return new CMon_Bullats;
		break;
	case (UINT)SCRIPT_TYPE::MON_CUBULON:
		return new CMon_Cubulon;
		break;
	case (UINT)SCRIPT_TYPE::MON_GIGI:
		return new CMon_Gigi;
		break;
	case (UINT)SCRIPT_TYPE::MON_KIN:
		return new CMon_Kin;
		break;
	case (UINT)SCRIPT_TYPE::MON_MAIDEN:
		return new CMon_Maiden;
		break;
	case (UINT)SCRIPT_TYPE::MOVEMENTSCRIPT:
		return new CMovementScript;
		break;
	case (UINT)SCRIPT_TYPE::M_BULLET:
		return new CM_Bullet;
		break;
	case (UINT)SCRIPT_TYPE::PARTICLESCRIPT:
		return new CParticleScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERSCRIPT:
		return new CPlayerScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERSTATESCRIPT:
		return new CPlayerStateScript;
		break;
	case (UINT)SCRIPT_TYPE::P_BULLET:
		return new CP_Bullet;
		break;
	case (UINT)SCRIPT_TYPE::ROOMEVENT_1:
		return new CRoomEvent_1;
		break;
	case (UINT)SCRIPT_TYPE::ROOMEVENT_2:
		return new CRoomEvent_2;
		break;
	case (UINT)SCRIPT_TYPE::ROOMEVENT_3:
		return new CRoomEvent_3;
		break;
	case (UINT)SCRIPT_TYPE::ROOMEVENT_4:
		return new CRoomEvent_4;
		break;
	case (UINT)SCRIPT_TYPE::SKULL_BULLET:
		return new CSkull_Bullet;
		break;
	case (UINT)SCRIPT_TYPE::TABLESCRIPT:
		return new CTableScript;
		break;
	case (UINT)SCRIPT_TYPE::WALLSCRIPT:
		return new CWallScript;
		break;
	}
	return nullptr;
}

const wchar_t * CScriptMgr::GetScriptName(CScript * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::AKSCRIPT:
		return L"CAKScript";
		break;

	case SCRIPT_TYPE::ASTARSCRIPT:
		return L"CAStarScript";
		break;

	case SCRIPT_TYPE::BARRELSCRIPT:
		return L"CBarrelScript";
		break;

	case SCRIPT_TYPE::BARRIER_BULLET:
		return L"CBarrier_Bullet";
		break;

	case SCRIPT_TYPE::BLANKBULLETEFFECT:
		return L"CBlankBulletEffect";
		break;

	case SCRIPT_TYPE::BLANKBULLETSCRIPT:
		return L"CBlankBulletScript";
		break;

	case SCRIPT_TYPE::BOSSROOMEVENT:
		return L"CBossRoomEvent";
		break;

	case SCRIPT_TYPE::BOSSSCRIPT:
		return L"CBossScript";
		break;

	case SCRIPT_TYPE::BOSS_BULLET:
		return L"CBoss_Bullet";
		break;

	case SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return L"CCameraMoveScript";
		break;

	case SCRIPT_TYPE::CHESTSCRIPT:
		return L"CChestScript";
		break;

	case SCRIPT_TYPE::DOORSCRIPT:
		return L"CDoorScript";
		break;

	case SCRIPT_TYPE::GLOBESCRIPT:
		return L"CGlobeScript";
		break;

	case SCRIPT_TYPE::HANDSCRIPT:
		return L"CHandScript";
		break;

	case SCRIPT_TYPE::HEADGUNSCRIPT:
		return L"CHeadGunScript";
		break;

	case SCRIPT_TYPE::HEARTSCRIPT:
		return L"CHeartScript";
		break;

	case SCRIPT_TYPE::ICEGUNSCRIPT:
		return L"CIceGunScript";
		break;

	case SCRIPT_TYPE::ICE_BULLET:
		return L"CIce_Bullet";
		break;

	case SCRIPT_TYPE::LAMPSSCRIPT:
		return L"CLampsScript";
		break;

	case SCRIPT_TYPE::MAGNUMSCRIPT:
		return L"CMagnumScript";
		break;

	case SCRIPT_TYPE::MAIDENDEAD:
		return L"CMaidenDead";
		break;

	case SCRIPT_TYPE::MAIDEN_BULLET:
		return L"CMaiden_Bullet";
		break;

	case SCRIPT_TYPE::MISSILESCRIPT:
		return L"CMissileScript";
		break;

	case SCRIPT_TYPE::MONSTERSCRIPT:
		return L"CMonsterScript";
		break;

	case SCRIPT_TYPE::MON_BOOKLETS:
		return L"CMon_Booklets";
		break;

	case SCRIPT_TYPE::MON_BULLATS:
		return L"CMon_Bullats";
		break;

	case SCRIPT_TYPE::MON_CUBULON:
		return L"CMon_Cubulon";
		break;

	case SCRIPT_TYPE::MON_GIGI:
		return L"CMon_Gigi";
		break;

	case SCRIPT_TYPE::MON_KIN:
		return L"CMon_Kin";
		break;

	case SCRIPT_TYPE::MON_MAIDEN:
		return L"CMon_Maiden";
		break;

	case SCRIPT_TYPE::MOVEMENTSCRIPT:
		return L"CMovementScript";
		break;

	case SCRIPT_TYPE::M_BULLET:
		return L"CM_Bullet";
		break;

	case SCRIPT_TYPE::PARTICLESCRIPT:
		return L"CParticleScript";
		break;

	case SCRIPT_TYPE::PLAYERSCRIPT:
		return L"CPlayerScript";
		break;

	case SCRIPT_TYPE::PLAYERSTATESCRIPT:
		return L"CPlayerStateScript";
		break;

	case SCRIPT_TYPE::P_BULLET:
		return L"CP_Bullet";
		break;

	case SCRIPT_TYPE::ROOMEVENT_1:
		return L"CRoomEvent_1";
		break;

	case SCRIPT_TYPE::ROOMEVENT_2:
		return L"CRoomEvent_2";
		break;

	case SCRIPT_TYPE::ROOMEVENT_3:
		return L"CRoomEvent_3";
		break;

	case SCRIPT_TYPE::ROOMEVENT_4:
		return L"CRoomEvent_4";
		break;

	case SCRIPT_TYPE::SKULL_BULLET:
		return L"CSkull_Bullet";
		break;

	case SCRIPT_TYPE::TABLESCRIPT:
		return L"CTableScript";
		break;

	case SCRIPT_TYPE::WALLSCRIPT:
		return L"CWallScript";
		break;

	}
	return nullptr;
}