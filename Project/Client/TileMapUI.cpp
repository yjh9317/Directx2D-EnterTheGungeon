#include "pch.h"
#include "TileMapUI.h"

#include <Engine/CTileMap.h>
#include "TileMapTex.h"
#include "CImGuiMgr.h"

#include <Engine/func.h>

TileMapUI::TileMapUI()
	:ComponentUI("TileMap", COMPONENT_TYPE::TILEMAP)
	, m_pObj(nullptr)


{
	SetSize(Vec2(0.f, 220.f));
}

TileMapUI::~TileMapUI()
{
}


void TileMapUI::update()
{
	ComponentUI::update();
}

void TileMapUI::render_update()
{
	ComponentUI::render_update();

	m_pObj = GetTargetObject();
	assert(m_pObj);

	int ColCount = m_pObj->TileMap()->GetColCount();
	int RowCount = m_pObj->TileMap()->GetRowCount();

	int TileCountX = m_pObj->TileMap()->GetTileCountX();
	int TileCountY = m_pObj->TileMap()->GetTileCountY();



	Vec2 SlicePixel = m_pObj->TileMap()->GetSlicePixel();
	Vec2 SliceUV = m_pObj->TileMap()->GetSliceUV();

	Ptr<CTexture> Tex = m_pObj->TileMap()->GetTexture();

	ImGui::PushItemWidth(150); // Float3 위젯 간격 설정

	//ImGui::Text("ColCount");
	//ImGui::SameLine(100);
	//ImGui::InputInt("##ColCount", &ColCount, 0, 0, ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);
	//if (ColCount < 0) ColCount = 0;
	//pTargetObject->TileMap()->SetColCount(ColCount);

	//ImGui::Text("RowCount");
	//ImGui::SameLine(100);
	//ImGui::InputInt("##RowCount", &RowCount, 0, 0, ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);
	//if (RowCount < 0) RowCount = 0;
	//pTargetObject->TileMap()->SetRowCount(RowCount);



	ImGui::Text("TileCountX");
	ImGui::SameLine(100);
	ImGui::InputInt("##TileCountX", &TileCountX, 0, 0, ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);


	ImGui::Text("TileCountY");
	ImGui::SameLine(100);
	ImGui::InputInt("##TileCountY", &TileCountY, 0, 0, ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);



	//float SP[2] = { SlicePixel.x,SlicePixel.y };

	//ImGui::Text("SlicePixel");
	//ImGui::SameLine(100);
	//ImGui::InputFloat2("##SlicePixel", SP,"%.f", ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

	static int TileX = 0;
	static int TileY = 0;
	ImGui::Text("TileX");
	ImGui::SameLine(100);
	ImGui::InputInt("##TileX", &TileX);

	ImGui::Text("TileY");
	ImGui::SameLine(100);
	ImGui::InputInt("##TileY", &TileY);

	if (ImGui::Button("TileCountChange"))
	{
		if (TileX > 0 && TileY > 0) {

			m_pObj->TileMap()->SetTileMapCount(TileX, TileY);

			for (int i = 0; i < TileX * TileY; ++i)
			{
				m_pObj->TileMap()->SetTileData(i, 0);
			}
			TileX = 0;
			TileY = 0;
		}
	}
	ImGui::SameLine();

	if (ImGui::Button("TileEdit"))
	{

		TileMapTex* pTileMapTex = (TileMapTex*)CImGuiMgr::GetInst()->FindUI("##TileMapTex");
		pTileMapTex->SetObj(m_pObj);
		pTileMapTex->SetTileMapUI(this);
		pTileMapTex->SetTitle("TileEdit");
		pTileMapTex->Activate();
		//RenderTargetUI* pRTUI = (RenderTargetUI*)CImGuiMgr::GetInst()->FindUI("##RenderTargetUI");
	}
	ImGui::PushItemWidth(220.f);

	ImGui::NewLine();

	ImGui::Text("Save");
	ImGui::SameLine();
	ImGui::InputText("##Save", (char*)m_SaveName.c_str(), sizeof(m_SaveName));
	ImGui::SameLine();
	if (ImGui::Button("Save"))
	{
		string str = m_SaveName.c_str();
		wstring wstr = wstring(str.begin(), str.end());
		TileMapSave(wstr);
	}

	ImGui::Text("Load");
	ImGui::SameLine();
	ImGui::InputText("##Load", (char*)m_LoadName.c_str(), sizeof(m_LoadName));
	ImGui::SameLine();
	if (ImGui::Button("Load"))
	{
		string str = m_LoadName.c_str();
		wstring wstr = wstring(str.begin(), str.end());
		TileMapLoad(wstr);
	}

}

void TileMapUI::TileMapSave(const wstring& TileName)
{
	if (TileName == L"")
		return;

	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += L"tilemap\\";
	strFilePath += TileName + L".tile";


	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFilePath.c_str(), L"wb");


	assert(pFile);
	if (nullptr == pFile)
		return;

	CTileMap* pTileMap = m_pObj->TileMap();

	//이름 
	wstring wstr = m_pObj->GetName();
	BYTE len = (BYTE)wstr.length();
	fwrite(&len, sizeof(BYTE), 1, pFile);
	fwrite(wstr.c_str(), sizeof(wchar_t), len, pFile);
	
	// 타일맵 텍스쳐 저장
	SaveResPtr(GetTargetObject()->TileMap()->GetTexture(), pFile);

	// 슬라이스 픽셀 ,UV
	Vec2 vSlicePixel = pTileMap->GetSlicePixel();
	Vec2 vSliceUV = pTileMap->GetSliceUV();

	fwrite(&vSlicePixel, sizeof(Vec2), 1, pFile);
	fwrite(&vSliceUV, sizeof(Vec2), 1, pFile);

	// 타일 행 열 개수 저장
	UINT RowCnt = pTileMap->GetRowCount();
	UINT ColCnt = pTileMap->GetColCount();
	 
	fwrite(&RowCnt, sizeof(UINT), 1, pFile);
	fwrite(&ColCnt, sizeof(UINT), 1, pFile);

	// 타일 가로 세로 개수 저장
	UINT TileX = pTileMap->GetTileCountX();
	UINT TileY = pTileMap->GetTileCountY();

	fwrite(&TileX, sizeof(UINT), 1, pFile);
	fwrite(&TileY, sizeof(UINT), 1, pFile);

	//타일 데이터와 크기 저장
	vector<tTileData> vTileData = pTileMap->GetTileData();
	
	for (int i = 0; i < TileY*TileX ; ++i)
	{
		fwrite(&vTileData[i].iImgIdx, sizeof(int), 1, pFile);
		fwrite(&vTileData[i].vLTUV, sizeof(Vec2), 1, pFile);
	}
	fclose(pFile);
}


//
//SaveResPtr(m_pAtlasTex, _pFile);
//
//fwrite(&m_vSlicePixel, sizeof(Vec2), 1, _pFile);
//fwrite(&m_vSliceUV, sizeof(Vec2), 1, _pFile);
//fwrite(&m_iRowCount, sizeof(UINT), 1, _pFile);
//fwrite(&m_iColCount, sizeof(UINT), 1, _pFile);
//fwrite(&m_iTileCountX, sizeof(UINT), 1, _pFile);
//fwrite(&m_iTileCountY, sizeof(UINT), 1, _pFile);
//fwrite(m_vecTileData.data(), sizeof(tTileData), m_vecTileData.size(), _pFile);

void TileMapUI::TileMapLoad(const wstring& TileName)
{
	if (TileName == L"")
		return;

	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += L"tilemap\\";
	strFilePath += TileName + L".tile";


	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFilePath.c_str(), L"rb");

	assert(pFile);
	if (nullptr == pFile)
		return;

	wchar_t szBuffer[256] = {};
	memset(szBuffer, 0, sizeof(wchar_t) * 256);
	//이름
	BYTE len = 0;
	fread(&len, sizeof(BYTE), 1, pFile);
	fread(szBuffer, sizeof(wchar_t), (size_t)len, pFile);
	m_pObj->SetName(szBuffer);

	// 타일맵 텍스쳐 불러오기
	Ptr<CTexture> pTex;
	LoadResPtr(pTex, pFile);
	m_pObj->TileMap()->SetAtlasTex(pTex);


	//// 슬라이스 픽셀 ,UV
	Vec2 vSlicePixel = {};
	Vec2 vSliceUV = {};

	fread(&vSlicePixel, sizeof(Vec2), 1, pFile);
	fread(&vSliceUV, sizeof(Vec2), 1, pFile);

	m_pObj->TileMap()->SetSlicePixel(vSlicePixel);
	m_pObj->TileMap()->SetSliceUV(vSliceUV);


	// 타일 행 열 개수 저장
	UINT RowCnt = 0;
	UINT ColCnt = 0;

	fread(&RowCnt, sizeof(UINT), 1, pFile);
	fread(&ColCnt, sizeof(UINT), 1, pFile);

	m_pObj->TileMap()->SetRowCount(RowCnt);
	m_pObj->TileMap()->SetColCount(ColCnt);


	// 타일 가로 세로 개수 저장
	UINT TileX = 0;
	UINT TileY = 0;

	fread(&TileX, sizeof(UINT), 1, pFile);
	fread(&TileY, sizeof(UINT), 1, pFile);

	m_pObj->TileMap()->SetTileMapCount(TileX, TileY);


	vector<tTileData> vTileData = {};
	vTileData.resize(TileX * TileY);
	for (int i = 0; i < TileY * TileX; ++i)
	{
		fread(&vTileData[i].iImgIdx, sizeof(int), 1, pFile);
		fread(&vTileData[i].vLTUV, sizeof(Vec2), 1, pFile);
	}

	
	m_pObj->TileMap()->SetVecTileData(vTileData);
	fclose(pFile);
}
