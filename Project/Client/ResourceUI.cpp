#include "pch.h"
#include "ResourceUI.h"

#include <Engine/CResMgr.h>
#include <Engine\CPrefab.h>
#include <Engine/CEventMgr.h>

#include "CImGuiMgr.h"
#include "InspectorUI.h"
#include "TreeUI.h"

#include "SceneOutliner.h"

#include <Engine/CTransform.h>
#include <Engine/CTileMap.h>
#include <Engine/CSound.h>

#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>
#include <Script/CSceneSaveLoad.h>

#include <experimental/filesystem>




ResourceUI::ResourceUI()
	: UI("Resource")
	, m_RightClicked(false)
{
	m_TreeUI = new TreeUI(true);
	m_TreeUI->SetTitle("Resource");
	m_TreeUI->UseFrame(true);
	m_TreeUI->UseDragDropOuter(true);

	AddChild(m_TreeUI);

	// Clicked Delegate 등록
	m_TreeUI->SetClickedDelegate(this, (CLICKED)&ResourceUI::ItemClicked);

	// DoubleClicked Deletage 등록
	m_TreeUI->SetDoubleClickedDelegate(this, (CLICKED)&ResourceUI::ItemDBClicked);

	Reset();


}

ResourceUI::~ResourceUI()
{
}

void ResourceUI::update()
{
	// 리소스 변경상태 저장
	if (KEY_PRESSED(KEY::LCTRL) && KEY_TAP(KEY::S))
	{
		CResMgr::GetInst()->SaveChangedRes();
	}

	UI::update();
}

void ResourceUI::render_update()
{
	if (CEventMgr::GetInst()->HasOccurObjEvent())
	{
		Renew();
	}


	ClickPopUp();
}

void ResourceUI::Reset()
{
	// Content 폴더 밑의 리소스 로딩
	Reload();

	Renew();	
}

void ResourceUI::ItemClicked(DWORD_PTR _dwNode)
{
	TreeNode* pNode = (TreeNode*)_dwNode;

	string strKey = pNode->GetName();
	CRes* pResource = (CRes*)pNode->GetData();

	// 프레임 노드가 눌렸다면 아무일도 없다.
	if (nullptr == pResource)
		return;

	// InspectorUI 를 얻어옴
	InspectorUI* pInspectorUI = (InspectorUI*)CImGuiMgr::GetInst()->FindUI("Inspector");
	pInspectorUI->SetTargetResource(pResource);
}

void ResourceUI::ItemDBClicked(DWORD_PTR _dwNode)
{
	TreeNode* pNode = (TreeNode*)_dwNode;

	string strKey = pNode->GetName();
	CRes* pResource = (CRes*)pNode->GetData();

	// 프레임 노드가 눌렸다면 아무일도 없다.
	if (nullptr == pResource || pNode->GetParent()->GetName() != "SCENEFILE")
		return;


	// Scene 로딩
	// 현재 Scene 정지
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	pCurScene->SetSceneState(SCENE_STATE::STOP);

	// 로딩할 Scene 파일의 경로 계산
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += pResource->GetRelativePath();

	CScene* pNewScene = CSceneSaveLoad::LoadScene(strFilePath);
	CSceneMgr::GetInst()->ChangeScene(pNewScene);


	SceneOutliner* pSceneOutlinerUI = (SceneOutliner*)CImGuiMgr::GetInst()->FindUI("SceneOutliner");
	pSceneOutlinerUI->Reset();


}



void ResourceUI::Reload()
{
	// Content 폴더 밑으로 모든 파일의 상대경로를 알아낸다.
	m_vecResPath.clear();
	FindFileName(CPathMgr::GetInst()->GetContentPath());

	// 키값 확인
	for (size_t i = 0; i < m_vecResPath.size(); ++i)
	{
		// 1. 파일명을 통해서 리소스의 종류를 알아낸다.
		RES_TYPE eType = GetResTypeFromExt(m_vecResPath[i]);

		// 2. 상대경로를 키값으로 해서 리소스매니저에 로딩한다.
		switch (eType)
		{
		case RES_TYPE::PREFAB:
			CResMgr::GetInst()->Load<CPrefab>(m_vecResPath[i], m_vecResPath[i]);
			break;
		case RES_TYPE::MESHDATA:

			break;
		case RES_TYPE::MATERIAL:
			CResMgr::GetInst()->Load<CMaterial>(m_vecResPath[i], m_vecResPath[i]);
			break;		
		case RES_TYPE::MESH:

			break;
		case RES_TYPE::TEXTURE:
			CResMgr::GetInst()->Load<CTexture>(m_vecResPath[i], m_vecResPath[i]);
			break;
		case RES_TYPE::SOUND:
			CResMgr::GetInst()->Load<CSound>(m_vecResPath[i], m_vecResPath[i]);
			break;
		case RES_TYPE::SCENEFILE:
			CResMgr::GetInst()->Load<CSceneFile>(m_vecResPath[i], m_vecResPath[i]);
			break;
		}
	}

	// 3. 로딩되어있는 리소스들의 실제 파일 존재 확인
	for (UINT i = 0; i < (UINT)RES_TYPE::END; ++i)
	{
		wstring strContentPath = CPathMgr::GetInst()->GetContentPath();

		const map<wstring, CRes*>& mapRes = CResMgr::GetInst()->GetResList((RES_TYPE)i);
		for (const auto& pair : mapRes)
		{
			if (pair.second->IsEngineRes())
				continue;

			// File Exist 체크
			if (!filesystem::exists(strContentPath + pair.second->GetRelativePath()))
			{
				// 4. 없으면 리소스매니저에서 메모리 해제
				MessageBox(nullptr, L"원본파일 삭제 됨", L"파일변경 감지", MB_OK);

				if (0 == pair.second->GetRefCount())
				{
					// 삭제
					tEventInfo info;
					info.eType = EVENT_TYPE::DELETE_RES;
					info.lParam = (DWORD_PTR)pair.second;
					CEventMgr::GetInst()->AddEvent(info);

					MessageBox(nullptr, L"리소스 메모리 해제", L"파일변경 감지", MB_OK);
				}

				else
				{
					MessageBox(nullptr, L"사용 중인 리소스\n 메모리 해제 실패", L"파일변경 감지", MB_OK);
				}
			}
		}
	}
}

void ResourceUI::Renew()
{
	m_TreeUI->Clear();

	// 리소스를 Tree 에 표시
	for (int i = 0; i < (int)RES_TYPE::END; ++i)
	{
		// TreeUI 가 DummyRoot 를 사용하기 때문에, 리소스 항목 노드들은 더미 자식으로 들어감
		TreeNode* pResNode = m_TreeUI->AddTreeNode(nullptr, ToString((RES_TYPE)i));

		const map<wstring, CRes*>& mapRes = CResMgr::GetInst()->GetResList((RES_TYPE)i);

		for (const auto& pair : mapRes)
		{
			// 각 리소스 노드들은 해당 리소스 항목 자식으로 들어감
			m_TreeUI->AddTreeNode(pResNode, string(pair.first.begin(), pair.first.end()), (DWORD_PTR)pair.second);
		}
	}
}

void ResourceUI::FindFileName(const wstring& _strFolderPath)
{
	wstring strContent = _strFolderPath + L"*.*";

	WIN32_FIND_DATA FindFileData = {};

	HANDLE hFind = nullptr;

	hFind = FindFirstFile(strContent.c_str(), &FindFileData);

	if (hFind == INVALID_HANDLE_VALUE)
		return;

	while (FindNextFile(hFind, &FindFileData))
	{
		if (FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
		{
			if (0 == wcscmp(FindFileData.cFileName, L".."))
				continue;

			FindFileName(_strFolderPath + FindFileData.cFileName + L"\\");
			continue;
		}

		wstring strRelativePath = _strFolderPath + FindFileData.cFileName;
		strRelativePath = CPathMgr::GetInst()->GetRelativePath(strRelativePath);

		m_vecResPath.push_back(strRelativePath);
	}

	FindClose(hFind);
}


RES_TYPE ResourceUI::GetResTypeFromExt(const wstring& _strExt)
{
	wchar_t szExt[50] = {};
	_wsplitpath_s(_strExt.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExt, 50);

	wstring strExt = szExt;

	if (L".mtrl" == strExt)
		return RES_TYPE::MATERIAL;
	else if (L".png" == strExt || L".jpeg" == strExt || L".bmp" == strExt
		|| L".jpg" == strExt || L".tga" == strExt || L".dds" == strExt)
		return RES_TYPE::TEXTURE;
	else if (L".mp3" == strExt || L".wav" == strExt || L".ogg" == strExt)
		return RES_TYPE::SOUND;
	else if (L".pref" == strExt)
		return RES_TYPE::PREFAB;
	else if (L".mesh" == strExt)
		return RES_TYPE::MESH;
	else if (L".scene" == strExt)
		return RES_TYPE::SCENEFILE;

	return RES_TYPE::END;
}



void ResourceUI::ClickPopUp()
{
	ImVec2 MousePos = ImGui::GetMousePos();
	ImVec2 WindowPos = ImGui::GetWindowPos();
	ImVec2 WindowSize = ImGui::GetWindowSize();

	TreeNode* pNode = nullptr;

	if (ImGui::IsMouseClicked(1) && !m_RightClicked)
	{

		if (nullptr != m_TreeUI->GetSelectedNode()) {
			pNode = m_TreeUI->GetSelectedNode()->GetParent();


			if ((MousePos.x > WindowPos.x && MousePos.y > WindowPos.y &&
				MousePos.x < WindowPos.x + WindowSize.x && MousePos.y < WindowPos.y + WindowSize.y) && pNode->GetName() == "PREFAB")
			{
				m_RightClicked = true;
			}
		}
	}


	if (m_RightClicked)
	{
		if (!(MousePos.x > WindowPos.x && MousePos.y > WindowPos.y &&
			MousePos.x < WindowPos.x + WindowSize.x && MousePos.y < WindowPos.y + WindowSize.y) && ImGui::IsMouseClicked(0))
		{
			m_RightClicked = false;
		}

		if (ImGui::IsMouseClicked(2))
			m_RightClicked = false;



		ImGui::OpenPopup("ResourceUIPopup");

		if (ImGui::BeginPopup("ResourceUIPopup"))
		{
			ImVec2 vSize = ImGui::GetWindowSize();
			ImVec2 vPos = ImGui::GetWindowPos();

			ImGui::PushItemWidth(30);
			static int Obj_Layer = 0;
			ImGui::Text("Layer");
			ImGui::SameLine();
			ImGui::InputInt("##Layer", &Obj_Layer, 0);

			ImGui::SameLine();

			if (Obj_Layer < 0) Obj_Layer = 0;
			if (Obj_Layer > 31) Obj_Layer = 31;


			if (ImGui::Button("Add to Scene", ImVec2(90.f, 20.f)))
			{
				CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
				CPrefab* pPrefab = (CPrefab*)m_TreeUI->GetSelectedNode()->GetData();
				CGameObject* pObj = pPrefab->Instantiate();
				pObj->SetName(pPrefab->GetProto()->GetName());
				if (pObj->TileMap())
				{
					Ptr<CTexture> pTileAtlas = CResMgr::GetInst()->Load<CTexture>(L"texture\\Map\\map1.png", L"texture\\Map\\map1.png");
					pObj->TileMap()->SetAtlasTex(pTileAtlas);
				}

				tEventInfo evninfo;
				evninfo.eType = EVENT_TYPE::CREATE_OBJ;
				evninfo.lParam = (DWORD_PTR)pObj;
				evninfo.wParam = Obj_Layer;

				CEventMgr::GetInst()->AddEvent(evninfo);

				m_RightClicked = false;
			}

			//if (ImGui::Button("Delete", ImVec2(50.f, 20.f)))
			//{

			//}
			ImGui::EndPopup();
		}

	}
}
