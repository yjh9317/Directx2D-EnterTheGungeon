#include "pch.h"
#include "CTestScene.h"

#include <Engine/CResMgr.h>
#include <Engine/CCollisionMgr.h>

#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>

#include <Engine/CTransform.h>
#include <Engine/CMeshRender.h>
#include <Engine/CCollider2D.h>
#include <Engine/CParticleSystem.h>
#include <Engine/CCamera.h>
#include <Engine/CTileMap.h>
#include <Engine/CLight2D.h>
#include <Engine/CAnimator2D.h>

#include <Engine/CSceneFile.h>

#include <Script/CPlayerScript.h>
#include <Script/CCameraMoveScript.h>
#include <Script/CMissileScript.h>

#include <Script\CSceneSaveLoad.h>

void CTestScene::CreateTestScene()
{
	//CResMgr::GetInst()->Load<CSceneFile>(L"scene\\Test.scene", L"scene\\Test.scene");
	//CResMgr::GetInst()->Load<CSceneFile>(L"scene\\Test_01.scene", L"scene\\Test_01.scene");

	CScene* pCurScene = new CScene;
	CSceneMgr::GetInst()->ChangeScene(pCurScene);		

	
	pCurScene->SetLayerName(0, L"Default");
	pCurScene->SetLayerName(1, L"Tile");
	pCurScene->SetLayerName(2, L"Tile_deco");
	pCurScene->SetLayerName(3, L"Player");
	pCurScene->SetLayerName(4, L"P_Bullet");
	pCurScene->SetLayerName(5, L"Monster");
	pCurScene->SetLayerName(6, L"M_Bullet");


	
	// Camera Object 추가
	CGameObject* pCamObj = new CGameObject;
	pCamObj->SetName(L"MainCamera");
	pCamObj->AddComponent(new CTransform);
	pCamObj->AddComponent(new CCamera);
	pCamObj->AddComponent(new CCameraMoveScript);

	pCamObj->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	pCamObj->Camera()->SetCameraAsMain();
	pCamObj->Camera()->CheckLayerMaskAll();
	
	pCurScene->AddObject(pCamObj, L"Default");


	// 광원 오브젝트 추가
	CGameObject* pLight2D = new CGameObject;
	pLight2D->SetName(L"DirLight");

	pLight2D->AddComponent(new CTransform);
	pLight2D->AddComponent(new CMeshRender);
	pLight2D->AddComponent(new CLight2D);

	pLight2D->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));

	pLight2D->Light2D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);	
	pLight2D->Light2D()->SetDiffuse(Vec3(0.2f, 0.2f, 0.2f));

	pCurScene->AddObject(pLight2D, L"Default");


	//pLight2D = new CGameObject;
	//pLight2D->SetName(L"PointLight");

	//pLight2D->AddComponent(new CTransform);
	//pLight2D->AddComponent(new CMeshRender);
	//pLight2D->AddComponent(new CLight2D);

	//pLight2D->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));

	//pLight2D->Light2D()->SetLightType(LIGHT_TYPE::POINT);
	//pLight2D->Light2D()->SetRange(500.f);
	//pLight2D->Light2D()->SetDiffuse(Vec3(1.f, 1.f, 1.f));	

	//pCurScene->AddObject(pLight2D, L"Default");




	//Tile
	CGameObject* pObject = new CGameObject;
	pObject->SetName(L"Stage01_Start");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CTileMap);
	
	pObject->Transform()->SetRelativePos(Vec3(0.f, 0.f, 900.f));
	pObject->Transform()->SetRelativeScale(1000.f, 1000.f, 1.f);
	
	
	pObject->TileMap()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->TileMap()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\TileMapMtrl.mtrl"));
	Ptr<CTexture> pTileAtlas = CResMgr::GetInst()->Load<CTexture>(L"texture\\Map\\map1.png", L"texture\\Map\\map1.png");
	pObject->TileMap()->SetAtlasTex(pTileAtlas);
	pObject->TileMap()->SetTileSize(Vec2(18.f, 18.f));
	pObject->TileMap()->SetTileMapCount(8, 8);

	for (int i = 0; i < 64; ++i)
	{
		pObject->TileMap()->SetTileData(i, 0);
	}

	//pObject->TileMap()->SetTileData(7, -1);
	pCurScene->AddObject(pObject, L"Tile");


	// Particle Object 
	//CGameObject* pParticleObj = new CGameObject;
	//pParticleObj->SetName(L"ParticleObject_01");

	//pParticleObj->AddComponent(new CTransform);
	//pParticleObj->AddComponent(new CParticleSystem);

	//pParticleObj->Transform()->SetRelativePos(0.f, 0.f, 500.f);

	//Ptr<CTexture> pParticleTex = CResMgr::GetInst()->Load<CTexture>(L"texture\\particle\\AlphaCircle.png", L"texture\\particle\\AlphaCircle.png");
	//pParticleObj->ParticleSystem()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, pParticleTex);

	//pCurScene->AddObject(pParticleObj, L"Default");






	//PostProcess Object
	CGameObject* pPostProcess = new CGameObject;
	pPostProcess->SetName(L"PostProcessObject");

	pPostProcess->AddComponent(new CTransform);
	pPostProcess->AddComponent(new CMeshRender);


	//pPostProcess->Transform()->SetRelativePos(0.f, 0.f, 100.f);
	//pPostProcess->Transform()->SetRelativeScale(200.f, 200.f, 1.f);

	pPostProcess->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pPostProcess->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\PostProcessMtrl.mtrl"));
	

	pCurScene->AddObject(pPostProcess, L"Default");
	pPostProcess->Deactivate();


	((CCameraMoveScript*)pCamObj->GetScript(0))->SetFilter(pPostProcess);
	//CCameraMoveScript* pCamMoveScript = pCamObj->GetScript<CCameraMoveScript>();
	
	//플레이어
	//pObject = new CGameObject;
	//pObject->SetName(L"Player");
	//pObject->AddComponent(new CTransform);
	//pObject->AddComponent(new CMeshRender);
	//pObject->AddComponent(new CAnimator2D);
	//pObject->AddComponent(new CCollider2D);
	//pObject->AddComponent(new CPlayerScript);

	//pObject->Transform()->SetRelativePos(0.f, 0.f, 500.f);
	//pObject->Transform()->SetRelativeScale(Vec3(300.f, 300.f, 1.f));


	//

	//pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//pObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\Std2DMtrl.mtrl"));
	//

	//pObject->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::BOX);
	//pObject->Collider2D()->SetOffsetPos(Vec2(0.f, 0.f));
	//pObject->Collider2D()->SetOffsetScale(Vec2(100.f, 100.f));


	//Ptr<CTexture> pTexture = CResMgr::GetInst()->Load<CTexture>(L"texture\\Player\\link_0.png", L"texture\\Player\\link_0.png");
	//pObject->Animator2D()->CreateAnim(L"Player_Anim", pTexture, Vec2(70.f, 70.f), Vec2(0.f, 265.f), Vec2(60.f, 65.f), Vec2(60.f, 0.f), 0.1f, 10);

	//pObject->Animator2D()->Play(L"Player_Anim", true);

	////CResMgr::GetInst()->AddRes<CPrefab>(L"PlayerPrefab", new CPrefab(pObject));

	//pCurScene->AddObject(pObject, L"Player");


	// 충돌 레이어 설정
	//CCollisionMgr::GetInst()->CollisionCheck(L"Player", L"Monster");
	CCollisionMgr::GetInst()->CollisionCheck(L"Tile", L"Player");
	CCollisionMgr::GetInst()->CollisionCheck(L"Tile", L"Monster");
	CCollisionMgr::GetInst()->CollisionCheck(L"Tile", L"M_Bullet");
	CCollisionMgr::GetInst()->CollisionCheck(L"Monster", L"Player");
	CCollisionMgr::GetInst()->CollisionCheck(L"M_Bullet", L"Player");
	CCollisionMgr::GetInst()->CollisionCheck(L"P_Bullet", L"Monster");

	
	pCurScene->SetResKey(L"scene\\TestScene.scene");
	wstring strSceneFilePath = CPathMgr::GetInst()->GetContentPath();	
	CSceneSaveLoad::SaveScene(pCurScene, strSceneFilePath + L"scene\\TestScene.scene");
	
	pCurScene->start();	
	pCurScene->SetSceneState(SCENE_STATE::STOP);
}
