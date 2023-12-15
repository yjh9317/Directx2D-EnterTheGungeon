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

	// Grid On,Off ��ư
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


	// Slice Ÿ�� �ϳ��� ���� �����̴�
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

	// Ÿ���� �����Ϸ��� �ؽ����� ũ�� ����
	ImGui::SetCursorPos(ImVec2(1650.f, 50.f));
	ImGui::Text("Texture Width ");
	ImGui::SameLine();
	ImGui::SliderFloat("##TexWidth", &TextureSize[0], 300, 900);

	ImGui::SetCursorPos(ImVec2(1650.f, 80.f));
	ImGui::Text("Texture Height");
	ImGui::SameLine();
	ImGui::SliderFloat("##TexHeight", &TextureSize[1], 300, 900);

	ImGui::SetCursorPos(ImVec2(2050.f, 40.f));

	// ���� ������� ���� ����� Ÿ�� �̹����� ���
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

	//�ؽ����� ����,���� Ÿ�� ����
	float TexWidthCnt = Width / m_pCurObj->TileMap()->GetSlicePixel().x;
	float TexHeightCnt = Height / m_pCurObj->TileMap()->GetSlicePixel().y;

	// Ÿ�ϰ��� ����
	float IntervalX = Width / TexWidthCnt * (TextureSize[0] / Width);
	float IntervalY = Height / TexHeightCnt * (TextureSize[1] / Width);

	
	ImGui::SetCursorPos(ImVec2(1300.f, 110.f));

	
	// Ÿ���� �����Ϸ��� �ؽ�ó�� Grid
	for (int i = 0; i <= TexHeightCnt; ++i)
	{
		draw_list->AddLine(ImVec2(v2.x + WindowPos.x, WindowPos.y + v2.y + i * IntervalY), ImVec2(WindowPos.x + v2.x + TextureSize[0], WindowPos.y + v2.y + i * IntervalY), IM_COL32(255, 0, 0, 80));
	}
	for (int j = 0; j <= TexWidthCnt; ++j)
	{
		draw_list->AddLine(ImVec2(v2.x + WindowPos.x + j * IntervalX, v2.y + WindowPos.y), ImVec2(v2.x + WindowPos.x + j * IntervalX, v2.y + WindowPos.y + TextureSize[1]), IM_COL32(255, 0, 0, 80));
	}



	// ���콺 ��Ŭ���� ������ Ÿ�� ����
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



	//����Ÿ���ؽ���
	CTexture* pTex = CResMgr::GetInst()->FindRes<CTexture>(L"RenderTarget").Get();

	//����Ÿ�ٸ� ������ �ؽ��ĸ� �̹��� ���� 
	// Pos�� Collider Idx������ ����ؾ��ϹǷ� ��������� ������
	Pos = ImGui::GetCursorPos();

	ImGui::Image((ImTextureID)pTex->GetSRV().Get()
		, ImageSize, uv_min, uv_max, tint_col, border_col);

	// ī�޶� ��ġ�� Width,Height(��Ⱦ��)
	CCamera* pCam = CRenderMgr::GetInst()->GetEditCam();

	//Vec3 CamPos = CRenderMgr::GetInst()->GetMainCamPos();
	//float CamWidth = CRenderMgr::GetInst()->GetMainCamWidth();
	//float CamHeight = CRenderMgr::GetInst()->GetMainCamHeight();

	Vec3 CamPos = pCam->Transform()->GetRelativePos();
	float CamWidth = pCam->GetWidth();
	float CamHeight = pCam->GetWidth() / pCam->GetAspectRatio();

	// �̹��� �ȿ����� ���콺 ��ġ��	//  CamPos.x * (ImageSize.x / CamWidth) = ī�޶� �����θ�ŭ �̹��� �ȿ��� ī�޶� ������ŭ ������
	float x = MousePos.x - WindowPos.x - Pos.x + CamPos.x * (ImageSize.x / CamWidth);
	float y = MousePos.y - WindowPos.y - Pos.y - CamPos.y * (ImageSize.y / CamHeight);

	// Ÿ�� ��ġ�� ũ�� �� Ÿ�� ���� ���� ����
	Vec3 TargetPos = m_pCurObj->Transform()->GetWorldPos();
	Vec3 TargetScale = m_pCurObj->Transform()->GetRelativeScale();
	UINT TileCountX = m_pCurObj->TileMap()->GetTileCountX();
	UINT TileCountY = m_pCurObj->TileMap()->GetTileCountY();


	// Ÿ�Ͽ�����Ʈ�� �߾Ӻ��� �����ڸ����� �Ÿ��� ��� (CamWidth�� ������ ���α��� , CamHeight�� ������ ���α���)
	// CamWidth / 2.f, CamHeight / 2.f�� 0,0�� �߾��̹Ƿ� �����ֱ� ���� ����, (ImageSize.x / CamWidth)�� ����
	float TileLeft = (CamWidth / 2.f + TargetPos.x - TargetScale.x / 2.f) * (ImageSize.x / CamWidth);
	float TileTop = (CamHeight / 2.f - TargetPos.y - TargetScale.y / 2.f) * (ImageSize.y / CamHeight);
	float TileRight = (CamWidth / 2.f + TargetPos.x + TargetScale.x / 2.f) * (ImageSize.x / CamWidth);
	float TileBot = (CamHeight / 2.f - TargetPos.y + TargetScale.y / 2.f) * (ImageSize.y / CamHeight);
	
	// �� ���� ũ��
	float OneBlockX = (TileRight - TileLeft) / TileCountX;
	float OneBlockY = (TileBot - TileTop) / TileCountY;
	
	// ����Ÿ�� �ؽ��� �߾Ӱ�
	Vec2 TexMid = Vec2((WindowPos.x + ImageSize.x / 2.f) + Pos.x, (WindowPos.y + ImageSize.y / 2.f) + Pos.y);
	

	// Ÿ��(Ÿ��)�� �»������ �̵�
	Vec2 TileScale = Vec2(TargetScale.x / 2.f, TargetScale.y / 2.f ) * Vec2(ImageSize.x/CamWidth, ImageSize.y /CamHeight);
	Vec2 TileLT = Vec2(TexMid.x - TileScale.x, TexMid.y - TileScale.y);
	
	// ī�޶� ��ġ���� ���
	Vec2 TileCamLT = Vec2(TileLT.x - CamPos.x * (ImageSize.x / CamWidth), TileLT.y + CamPos.y * (ImageSize.y / CamHeight));
	
	Vec3 ObjPos = Vec3(m_pCurObj->Transform()->GetRelativePos().x *(ImageSize[0]/CamWidth), m_pCurObj->Transform()->GetRelativePos().y * (ImageSize[1] / CamHeight), m_pCurObj->Transform()->GetRelativePos().z);
	
	// ����Ÿ�ٿ��� Ÿ�ϸ���ġ�� Grid
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
	

	// ���콺 ��Ŭ���� �ߴµ� Ÿ���� �־����� ����� Ÿ���� ����.
	if (ImGui::IsMouseDown(1))
	{
		// ���콺 ��ġ���� Ÿ�ϸ� LeftTop�� ���� ��ũ��� ���� idx���
		int IdxX = (int)((x - TileLeft) / OneBlockX);
		int IdxY = (int)((y - TileTop) / OneBlockY);

		// idx�� Ÿ�ϸ� �ȿ� �ִ� ���̿��߸� ����
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
		

	// �ε��� ���ϴ� ���� RTTex���� ������.

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


	// �� ���� ũ��
	float OneBlockX = (TileRight - TileLeft) / TileCountX;
	float OneBlockY = (TileBot - TileTop) / TileCountY;

	if (ImGui::IsMouseClicked(2))
	{
		// ���콺 ��ġ���� Ÿ�ϸ� LeftTop�� ���� ��ũ��� ���� idx���
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


