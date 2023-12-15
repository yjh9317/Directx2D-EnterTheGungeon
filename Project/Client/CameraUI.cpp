#include "pch.h"
#include "CameraUI.h"


#include <Engine/CCamera.h>

CameraUI::CameraUI()
	: ComponentUI("Camera", COMPONENT_TYPE::CAMERA)
{
    SetSize(Vec2(0.f, 180.f));
}

CameraUI::~CameraUI()
{
}

void CameraUI::update()
{
}

void CameraUI::render_update()
{
    ComponentUI::render_update();

	CCamera* pCameraCom = GetTargetObject()->Camera();

	// PROJECTION TYPE
    ImGui::Text("Projection Type"); 
    ImGui::SameLine();

	PROJ_TYPE eType = pCameraCom->GetProjType();

    const char* szProjType[2] = { "ORTHOGRAPHIC", "PERSPECTIVE" };

    if (ImGui::BeginCombo("##ProjType", szProjType[(int)eType]))
    {
        for (int i = 0; i < 2; i++)
        {            
            if (ImGui::Selectable(szProjType[i]))
            {
                pCameraCom->SetProjType((PROJ_TYPE)i);
            }             
        }
         
        ImGui::EndCombo();
    }

    // 투영 가로길이
    ImGui::Text("Width"); 
    ImGui::SameLine(100); 
    float fWidth = pCameraCom->GetWidth();    

    if (PROJ_TYPE::PERSPECTIVE == eType)
    {
        ImGui::GetStyle().Alpha = 0.25f;
        ImGui::InputFloat("##InputWidth", &fWidth, 0.f, 0.f, "%.3f", ImGuiInputTextFlags_ReadOnly);
        ImGui::GetStyle().Alpha = 1.f;
    }
     
    else
        ImGui::InputFloat("##InputWidth", &fWidth);
    
    pCameraCom->SetWidth(fWidth);

    // 종횡비
    ImGui::Text("AspectRatio");
    ImGui::SameLine(100);
    float fAspectRatio = pCameraCom->GetAspectRatio();
    ImGui::InputFloat("##InputAspectRatio", &fAspectRatio);
    pCameraCom->SetAspectRatio(fAspectRatio);


    // 시야 각      
    ImGui::Text("FOV");
    ImGui::SameLine(100);
    float fFOV = (pCameraCom->GetFOV() / XM_PI) * 180.f;

    if (PROJ_TYPE::ORTHOGRAPHIC == eType)
    {
        ImGui::GetStyle().Alpha = 0.25f;
        ImGui::InputFloat("##InputFOV", &fFOV, 0.f, 0.f, "%.3f", ImGuiInputTextFlags_ReadOnly);
        ImGui::GetStyle().Alpha = 1.f;
    }        
    else
        ImGui::InputFloat("##InputFOV", &fFOV);        
    
    fFOV = (fFOV / 180.f)* XM_PI;
    pCameraCom->SetFOV(fFOV);

    // 시야 거리
    ImGui::Text("Far Distance");
    ImGui::SameLine(100);
    float fFar = pCameraCom->GetFar();
    ImGui::InputFloat("##InputFar", &fFar);
    pCameraCom->SetFar(fFar);


    ImGui::NewLine();

    ImGui::Text("CheckLayer");


    // Combo로 숫자 띄우고 그옆에 체크박스로 숫자에 있는 레이어를 키거나 끄기

    string Items;

    const char* c[MAX_LAYER] = { "0","1", "2", "3", "4", "5", "6", "7", "8", "9",
                                "10","11", "12", "13", "14", "15", "16", "17", "18", "19",
                                "20","21", "22", "23", "24", "25", "26", "27", "28", "29",
                                "30","31" };

    //for문을 이용하여 char*안에 인수를 0~31 (MAX)LAYER까지 채워서 Combo에 전달

    for (int i = 0; i < MAX_LAYER; ++i)
    {
        Items = i;
    }
    ImGui::SameLine();
    ImGui::PushItemWidth(50);

    static int item_current = 0;
    ImGui::Combo("##CheckLayer", &item_current, c, MAX_LAYER);

    ImGui::SameLine();

    if (ImGui::Button("ChangeLayer"))
    {
        pCameraCom->CheckLayerMask(item_current);
    }

    ImGui::NewLine();

    static bool b[MAX_LAYER];
    for (int i = 0; i < MAX_LAYER; ++i) {
        if (pCameraCom->GetLayerMask(i) == 1)
        {
            b[i] = true;
        }
        else
        {
            b[i] = false;
        }
    }

    ImGui::Text("   0   1   2   3   4   5   6   7");

    ImGui::Text("1");
    ImGui::SameLine();
    for (int i = 0; i < MAX_LAYER / 4; ++i)
    {

        ImGui::SameLine(14 + 28.1f * i);

        ImGui::Checkbox("##", &b[i]);
    }

    ImGui::Text("2");
    ImGui::SameLine();
    for (int i = 8; i < MAX_LAYER / 2; ++i)
    {

        ImGui::SameLine(14 + 28.1f * (i % 8));

        ImGui::Checkbox("##", &b[i]);
    }

    ImGui::Text("3");
    ImGui::SameLine();
    for (int i = 16; i < MAX_LAYER * 3 / 4; ++i)
    {

        ImGui::SameLine(14 + 28.1f * (i % 16));

        ImGui::Checkbox("##", &b[i]);
    }

    ImGui::Text("4");
    ImGui::SameLine();
    for (int i = 24; i < MAX_LAYER; ++i)
    {

        ImGui::SameLine(14 + 28.1f * (i % 24));

        ImGui::Checkbox("##", &b[i]);
    }
}
