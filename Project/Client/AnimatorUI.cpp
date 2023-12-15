#include "pch.h"
#include "AnimatorUI.h"

#include "CImGuiMgr.h"
#include "ListUI.h"
#include "AnimationUI.h"

#include <Engine/CCore.h>
#include <Engine/CAnimator2D.h>
#include <Engine/CAnimation2D.h>
#include <Engine/struct.h>
#include <Engine/CTexture.h>
#include <Engine/CPathMgr.h>
#include <Engine/CTimeMgr.h>

//#include "ImGui/imgui_stdlib.h"

#include <Engine/func.h>

using std::pair;

AnimatorUI::AnimatorUI()
	:ComponentUI("Animator", COMPONENT_TYPE::ANIMATOR2D)
	, m_PrevObj(nullptr)
	, m_pObj(nullptr)
	, m_PreviewIdx(0)
	, m_PreviewFrameIdx(0)
	, m_Time(0)
	, m_PreviewChange(false)
{
	SetSize(Vec2(0.f, 320.f));
}

AnimatorUI::~AnimatorUI()
{
}


void AnimatorUI::update()
{
	ComponentUI::update();

	CGameObject* pTargetObj = GetTargetObject();

	if(nullptr == m_PrevObj)
	m_PrevObj = pTargetObj;
	else if(m_PrevObj != pTargetObj)
	{
		m_PrevObj = pTargetObj;
		m_PreviewIdx = 0;
	}

	if (nullptr == pTargetObj)
		return;
	


	//if (KEY_TAP(KEY::_1))
	//{
	//	if (IsActive())
	//		Deactivate();
	//	else
	//		Activate();
	//}

}



void AnimatorUI::render_update()
{
	ComponentUI::render_update();
	m_Time += DT;

	m_pObj = GetTargetObject();
	assert(m_pObj);


	ImGui::SameLine();
	AnimList();
	ImGui::SameLine();
	CreateAnimByTool();

	

	CAnimation2D* pCurAnim = m_pObj->Animator2D()->GetCurAnim();

	//if (nullptr == pCurAnim)
	//	return;

	//int CurIdx = pCurAnim->m_iCurFrmIdx;


	//ImGui::PushItemWidth(150);
	//ImGui::Text("CurIdx");
	//ImGui::SameLine();
	//ImGui::InputInt("##Position", &CurIdx, 0);

	//pCurAnim->m_iCurFrmIdx = CurIdx;

	//if (CurIdx < 0 || CurIdx >= pCurAnim->GetMaxFrame()) {
	//	CurIdx = 0;
	//	pCurAnim->m_iCurFrmIdx = CurIdx;
	//}

	ImGui::PushItemWidth(220);

	SaveLoadButton();

	ImGui::NewLine();

	Preview();
}


void AnimatorUI::Save(const wstring& AnimName)
{
	if (AnimName == L"")
		return;

	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += L"animation\\";
	strFilePath += AnimName +L".anim";


	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFilePath.c_str(), L"wb");


	assert(pFile);
	if (nullptr == pFile)
		return;

	// 애니메이션 리스트
	map<wstring, CAnimation2D*> AnimList = m_pObj->Animator2D()->GetAnimList();

	// 애니메이션 리스트개수
	int Animcnt = AnimList.size();
	fwrite(&Animcnt,sizeof(int),1,pFile);
	

	for (auto iter = AnimList.begin(); iter != AnimList.end(); ++iter)
	{
		//이름 
		wstring wstr = iter->first;
		BYTE len = (BYTE)wstr.length();
		fwrite(&len, sizeof(BYTE), 1, pFile);
		fwrite(wstr.c_str(), sizeof(wchar_t), len, pFile);

		//텍스처
		SaveResPtr(iter->second->GetPtrTex(),pFile);

		//백그라운드 사이즈
		Vec2 vSize = iter->second->GetBackgroundSize();
		fwrite(&vSize, sizeof(Vec2), 1, pFile);

		vector<tAnim2DFrame> v= iter->second->m_vecFrm;
		
		//애니메이션 개수
		size_t AnimSize= v.size();
		fwrite(&AnimSize, sizeof(size_t), 1, pFile);

		//애니메이션 정보
		for (size_t j = 0; j < AnimSize; ++j)
		{
			fwrite(&v[j].vLT, sizeof(Vec2), 1, pFile);
			fwrite(&v[j].vSlice, sizeof(Vec2), 1, pFile);
			fwrite(&v[j].fDuration, sizeof(float), 1, pFile);
			fwrite(&v[j].vOffset, sizeof(Vec2), 1, pFile);
				
		}
				
	}
	fclose(pFile);

}

void AnimatorUI::Load(const wstring& AnimName)
{
	if (AnimName == L"")
		return;

	m_PreviewIdx = 0;
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += AnimName;
	


	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFilePath.c_str(), L"rb");

	assert(pFile);
	if (nullptr == pFile)
		return;


	map<wstring, CAnimation2D*> AnimList;
	wchar_t szBuffer[256] = {};
	vector<tAnim2DFrame> v;

	CAnimation2D* pCurAnim = nullptr;
	
	// 애니메이션 리스트 개수
	int Animcnt = 0;
	fread(&Animcnt, sizeof(int), 1, pFile);

	for (int i = 0; i < Animcnt; ++i)
	{
		CAnimation2D* pAnim = new CAnimation2D;
		memset(szBuffer, 0, sizeof(wchar_t) * 256);
		//이름
		BYTE len = 0;
		fread(&len, sizeof(BYTE), 1, pFile);
		fread(szBuffer, sizeof(wchar_t), (size_t)len, pFile);
		pAnim->SetName(szBuffer);


		//텍스처
		Ptr<CTexture> tmp;
		LoadResPtr(tmp,pFile);
		pAnim->SetTex(tmp);

		//백그라운드 사이즈
		Vec2 vSize;
		fread(&vSize, sizeof(Vec2), 1, pFile);
		pAnim->SetBackgroundSize(vSize);

		
		

		//애니메이션 개수
		size_t AnimSize=0;
		fread(&AnimSize, sizeof(size_t), 1, pFile);


		v.resize(AnimSize);

		//애니메이션 정보
		for (size_t j = 0; j < AnimSize; ++j)
		{
			Vec2 _vLT;
			fread(&_vLT, sizeof(Vec2), 1, pFile);
			v[j].vLT = _vLT;

			Vec2 _vSlice;
			fread(&_vSlice, sizeof(Vec2), 1, pFile);
			v[j].vSlice = _vSlice;

			float fDur;
			fread(&fDur, sizeof(float), 1, pFile);
			v[j].fDuration = fDur;

			Vec2 _vOffset;
			fread(&_vOffset, sizeof(Vec2), 1, pFile);
			v[j].vOffset = _vOffset;

			pAnim->SetFrame(v[j]);
		}
		AnimList.insert({szBuffer,pAnim});
		if (i == 0)
		{
			pCurAnim = pAnim;
		}
	}
	v.clear();
	
	m_pObj->Animator2D()->SetCurAnim(pCurAnim);
	Safe_Del_Map(m_pObj->Animator2D()->GetAnimList());
	m_pObj->Animator2D()->GetAnimList() = AnimList;


	fclose(pFile);
}


void AnimatorUI::SelectAnim(DWORD_PTR _dw)
{
	string s = (char*)_dw;
	wstring wstr = wstring(s.begin(), s.end());
		
	m_pObj->Animator2D()->Play(wstr,true);
}


void AnimatorUI::Preview()
{
	// Preview 텍스트
	ImGui::PushID(0);
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.63f, 0.4f, 0.8f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.63f, 0.4f, 0.8f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.63f, 0.4f, 0.8f));
	ImGui::Button("Preview");
	ImGui::PopStyleColor(3);
	ImGui::PopID();

	ImGui::SameLine();

	// Preview Combo

	map<wstring, CAnimation2D*> AnimList = m_pObj->Animator2D()->GetAnimList();
	vector<pair<string, CAnimation2D*>> _vAnimList;

	for (const auto& pair : AnimList)
	{
		string str = string(pair.first.begin(), pair.first.end());
		_vAnimList.push_back(make_pair(str,pair.second));
	}


	if (0 == _vAnimList.size())
		return;

		
	if(nullptr == _vAnimList[m_PreviewIdx].second)
		m_PreviewIdx = 0;

	const char* combo_preview_value = _vAnimList[m_PreviewIdx].first.c_str();

	if (ImGui::BeginCombo("##PreviewCombo", combo_preview_value))
	{
		for (int n = 0; n < AnimList.size(); n++)
		{
			const bool is_selected = (m_PreviewIdx == n);
			if (ImGui::Selectable(_vAnimList[n].first.c_str(), is_selected))
			{
				m_PreviewIdx = n;
				m_PreviewChange = true;
			}
		}
		ImGui::EndCombo();
	}

	if (m_PreviewChange)
	{
		m_PreviewFrameIdx = 0;
		m_PreviewChange = false;
	}

	CAnimation2D* pAnim = _vAnimList[m_PreviewIdx].second;

	if (m_PreviewFrameIdx > pAnim->GetMaxFrame() - 1)
		m_PreviewFrameIdx = 0;


	tAnim2DFrame CurFrame = pAnim->GetFrame(m_PreviewFrameIdx);

	if (m_Time > CurFrame.fDuration) {
		m_Time -= CurFrame.fDuration;
		++m_PreviewFrameIdx;
	}


	// Preview Image
	ImVec2 uv_min = ImVec2(0.0f, 0.0f);
	ImVec2 uv_max = ImVec2(1.0f, 1.0f);
	ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 border_col = ImVec4(0.82f, 0.82f, 0.82, 1.f);

	

	ImGui::Image((ImTextureID)pAnim->GetTex()->GetSRV().Get(),ImVec2(90.f,90.f),
		CurFrame.vLT, CurFrame.vLT + CurFrame.vSlice, tint_col, border_col);

	
}


void AnimatorUI::AnimList()
{
	const map<wstring, CAnimation2D*>& mapAnim = m_pObj->Animator2D()->GetAnimList();
	if (ImGui::Button("AnimationList"))
	{
		// ListUI 활성화한다.
		ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##ListUI");
		pListUI->Clear();
		pListUI->SetTitle("Animation List");

		for (const auto& pair : mapAnim)
		{
			pListUI->AddList(string(pair.first.begin(), pair.first.end()));
		}

		pListUI->Activate();
		pListUI->SetDBCEvent(this, (DBCLKED)&AnimatorUI::SelectAnim);
	}
}

void AnimatorUI::CreateAnimByTool()
{
	if (ImGui::Button("Edit"))
	{
		AnimationUI* pAnimationUI = (AnimationUI*)CImGuiMgr::GetInst()->FindUI("##AnimationUI");
		pAnimationUI->SetTitle("Edit");
		pAnimationUI->SetObj(m_pObj);

		pAnimationUI->Activate();
		//pListUI->SetDBCEvent(this, (DBCLKED)&AnimatorUI::MeshSelect);
	}

}

void AnimatorUI::SaveLoadButton()
{
	ImGui::Text("Save");
	ImGui::SameLine(50);
	ImGui::InputText("##Save", (char*)m_SaveName.c_str(), sizeof(m_SaveName));

	ImGui::SameLine();

	if (ImGui::Button("Save"))
	{
		wstring wstr;
		string s = m_SaveName.c_str();
		wstr = wstring(s.begin(), s.end());

		wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
		strFilePath += L"animation\\";
		strFilePath += wstr + L".anim";

		map<wstring, CAnimation2D*> AnimList = m_pObj->Animator2D()->GetAnimList();

		for (const auto& pair : AnimList)
		{
			if (strFilePath == pair.first)
				return;
		}

		Save(wstr);
	}


	ImGui::Text("Load");
	ImGui::SameLine(50);
	LoadCombo();



	ImGui::SameLine();
	if (ImGui::Button("Load"))
	{
		wstring wstr;
		string s = m_LoadName.c_str();
		wstr = wstring(s.begin(), s.end());

		Load(wstr);
	}
}

void AnimatorUI::FindAnimFile(const wstring& _strFolderPath)
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

				FindAnimFile(_strFolderPath + FindFileData.cFileName + L"\\");
				continue;
			}

			wstring strRelativePath = _strFolderPath + FindFileData.cFileName;
			strRelativePath = CPathMgr::GetInst()->GetRelativePath(strRelativePath);
			string strLoadName = string(strRelativePath.begin(), strRelativePath.end());

			LoadNameVector.push_back(strLoadName);
		}

		FindClose(hFind);

}

//void ResourceUI::FindFileName(const wstring& _strFolderPath)
//{
//	wstring strContent = _strFolderPath + L"*.*";
//
//	WIN32_FIND_DATA FindFileData = {};
//
//	HANDLE hFind = nullptr;
//
//	hFind = FindFirstFile(strContent.c_str(), &FindFileData);
//
//	if (hFind == INVALID_HANDLE_VALUE)
//		return;
//
//	while (FindNextFile(hFind, &FindFileData))
//	{
//		if (FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
//		{
//			if (0 == wcscmp(FindFileData.cFileName, L".."))
//				continue;
//
//			FindFileName(_strFolderPath + FindFileData.cFileName + L"\\");
//			continue;
//		}
//
//		wstring strRelativePath = _strFolderPath + FindFileData.cFileName;
//		strRelativePath = CPathMgr::GetInst()->GetRelativePath(strRelativePath);
//
//		m_vecResPath.push_back(strRelativePath);
//	}
//
//	FindClose(hFind);
//}


void AnimatorUI::LoadCombo()
{
	LoadNameVector.clear();
	LoadNameVector.resize(0);

	wstring wstr = CPathMgr::GetInst()->GetContentPath();
	wstr+= L"animation\\";
	FindAnimFile(wstr);

	if (LoadNameVector.size() == 0) {
		ImGui::InputText("##Load", (char*)m_LoadName.c_str(), sizeof(m_LoadName),ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);
		return;
	}

	
	static int LoadCurIdx = 0;
	const char* Loadcombo_preview = LoadNameVector[LoadCurIdx].c_str();  // Pass in the preview value visible before opening the combo (it could be anything)

	if (ImGui::BeginCombo("##LoadCombo", Loadcombo_preview))
	{
		for (int n = 0; n < LoadNameVector.size(); n++)
		{
			const bool Selected = (LoadCurIdx == n);
			if (ImGui::Selectable(LoadNameVector[n].c_str(), Selected))
				LoadCurIdx = n;
		}
		ImGui::EndCombo();
	}
	m_LoadName = LoadNameVector[LoadCurIdx].c_str();
}
