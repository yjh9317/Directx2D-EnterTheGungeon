#include "pch.h"
#include "CImGuiMgr.h"

#include <Engine/CDevice.h>
#include <Engine/CGameObject.h>
#include <Engine/CSceneMgr.h>

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"

#include "UI.h"
#include "ParamUI.h"



CImGuiMgr::CImGuiMgr()
{

}

CImGuiMgr::~CImGuiMgr()
{
    Safe_Del_Map(m_mapUI);
}

void CImGuiMgr::init(HWND _hwnd)
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;
    //io.ConfigViewportsNoDefaultParent = true;
    //io.ConfigDockingAlwaysTabBar = true;
    //io.ConfigDockingTransparentPayload = true;
    //io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;     // FIXME-DPI: Experimental. THIS CURRENTLY DOESN'T WORK AS EXPECTED. DON'T USE IN USER APP!
    //io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports; // FIXME-DPI: Experimental.

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(_hwnd);
    ImGui_ImplDX11_Init(DEVICE, CONTEXT);

    // �⺻ UI �� ����
    CreateUI();

    // �˸�����  
    wstring strPath  = CPathMgr::GetInst()->GetContentPath();
    m_hNotify = FindFirstChangeNotification(strPath.c_str(), TRUE, 
        FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME | FILE_ACTION_ADDED | FILE_ACTION_REMOVED);
}

void CImGuiMgr::progress()
{
    // Start the Dear ImGui frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    // Param Init
    ParamUI::KeyCount = 0;

    // UI Update
    for (auto& pair : m_mapUI)
    {
        pair.second->update();
    }

    // UI Render
    for (auto& pair : m_mapUI)
    {
        pair.second->render();
    }

    bool bOpen = true;
    //ImGui::ShowDemoWindow(&bOpen);

    // Delegate ȣ��
    for (size_t i = 0; i < m_vecDelegate.size(); ++i)
    {
        (m_vecDelegate[i].pInst->*m_vecDelegate[i].pFunc)(m_vecDelegate[i].dwParam);
    }

    m_vecDelegate.clear();

    // Content ���� ����
    ObserveContent();
}





void CImGuiMgr::render()
{
    // Rendering
    ImGui::Render();   
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}



void CImGuiMgr::clear()
{
    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

#include "MenuUI.h"
#include "InspectorUI.h"
#include "ResourceUI.h"
#include "SceneOutliner.h"
#include "ListUI.h"

#include "AnimationUI.h"
#include "TileMapTex.h"

void CImGuiMgr::CreateUI()
{
    // MenuBar ����
    MenuUI* pMenuUI = new MenuUI;
    pMenuUI->Activate();
    m_mapUI.insert(make_pair(pMenuUI->GetName(), pMenuUI));

    // InspectorUI ����
    InspectorUI* pUI = new InspectorUI;        
    pUI->SetTargetObject(nullptr);
    m_mapUI.insert(make_pair(pUI->GetName(), pUI));

    // ResourceUI ����
    ResourceUI* pResUI = new ResourceUI;    
    m_mapUI.insert(make_pair(pResUI->GetName(), pResUI));

    // SceneOutLiner
    SceneOutliner* pSceneOutliner = new SceneOutliner;
    m_mapUI.insert(make_pair(pSceneOutliner->GetName(), pSceneOutliner));

    // ListUI ����
    ListUI* pListUI = new ListUI;    
    pListUI->Deactivate();
    pListUI->SetModalUI(true);    
    m_mapUI.insert(make_pair(pListUI->GetName(), pListUI));

    AnimationUI* pAnimationUI = new AnimationUI;
    pAnimationUI->Deactivate();
    pAnimationUI->SetModalUI(true);
    m_mapUI.insert(make_pair(pAnimationUI->GetName(), pAnimationUI));

    TileMapTex* pTileMapTex = new TileMapTex;
    pTileMapTex->Deactivate();
    m_mapUI.insert(make_pair(pTileMapTex->GetName(), pTileMapTex));


}


UI* CImGuiMgr::FindUI(const string& _strKey)
{
    map<string, UI*>::iterator iter = m_mapUI.find(_strKey);

    if (m_mapUI.end() == iter)
    {
        return nullptr;
    }

    return iter->second;
}



void CImGuiMgr::ObserveContent()
{
    DWORD dwWaitStatus = WaitForSingleObject(m_hNotify, 0);

    if (dwWaitStatus == WAIT_OBJECT_0)
    {
        ResourceUI* pResUI = (ResourceUI*)FindUI("Resource");
        pResUI->Reset();

        wstring strPath = CPathMgr::GetInst()->GetContentPath();
        FindNextChangeNotification(m_hNotify);        
    }
}
