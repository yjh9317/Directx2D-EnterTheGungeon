#include "pch.h"
#include "TileMapTex.h"

#include <Engine/CGameObject.h>
#include <Engine/CRenderMgr.h>
#include <Engine/CEventMgr.h>

#include <Engine/CTransform.h>
#include <Engine/CCamera.h>
#include <Engine/CCollider2D.h>
#include <Engine/CMeshRender.h>

#include <Engine/CTexture.h>
#include <Engine/CTileMap.h>

#include "TileMapUI.h"

#include <Script/CWallScript.h>


TileMapTex::TileMapTex()
	:UI("##TileMapTex")
	, m_pCurObj(nullptr)
	, m_pTileMapUI(nullptr)
	, ImageSize{}
	, Pos{}
	, m_iCol(0)
	, m_iRow(0)
	, bGrid(0)
	, bCollider(0)
	, bIdxCheck(0)
	,_col(0)
	,_row(0)

{
}

TileMapTex::~TileMapTex()
{
}


void TileMapTex::update()
{
}

void TileMapTex::render_update()
{
	if (nullptr == m_pCurObj)
		return;


	if (ImGui::BeginChild("##TargetTile", ImVec2(1230.f, 930.f), true, ImGuiWindowFlags_NoMove))
	{

		RTTex();
		ImGui::EndChild();
	}
	
	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	// Grid On,Off 버튼
	ImGui::Text("Grid");
	ImGui::SameLine();
	ImGui::RadioButton("On", &bGrid, 1);
	ImGui::SameLine();
	ImGui::RadioButton("Off", &bGrid, 0);

	ImGui::SameLine(250);
	ImGui::Text("Collider");
	ImGui::SameLine();
	ImGui::RadioButton("On##", &bCollider, 1);
	ImGui::SameLine();
	ImGui::RadioButton("Off##", &bCollider, 0);


	ImGui::SameLine(540);
	ImGui::Text("IdxCheck");
	ImGui::SameLine();
	ImGui::RadioButton("On##Idx", &bIdxCheck, 1);
	ImGui::SameLine();
	ImGui::RadioButton("Off##Idx", &bIdxCheck, 0);

	AddCollider();

	ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 border_col = ImVec4(0.82f, 0.82f, 0.82, 1.f);


	// Slice 타일 하나의 길이 슬라이더
	ImGui::PushItemWidth(200.f);
	ImGui::SetWindowFontScale(1.6f);

	ImGui::SetCursorPos(ImVec2(1250.f, 50.f));

	int Xstep = (int)m_pCurObj->TileMap()->GetSlicePixel().x;
	int Ystep = (int)m_pCurObj->TileMap()->GetSlicePixel().y;

	ImGui::Text("Xstep Slice Size");
	ImGui::SameLine();
	ImGui::SliderInt("##Xstep", &Xstep, 1, 32);

	ImGui::SetCursorPos(ImVec2(1250.f, 80.f));
	ImGui::Text("Ystep Slice Size");
	ImGui::SameLine();
	ImGui::SliderInt("##Ystep", &Ystep, 1, 32);

	m_pCurObj->TileMap()->SetTileSize(Vec2(Xstep, Ystep));


	// Tile Texture Image 
	ImVec2 MousePos = ImGui::GetMousePos();
	ImVec2 WindowPos = ImGui::GetWindowPos();

	static float TextureSize[2] = { 900,900 };

	float Width = m_pCurObj->TileMap()->GetTexture()->Width();
	float Height = m_pCurObj->TileMap()->GetTexture()->Height();

	// 타일을 추출하려는 텍스쳐의 크기 조절
	ImGui::SetCursorPos(ImVec2(1650.f, 50.f));
	ImGui::Text("Texture Width ");
	ImGui::SameLine();
	ImGui::SliderFloat("##TexWidth", &TextureSize[0], 300, 900);

	ImGui::SetCursorPos(ImVec2(1650.f, 80.f));
	ImGui::Text("Texture Height");
	ImGui::SameLine();
	ImGui::SliderFloat("##TexHeight", &TextureSize[1], 300, 900);

	ImGui::SetCursorPos(ImVec2(2050.f, 40.f));

	// 우측 상단위에 현재 사용할 타일 이미지를 띄움
	Vec2 Slice = m_pCurObj->TileMap()->GetSliceUV();
	ImGui::Image((ImTextureID)m_pCurObj->TileMap()->GetTexture()->GetSRV().Get(), ImVec2(75.f, 75.f), ImVec2(m_iRow * Slice.x, m_iCol * Slice.y), ImVec2(m_iRow * Slice.x + Slice.x, m_iCol * Slice.y + Slice.y), tint_col, border_col);

	// Texture Image
	ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
	ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right

	
	ImVec2 CursorPos = ImVec2(1250.f, 120.f);
	draw_list->AddRectFilled(ImVec2(WindowPos.x+CursorPos.x, WindowPos.y+CursorPos.y), ImVec2(WindowPos.x+CursorPos.x + TextureSize[0], WindowPos.y+CursorPos.y + TextureSize[1]), IM_COL32(255, 255, 255, 255));


	ImGui::SetCursorPos(ImVec2(1250.f, 120.f));
	ImVec2 v2 = ImGui::GetCursorPos();

	ImGui::Image((ImTextureID)m_pCurObj->TileMap()->GetTexture()->GetSRV().Get()
		, ImVec2(TextureSize[0], TextureSize[1]), uv_min, uv_max, tint_col, border_col);

	//텍스쳐의 가로,세로 타일 개수
	float TexWidthCnt = Width / m_pCurObj->TileMap()->GetSlicePixel().x;
	float TexHeightCnt = Height / m_pCurObj->TileMap()->GetSlicePixel().y;

	// 타일간의 간격
	float IntervalX = Width / TexWidthCnt * (TextureSize[0] / Width);
	float IntervalY = Height / TexHeightCnt * (TextureSize[1] / Width);

	
	ImGui::SetCursorPos(ImVec2(1300.f, 110.f));

	
	// 타일을 추출하려는 텍스처의 Grid
	for (int i = 0; i <= TexHeightCnt; ++i)
	{
		draw_list->AddLine(ImVec2(v2.x + WindowPos.x, WindowPos.y + v2.y + i * IntervalY), ImVec2(WindowPos.x + v2.x + TextureSize[0], WindowPos.y + v2.y + i * IntervalY), IM_COL32(255, 0, 0, 80));
	}
	for (int j = 0; j <= TexWidthCnt; ++j)
	{
		draw_list->AddLine(ImVec2(v2.x + WindowPos.x + j * IntervalX, v2.y + WindowPos.y), ImVec2(v2.x + WindowPos.x + j * IntervalX, v2.y + WindowPos.y + TextureSize[1]), IM_COL32(255, 0, 0, 80));
	}



	// 마우스 우클릭을 누르면 타일 선택
	ImVec2 TexPos = { WindowPos.x + v2.x ,WindowPos.y + v2.y };
	if (MousePos.x >= TexPos.x && MousePos.y >= TexPos.y && MousePos.x <= TexPos.x + TextureSize[0] && MousePos.y <= TexPos.y + TextureSize[1])
	{
		if (ImGui::IsMouseClicked(1))
		{

			m_iRow = (int)((MousePos.x - WindowPos.x - v2.x) / (m_pCurObj->TileMap()->GetSlicePixel().x * (TextureSize[0] / Width)));
			m_iCol = (int)((MousePos.y - WindowPos.y - v2.y) / (m_pCurObj->TileMap()->GetSlicePixel().y * (TextureSize[1] / Height)));
		}
	}
	//


	CheckColliderIdx();
	
}


void TileMapTex::RTTex()
{
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	ImVec2 uv_min = ImVec2(0.0f, 0.0f);
	ImVec2 uv_max = ImVec2(1.0f, 1.0f);
	ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f);

	ImVec2 MousePos = ImGui::GetMousePos();
	WindowPos = ImGui::GetWindowPos();
	ImageSize = { 1200.f,900.f };



	//렌더타겟텍스쳐
	CTexture* pTex = CResMgr::GetInst()->FindRes<CTexture>(L"RenderTarget").Get();

	//렌더타겟를 복사한 텍스쳐를 이미지 띄우기 
	// Pos는 Collider Idx에서도 사용해야하므로 멤버변수로 변경함
	Pos = ImGui::GetCursorPos();

	ImGui::Image((ImTextureID)pTex->GetSRV().Get()
		, ImageSize, uv_min, uv_max, tint_col, border_col);

	// 카메라 위치와 Width,Height(종횡비)
	CCamera* pCam = CRenderMgr::GetInst()->GetEditCam();

	//Vec3 CamPos = CRenderMgr::GetInst()->GetMainCamPos();
	//float CamWidth = CRenderMgr::GetInst()->GetMainCamWidth();
	//float CamHeight = CRenderMgr::GetInst()->GetMainCamHeight();

	Vec3 CamPos = pCam->Transform()->GetRelativePos();
	float CamWidth = pCam->GetWidth();
	float CamHeight = pCam->GetWidth() / pCam->GetAspectRatio();

	// 이미지 안에서의 마우스 위치값	//  CamPos.x * (ImageSize.x / CamWidth) = 카메라가 움직인만큼 이미지 안에서 카메라도 비율만큼 움직임
	float x = MousePos.x - WindowPos.x - Pos.x + CamPos.x * (ImageSize.x / CamWidth);
	float y = MousePos.y - WindowPos.y - Pos.y - CamPos.y * (ImageSize.y / CamHeight);

	// 타일 위치와 크기 및 타일 가로 세로 개수
	Vec3 TargetPos = m_pCurObj->Transform()->GetWorldPos();
	Vec3 TargetScale = m_pCurObj->Transform()->GetRelativeScale();
	UINT TileCountX = m_pCurObj->TileMap()->GetTileCountX();
	UINT TileCountY = m_pCurObj->TileMap()->GetTileCountY();


	// 타일오브젝트의 중앙부터 가장자리까지 거리값 계산 (CamWidth은 윈도우 가로길이 , CamHeight은 윈도우 세로길이)
	// CamWidth / 2.f, CamHeight / 2.f은 0,0이 중앙이므로 맞춰주기 위해 넣음, (ImageSize.x / CamWidth)은 비율
	float TileLeft = (CamWidth / 2.f + TargetPos.x - TargetScale.x / 2.f) * (ImageSize.x / CamWidth);
	float TileTop = (CamHeight / 2.f - TargetPos.y - TargetScale.y / 2.f) * (ImageSize.y / CamHeight);
	float TileRight = (CamWidth / 2.f + TargetPos.x + TargetScale.x / 2.f) * (ImageSize.x / CamWidth);
	float TileBot = (CamHeight / 2.f - TargetPos.y + TargetScale.y / 2.f) * (ImageSize.y / CamHeight);
	
	// 한 블럭당 크기
	float OneBlockX = (TileRight - TileLeft) / TileCountX;
	float OneBlockY = (TileBot - TileTop) / TileCountY;
	
	// 렌더타겟 텍스쳐 중앙값
	Vec2 TexMid = Vec2((WindowPos.x + ImageSize.x / 2.f) + Pos.x, (WindowPos.y + ImageSize.y / 2.f) + Pos.y);
	

	// 타겟(타일)의 좌상단으로 이동
	Vec2 TileScale = Vec2(TargetScale.x / 2.f, TargetScale.y / 2.f ) * Vec2(ImageSize.x/CamWidth, ImageSize.y /CamHeight);
	Vec2 TileLT = Vec2(TexMid.x - TileScale.x, TexMid.y - TileScale.y);
	
	// 카메라 위치까지 계산
	Vec2 TileCamLT = Vec2(TileLT.x - CamPos.x * (ImageSize.x / CamWidth), TileLT.y + CamPos.y * (ImageSize.y / CamHeight));
	
	Vec3 ObjPos = Vec3(m_pCurObj->Transform()->GetRelativePos().x *(ImageSize[0]/CamWidth), m_pCurObj->Transform()->GetRelativePos().y * (ImageSize[1] / CamHeight), m_pCurObj->Transform()->GetRelativePos().z);
	
	// 렌더타겟에서 타일맵위치에 Grid
	if (bGrid == 1) {
		for (int i = 0; i < (int)TileCountY; ++i)
		{
			draw_list->AddLine(ImVec2(ObjPos.x + TileCamLT.x, TileCamLT.y - ObjPos.y  + i * OneBlockY), ImVec2(ObjPos.x + TileCamLT.x + TileScale.x * 2.f, TileCamLT.y - ObjPos.y + i * OneBlockY), IM_COL32(100, 255, 0, 255), 1.2f);
			for (int j = 0; j < (int)TileCountX; ++j)
			{
				draw_list->AddLine(ImVec2(ObjPos.x  + TileCamLT.x + j * OneBlockX, TileCamLT.y - ObjPos.y ), ImVec2(ObjPos.x  + TileCamLT.x + j * OneBlockX, TileCamLT.y - ObjPos.y  + TileScale.y * 2.f), IM_COL32(100, 255, 0, 255), 1.f);
			}
		}
		draw_list->AddRect(ImVec2(ObjPos.x  + TileCamLT.x, TileCamLT.y - ObjPos.y ), ImVec2(ObjPos.x + TileCamLT.x + TileScale.x * 2.f, TileCamLT.y - ObjPos.y  + TileScale.y * 2.f), IM_COL32(100, 255, 0, 255));
	}
	

	// 마우스 우클릭을 했는데 타일이 있었으면 저장된 타일이 찍힘.
	if (ImGui::IsMouseDown(1))
	{
		// 마우스 위치에서 타일맵 LeftTop을 빼고 블럭크기로 나눠 idx계산
		int IdxX = (int)((x - TileLeft) / OneBlockX);
		int IdxY = (int)((y - TileTop) / OneBlockY);

		// idx가 타일맵 안에 있는 값이여야만 변경
		if (IdxX >= 0 && IdxY >= 0 && IdxX < m_pCurObj->TileMap()->GetTileCountX() && IdxY < m_pCurObj->TileMap()->GetTileCountY())
		{
			if (nullptr != m_pTileMapUI) {
				int tilex = m_pCurObj->TileMap()->GetTileCountX();
				int colcnt = m_pCurObj->TileMap()->GetColCount();
				m_pCurObj->TileMap()->SetTileData(IdxY * tilex + IdxX, m_iCol * colcnt + m_iRow);

			}
		}
	}
}

void TileMapTex::AddCollider()
{
	if (0 == bCollider)	return;

	ImGui::PushItemWidth(200.f);

	ImGui::Text("Name");
	ImGui::SameLine();
	ImGui::InputText("##Name", (char*)Collider_Name.c_str(), 50);
	ImGui::SameLine();
	if (ImGui::Button("Add Collider"))
	{
		CGameObject* pChildObj = new CGameObject;
		string str = Collider_Name.c_str();
		wstring wstr = wstring(str.begin(), str.end());
		pChildObj->SetName(wstr);

		pChildObj->AddComponent(new CTransform);
		pChildObj->AddComponent(new CMeshRender);
		pChildObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"PointMesh"));
		pChildObj->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\Collider2DMtrl.mtrl"));

		Vec2 OneTileSize = Vec2(m_pCurObj->Transform()->GetWorldScale().x / m_pCurObj->TileMap()->GetTileCountX(), m_pCurObj->Transform()->GetWorldScale().y / m_pCurObj->TileMap()->GetTileCountY());
		
		float XOffset = -OneTileSize.x * (m_pCurObj->TileMap()->GetTileCountX() / 2.f) + OneTileSize.x / 2.f;
		float YOffset = OneTileSize.y * (m_pCurObj->TileMap()->GetTileCountY() / 2.f) - OneTileSize.y / 2.f;

		XOffset += _row * OneTileSize.x;
		YOffset -= _col * OneTileSize.y;

		pChildObj->AddComponent(new CCollider2D);
		pChildObj->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::BOX);
		pChildObj->Transform()->SetIgnoreParentScale(true);
		float TileColX =  XOffset;
		float TileColY =  YOffset;

		pChildObj->Transform()->SetRelativePos(Vec3(TileColX, TileColY, 0.f));
		pChildObj->Collider2D()->SetOffsetScale(OneTileSize);
		pChildObj->AddComponent(new CWallScript);

		tEventInfo tEvninfo;
	
		tEvninfo.eType = EVENT_TYPE::CREATE_OBJ;
		tEvninfo.lParam =(DWORD_PTR)pChildObj;
		tEvninfo.wParam = 1;

		CEventMgr::GetInst()->AddEvent(tEvninfo);

		tEvninfo.eType = EVENT_TYPE::ADD_CHILD;
		tEvninfo.lParam = (DWORD_PTR)m_pCurObj;
		tEvninfo.wParam = (DWORD_PTR)pChildObj;

		CEventMgr::GetInst()->AddEvent(tEvninfo);
		bCollider = 1;
	}
}


void TileMapTex::CheckColliderIdx()
{
	if (0 == bIdxCheck)
	{
		_row = 0;
		_col = 0;
		return;
	}
		

	// 인덱스 구하는 값들 RTTex에서 가져옴.

	ImVec2 MousePos = ImGui::GetMousePos();
	
	CCamera* pCam = CRenderMgr::GetInst()->GetEditCam();

	//Vec3 CamPos = CRenderMgr::GetInst()->GetMainCamPos();
	//float CamWidth = CRenderMgr::GetInst()->GetMainCamWidth();
	//float CamHeight = CRenderMgr::GetInst()->GetMainCamHeight();

	Vec3 CamPos = pCam->Transform()->GetRelativePos();
	float CamWidth = pCam->GetWidth();
	float CamHeight = pCam->GetWidth() / pCam->GetAspectRatio();

	float x = MousePos.x - WindowPos.x - Pos.x + CamPos.x * (ImageSize.x / CamWidth);
	float y = MousePos.y - WindowPos.y - Pos.y - CamPos.y * (ImageSize.y / CamHeight);



	Vec3 TargetPos = m_pCurObj->Transform()->GetWorldPos();
	Vec3 TargetScale = m_pCurObj->Transform()->GetRelativeScale();
	UINT TileCountX = m_pCurObj->TileMap()->GetTileCountX();
	UINT TileCountY = m_pCurObj->TileMap()->GetTileCountY();



	float TileLeft = (CamWidth / 2.f + TargetPos.x - TargetScale.x / 2.f) * (ImageSize.x / CamWidth);
	float TileTop = (CamHeight / 2.f - TargetPos.y - TargetScale.y / 2.f) * (ImageSize.y / CamHeight);
	float TileRight = (CamWidth / 2.f + TargetPos.x + TargetScale.x / 2.f) * (ImageSize.x / CamWidth);
	float TileBot = (CamHeight / 2.f - TargetPos.y + TargetScale.y / 2.f) * (ImageSize.y / CamHeight);


	// 한 블럭당 크기
	float OneBlockX = (TileRight - TileLeft) / TileCountX;
	float OneBlockY = (TileBot - TileTop) / TileCountY;

	if (ImGui::IsMouseClicked(2))
	{
		// 마우스 위치에서 타일맵 LeftTop을 빼고 블럭크기로 나눠 idx계산
		_row = (int)((x - TileLeft) / OneBlockX);
		_col = (int)((y - TileTop) / OneBlockY);

		if (_row < 0) _row = 0;
		else if (_row >= TileCountX) _row = TileCountX - 1;
		if (_col < 0) _col = 0;
		else if (_col >= TileCountY) _col = TileCountY - 1;
	}

	ImGui::PushItemWidth(40.f);
	ImGui::SetCursorPos(ImVec2(780.f, 970.f));
	ImGui::Text("_row");
	ImGui::SameLine();
	ImGui::InputInt("##_row", &_row,0,0,ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

	ImGui::SameLine();
	ImGui::Text("_col");
	ImGui::SameLine();
	ImGui::InputInt("##_col", &_col, 0, 0, ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);
}


