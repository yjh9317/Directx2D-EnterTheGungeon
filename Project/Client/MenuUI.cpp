#include "pch.h"
#include "MenuUI.h"

#include <Engine/CCore.h>
#include <Engine/CPathMgr.h>

#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>
#include <Engine/CLayer.h>
#include <Script/CScriptMgr.h>

#include <Engine/CGameObject.h>
#include <Engine/CTransform.h>
#include <Engine/CEventMgr.h>
#include <Engine/CCollisionMgr.h>
#include <Engine/CScript.h>

#include <Script/CSceneSaveLoad.h>
#include "CImGuiMgr.h"
#include "SceneOutliner.h"
#include "InspectorUI.h"

#include <Engine/CAnimator2D.h>
#include <Engine/CLight2D.h>
#include <Engine/CCamera.h>
#include <Engine/CCollider2D.h>
#include <Engine/CMeshRender.h>
#include <Engine/CTileMap.h>
#include <Engine/CParticleSystem.h>


MenuUI::MenuUI()
	: UI("Menu")
    , m_bPackaging(false)
    , m_bSceneSave(false)
    , m_bSceneLoad(false)
    , m_bScenePlayPause(false)
    , m_bSceneStop(false)
    , m_bObjCreate(false)
    , m_Layer(0)
    , m_MtrlCheck(false)
    , m_CamCheck(false)
    , m_Col2DCheck(false)
    , m_Anim2DCheck(false)
    , m_Light2DCheck(false)
    , m_bCheck(false)
    , m_ScriptCheck(false)
    , m_ScriptIdx(-1)
    , LayerCheck{false,}
{
}

MenuUI::~MenuUI()
{
}

void MenuUI::update()
{
    Task();
    ComponenetCheck();
    ScriptCheck();
}

void MenuUI::render()
{    
    if (ImGui::BeginMainMenuBar())
    {
        render_update();       

        ImGui::EndMainMenuBar();
    }
}

void MenuUI::render_update()
{
    if (ImGui::BeginMenu("File"))
    {
        ImGui::MenuItem("Packaging", NULL, &m_bPackaging);


        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Scene"))
    {
        ImGui::MenuItem("Scene Save", NULL, &m_bSceneSave);
        ImGui::MenuItem("Scene Load", NULL, &m_bSceneLoad);

        CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
        SCENE_STATE eState = pCurScene->GetSceneState();

        if (SCENE_STATE::PLAY == eState)
            m_strPlayPause = "Pause";
        else
            m_strPlayPause = "Play";

        ImGui::MenuItem(m_strPlayPause.c_str(), NULL, &m_bScenePlayPause);

        if (SCENE_STATE::STOP == eState)
            ImGui::MenuItem("Stop", NULL, &m_bSceneStop, false);
        else
            ImGui::MenuItem("Stop", NULL, &m_bSceneStop, true);

        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Component"))
    {
        if (ImGui::BeginMenu("Add Component"))
        {
            ImGui::MenuItem("MeshRender", NULL, &m_MtrlCheck);
            ImGui::MenuItem("Camera", NULL, &m_CamCheck);
            ImGui::MenuItem("Collider2D", NULL, &m_Col2DCheck);
            ImGui::MenuItem("Collider3D", NULL);
            ImGui::MenuItem("Animator2D", NULL, &m_Anim2DCheck);
            ImGui::MenuItem("Animator3D", NULL);
            ImGui::MenuItem("Light2D", NULL, &m_Light2DCheck);
            ImGui::MenuItem("Light3D", NULL);
            ImGui::MenuItem("TileMap", NULL, &m_TileMapCheck);
            ImGui::MenuItem("Particle", NULL, &m_ParticleCheck);
            

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Add Script"))
        {
            vector<wstring> vecScriptName;
            CScriptMgr::GetScriptInfo(vecScriptName);
            for (size_t i = 0; i < vecScriptName.size(); ++i)
            {
                string strScriptName = string(vecScriptName[i].begin(), vecScriptName[i].end());
                ImGui::MenuItem(strScriptName.c_str(), NULL, &m_ScriptCheck);

                if (m_ScriptCheck)
                {
                    m_ScriptIdx = i;
                    m_ScriptCheck = false;
                }

            }
            ImGui::EndMenu();
        }

        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Object"))
    {
        if (ImGui::BeginMenu("Create")) {
            ImGui::PushItemWidth(150.f);
            ImGui::Text("Name ");
            ImGui::SameLine();
            ImGui::InputText("##ObjCreate", (char*)m_ObjName.c_str(), 50);

            ImGui::PushItemWidth(50.f);
            ImGui::Text("Layer");
            ImGui::SameLine();
            ImGui::InputInt("##ObjLayer", &m_Layer, 0, 0);

            if (m_Layer < 0) m_Layer = 0;
            if (m_Layer > 31) m_Layer = 31;

            ImGui::SameLine();
            if (ImGui::Button("Create"))
            {
                CGameObject* pObj = new CGameObject;
                pObj->AddComponent(new CTransform);

                string str = m_ObjName.c_str();
                wstring wstr = wstring(str.begin(), str.end());

                pObj->SetName(wstr);
                SceneOutliner* pUI = (SceneOutliner*)CImGuiMgr::GetInst()->FindUI("SceneOutliner");

                pUI->m_TreeUI->AddTreeNode(nullptr, m_ObjName, (DWORD_PTR)pObj);

                tEventInfo evninfo;
                evninfo.eType = EVENT_TYPE::CREATE_OBJ;
                evninfo.lParam = (DWORD_PTR)pObj;
                evninfo.wParam = m_Layer;

                CEventMgr::GetInst()->AddEvent(evninfo);
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Layer_Collision"))
    {
        if (ImGui::BeginMenu("Layer"))
        {
			UINT* ColArr = CCollisionMgr::GetInst()->GetLayerArr();
			CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
			static int layer = 12;

            //ImGui::SliderInt("##ColArr", &layer, 0, 12);

			string s;
            string s2;
            ImGui::NewLine();
            
            //행
            for (int j = 0; j <= layer; ++j)
            {
                CLayer* pLayer = pCurScene->GetLayer(j);
                s2 =  to_string(j);

                ImGui::SameLine(65 + j*27);
                ImGui::Text(s2.c_str());
                
            }

            //열
            for (int i = 0; i <= layer; ++i) {
                CLayer* pLayer = pCurScene->GetLayer(i);
                if (L"" == pLayer->GetName())
                    s = to_string(i);
                else
                    s = string(pLayer->GetName().begin(), pLayer->GetName().end());

                ImGui::Text(s.c_str());

                string tmp = "##" + to_string(i);
                
                UINT* arr = CCollisionMgr::GetInst()->GetLayerArr();
                ImGui::SameLine(65);

                for (UINT l = 0; l <= layer - i; ++l)
                {
                    UINT check = *(arr+i);
                    
                    tmp += to_string(l);

                    if (check > 0)
                    {
                        for (int m = 1; m <= pow(2,layer); m*=2)
                        {
                            int log2_m = log2(m);
                            if ((m & check) > 0)
                            {
                                LayerCheck[i][log2_m] = true;
                                LayerCheck[log2_m][i] = true;
                            }
                        }
                    }
                    ImGui::Checkbox(tmp.c_str(), &LayerCheck[i][l]);
					ImGui::SameLine();
                }
                ImGui::NewLine();
            }
			ImGui::EndMenu();
		}
		ImGui::EndMenu();
    }
}



void MenuUI::Task()
{
    if (m_bSceneSave)
    {
        wchar_t szName[256] = {};

        OPENFILENAME ofn = {};

        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = CCore::GetInst()->GetMainHwnd();
        ofn.lpstrFile = szName;
        ofn.nMaxFile = sizeof(szName);
        ofn.lpstrFilter = L"ALL\0*.*\0Scene\0*.scene\0";
        ofn.nFilterIndex = 0;
        ofn.lpstrFileTitle = nullptr;
        ofn.nMaxFileTitle = 0;

        wstring strTileFolder = CPathMgr::GetInst()->GetContentPath();
        strTileFolder += L"\scene";

        ofn.lpstrInitialDir = strTileFolder.c_str();
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

        // Modal
        if (GetSaveFileName(&ofn))
        {            
            CSceneSaveLoad::SaveScene(CSceneMgr::GetInst()->GetCurScene(), szName);            
        }

        m_bSceneSave = false;
    }

    else if (m_bSceneLoad)
    {
        wchar_t szName[256] = {};

        OPENFILENAME ofn = {};

        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = CCore::GetInst()->GetMainHwnd();
        ofn.lpstrFile = szName;
        ofn.nMaxFile = sizeof(szName);
        ofn.lpstrFilter = L"ALL\0*.*\0Scene\0*.scene\0";
        ofn.nFilterIndex = 0;
        ofn.lpstrFileTitle = nullptr;
        ofn.nMaxFileTitle = 0;

        wstring strTileFolder = CPathMgr::GetInst()->GetContentPath();
        strTileFolder += L"scene\\";

        ofn.lpstrInitialDir = strTileFolder.c_str();
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

        // Modal
        if (GetOpenFileName(&ofn))
        {
            CScene* pLoadScene = CSceneSaveLoad::LoadScene(szName);
            CSceneMgr::GetInst()->ChangeScene(pLoadScene);
        }

        // SceneOutliner 갱신
        SceneOutliner* pUI = (SceneOutliner*)CImGuiMgr::GetInst()->FindUI("SceneOutliner");
        pUI->Reset();

        m_bSceneLoad = false;
    }

    if (m_bScenePlayPause)
    {
        CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
        SCENE_STATE eState = pCurScene->GetSceneState();

        if (SCENE_STATE::STOP == eState)
        {
            pCurScene->SetSceneState(SCENE_STATE::PLAY);            
        }
        else if (SCENE_STATE::PLAY == eState)
        {
            pCurScene->SetSceneState(SCENE_STATE::PAUSE);
        }
        else if (SCENE_STATE::PAUSE == eState)
        {
            pCurScene->SetSceneState(SCENE_STATE::PLAY);
        }

        m_bScenePlayPause = false;
    }

    if (m_bSceneStop)
    {
        CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
        SCENE_STATE eState = pCurScene->GetSceneState();

        if (SCENE_STATE::STOP != eState)
        {
            pCurScene->SetSceneState(SCENE_STATE::STOP);
            CSceneFile* pSceneFile = pCurScene->GetSceneFile().Get();

            wstring strFilePath = CPathMgr::GetInst()->GetContentPath() + pSceneFile->GetRelativePath();
            CScene* pNewScene = CSceneSaveLoad::LoadScene(strFilePath);
            CSceneMgr::GetInst()->ChangeScene(pNewScene);
        }

        // SceneOutliner 갱신
        ((SceneOutliner*)CImGuiMgr::GetInst()->FindUI("SceneOutliner"))->Reset();

        m_bSceneStop = false;
    }
}

void MenuUI::ComponenetCheck()
{
    SceneOutliner* pSceneUI = (SceneOutliner*)CImGuiMgr::GetInst()->FindUI("SceneOutliner");
    TreeNode* pSelectedNode = pSceneUI->m_TreeUI->GetSelectedNode();

    if (nullptr == pSelectedNode)
        return;
    
    CGameObject* pObj =(CGameObject*)pSelectedNode->GetData();


    if (m_MtrlCheck)
    {
        if (nullptr == pObj->MeshRender())
        {
            pObj->AddComponent(new CMeshRender);
        }
        m_MtrlCheck = false;
        m_bCheck = true;
    }
    else if (m_CamCheck)
    {
        if (nullptr == pObj->Camera())
        {
            pObj->AddComponent(new CCamera);
        }
        m_CamCheck = false;
        m_bCheck = true;
    }
    else if (m_Col2DCheck)
    {
        if (nullptr == pObj->Collider2D())
        {
            pObj->AddComponent(new CCollider2D);
        }
        m_Col2DCheck = false;
        m_bCheck = true;
    }
    else if (m_Anim2DCheck)
    {
        if (nullptr == pObj->Animator2D())
        {
            pObj->AddComponent(new CAnimator2D);
        }
        m_Anim2DCheck = false;
        m_bCheck = true;
    }
    else if (m_Light2DCheck)
    {
        if (nullptr == pObj->Light2D())
        {
            pObj->AddComponent(new CLight2D);
        }
        m_Light2DCheck = false;
        m_bCheck = true;
    }
    else if (m_TileMapCheck)
    {
        if (nullptr == pObj->TileMap())
        {
            pObj->AddComponent(new CTileMap);
            pObj->Transform()->SetRelativeScale(1000.f, 1000.f, 1.f);
            Ptr<CTexture> pTileAtlas = CResMgr::GetInst()->Load<CTexture>(L"texture\\Map\\map1.png", L"texture\\Map\\map1.png");
            pObj->TileMap()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
            pObj->TileMap()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\TileMapMtrl.mtrl"));
            pObj->TileMap()->SetAtlasTex(pTileAtlas);
            pObj->TileMap()->SetTileSize(Vec2(18.f, 18.f));
            pObj->TileMap()->SetTileMapCount(16, 16);

        }
        m_TileMapCheck = false;
        m_bCheck = true;
    }
    else if (m_ParticleCheck)
    {
        if (nullptr == pObj->ParticleSystem())
        {
            pObj->AddComponent(new CParticleSystem);
        }
        m_ParticleCheck = false;
        m_bCheck = true;
    }

    //// InspectorUI UI 갱신
    if (m_bCheck)
    {
        InspectorUI* pInspectorUI = (InspectorUI*)CImGuiMgr::GetInst()->FindUI("Inspector");
        pInspectorUI->SetTargetObject(pObj);
        m_bCheck = false;
    }
}

void MenuUI::ScriptCheck()
{
    SceneOutliner* pSceneUI = (SceneOutliner*)CImGuiMgr::GetInst()->FindUI("SceneOutliner");
    TreeNode* pSelectedNode = pSceneUI->m_TreeUI->GetSelectedNode();

    if (nullptr == pSelectedNode || m_ScriptIdx == -1)
        return;

    CGameObject* pObj = (CGameObject*)pSelectedNode->GetData();


    CComponent* pScript = (CComponent*)CScriptMgr::GetScript(m_ScriptIdx);
    pScript->Activate();
    pObj->AddComponent(pScript);

    m_ScriptIdx = -1;

    // InspectorUI UI 갱신

    pSceneUI->Reset();
    //InspectorUI* pInspectorUI = (InspectorUI*)CImGuiMgr::GetInst()->FindUI("Inspector");
    //pInspectorUI->SetTargetObject(pObj);
}
