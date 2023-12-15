#include "pch.h"
#include "AnimationUI.h"

#include <Engine/CTimeMgr.h>
#include <Engine/CGameObject.h>
#include <Engine/CAnimator2D.h>
#include <Engine/CAnimation2D.h>

#include <Engine/CKeyMgr.h>
#include <Engine/CTransform.h>
#include <Engine/CResMgr.h>

#define _USE_MATH_DEFINES
#include <math.h>

AnimationUI::AnimationUI()
	: UI("##AnimationUI")
	, m_pCurObj(nullptr)
	, Anim_Name{}
	, m_vBackSize(0.f, 0.f)
	, m_idx(0)
	, m_bCheck(true)
	, m_fAccTime(0.f)
	, m_MaxIdx(0)
	, m_Check(false)
	, m_RectPos{}
	, m_UpLine(100.f)
	, m_LeftLine(100.f)
	, m_CurTex(nullptr)
	, m_ReviseIdx(0)
{
	Frame.fDuration = 0.1f;
}


AnimationUI::~AnimationUI()
{
}


void AnimationUI::update()
{

}

void AnimationUI::render_update()
{
	if (nullptr == m_pCurObj)
		return;

	// 텍스쳐 콤보 함수
	SelectTexture();


	if (nullptr == m_CurTex)
	{
		//만약 타겟 오브젝트의 텍스처가 없다면 첫번째 리소스 텍스쳐를 준다. ex) 빈 게임오브젝트
		if (nullptr == m_pCurObj->Animator2D()->GetCurAnim())
		{
			CTexture* pTex = (CTexture*)CResMgr::GetInst()->GetResList(RES_TYPE::TEXTURE).begin()->second;
			m_CurTex = pTex;
		}
		else// 있다면 현재 타겟 오브젝트의 텍스쳐를 사용 ex) 애니메이션이 있는 게임오브젝트
			m_CurTex = m_pCurObj->Animator2D()->GetCurAnim()->GetPtrTex();
	}

	ImGui::SetWindowFontScale(1.3f);



	// 텍스쳐쪽 함수
	Canvas();

	ImGui::PushItemWidth(140);


	// 추출한 프레임
	CurFrame();

	// 추출한 프레임을 모아놓은 벡터
	FrameVecInfo();



	//애니메이션 리스트에서 불러오기
	ImGui::SetCursorPos(ImVec2(1100.f, 35.f));

	ImGui::Text("AnimName"); ImGui::SameLine();
	static int LoadIdx = 0;
	static int LoadIdx2 = 0;
	// 애니메이션 리스트를 벡터로 저장해서 ImGui Combo와 연동하기 쉽게 만듬.

	map<wstring, CAnimation2D*> AnimList = m_pCurObj->Animator2D()->GetAnimList();
	vector<pair<string, CAnimation2D*>> v;

	for (const auto& pair : AnimList)
	{
		string str = string(pair.first.begin(), pair.first.end());
		v.push_back(make_pair(str, pair.second));
	}

	if (0 == v.size())	// 만약 빈 오브젝트였다면 벡터가 비어있을것 이므로 변경할 수 없는 빈칸을 생성
	{
		string blank;
		ImGui::InputText("##asd", (char*)blank.c_str(), sizeof(blank), ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);
		LoadIdx = 0;
	}
	else {
		const char* combo_preview_value = v[LoadIdx].first.c_str();

		if (ImGui::BeginCombo("##PreviewCombo", combo_preview_value))
		{
			for (int n = 0; n < AnimList.size(); n++)
			{
				const bool is_selected = (LoadIdx == n);
				if (ImGui::Selectable(v[n].first.c_str(), is_selected))
				{
					LoadIdx = n;
					m_idx = 0;
				}
			}
			ImGui::EndCombo();
		}
	}

	ImGui::SameLine();
	if (ImGui::Button("Load"))
	{
		FrameVec.clear();
		m_idx = 0;
		CAnimation2D* Anim = v[LoadIdx].second;


		for (UINT i = 0; i < Anim->GetMaxFrame(); ++i)
		{
			float fWidth = Anim->GetTex()->Width();
			float fHeight = Anim->GetTex()->Height();



			tAnim2DFrame TmpFrame = {};
			TmpFrame.vLT = Anim->m_vecFrm[i].vLT * Vec2(fWidth, fHeight);
			TmpFrame.vSlice = Anim->m_vecFrm[i].vSlice * Vec2(fWidth, fHeight);
			TmpFrame.fDuration = Anim->m_vecFrm[i].fDuration;
			TmpFrame.vOffset = Anim->m_vecFrm[i].vOffset * Vec2(fWidth, fHeight);

			FrameVec.push_back(TmpFrame);
		}
		LoadIdx = 0;
		LoadIdx2 = 0;
	}


	ImGui::SetCursorPos(ImVec2(1730.f, 85.f));
	if (0 == v.size())	// 만약 빈 오브젝트였다면 벡터가 비어있을것 이므로 변경할 수 없는 빈칸을 생성
	{
		string blank;
		ImGui::InputText("##asd2", (char*)blank.c_str(), sizeof(blank), ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);
		LoadIdx2 = 0;
	}
	else {
		const char* combo_preview_value = v[LoadIdx2].first.c_str();

		if (ImGui::BeginCombo("##PreviewCombo2", combo_preview_value))
		{
			for (int n = 0; n < AnimList.size(); n++)
			{
				const bool is_selected2 = (LoadIdx2 == n);
				if (ImGui::Selectable(v[n].first.c_str(), is_selected2))
				{
					LoadIdx2 = n;
					m_idx = 0;
				}
			}
			ImGui::EndCombo();
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("Delete"))
	{
		if (0 != AnimList.size())
		{
			// find해서 찾은 애니메이션의 주소를 삭제하고 erase로 리스트에서 빼준다.
			wstring wstr = wstring(v[LoadIdx2].first.begin(), v[LoadIdx2].first.end());
			CAnimation2D* pTmpAnim = m_pCurObj->Animator2D()->GetAnimList().find(wstr)->second;
			SAFE_DELETE(pTmpAnim);
			m_pCurObj->Animator2D()->GetAnimList().erase(wstr);

			// 만약 지울 때 현재 애니메이션을 지울 수도 있으므로 갱신된 애니메이션 리스트의 첫번째를 지정
			if (0 != m_pCurObj->Animator2D()->GetAnimList().size())
				m_pCurObj->Animator2D()->SetCurAnim(m_pCurObj->Animator2D()->GetAnimList().begin()->second);
			else // 만약 사이즈가 1인 상태에서 지웠다면 다음 애니메이션이 없으므로 nullptr
				m_pCurObj->Animator2D()->SetCurAnim(nullptr);



			LoadIdx = 0;
			LoadIdx2 = 0;
		}
	}



	// 애니메이션 리스트에 저장
	ImGui::SetCursorPos(ImVec2(1600.f, 35.f));
	ImGui::Text("AnimName"); ImGui::SameLine();
	ImGui::InputText("##Name", Anim_Name, sizeof(Anim_Name));

	ImGui::SetCursorPos(ImVec2(1900.f, 35.f));
	if (ImGui::Button("Save", ImVec2(100.f, 30.f)))
	{

		LoadIdx2 = 0;
		LoadIdx = 0;
		if (FrameVec.size() != 0) {
			string stmp = Anim_Name;

			if (stmp.size() != 0)
			{
				wstring wstmp = wstring(stmp.begin(), stmp.end());


				// 가장 큰 Slice값을 백그라운드 사이즈로 설정
				for (int i = 0; i < FrameVec.size(); ++i)
				{
					if (m_vBackSize.x < FrameVec[i].vSlice.x)
						m_vBackSize.x = FrameVec[i].vSlice.x;

					if (m_vBackSize.y < FrameVec[i].vSlice.y)
						m_vBackSize.y = FrameVec[i].vSlice.y;
				}

				for (int i = 0; i < v.size(); ++i)
				{
					for (int j = 0; j < v[i].second->GetMaxFrame(); ++j)
					{

						float fWidth = v[i].second->GetTex()->Width();
						float fHeight = v[i].second->GetTex()->Height();

						tAnim2DFrame Frm = v[i].second->GetFrame(j);

						if (m_vBackSize.x < Frm.vSlice.x * fWidth)
							m_vBackSize.x = Frm.vSlice.x * fWidth;

						if (m_vBackSize.y < Frm.vSlice.y * fHeight)
							m_vBackSize.y = Frm.vSlice.y * fHeight;
					}
				}


				// 애니메이션 생성
				m_pCurObj->Animator2D()->CreateAnimByTool(wstmp, m_CurTex, m_vBackSize, FrameVec, (int)FrameVec.size());

				// 최대 인덱스 0으로 되돌리고
				m_MaxIdx = 0;

				// 생성한 애니메이션 재생
				m_pCurObj->Animator2D()->Play(wstmp, true);

				// 이름 다시 되돌리기
				for (int i = 0; i < stmp.size(); ++i) {
					Anim_Name[i] = '\0';
				}
			}
		}
	}

}


void AnimationUI::CurFrame()
{
	ImGui::SetCursorPos(ImVec2(1020.f, 70.f));

	if (ImGui::BeginChild("##CurFrame", ImVec2(700.0f, 280.0f), true, ImGuiWindowFlags_NoMove)) {

		ImGui::PushItemWidth(150.f);

		ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		ImVec4 border_col = ImVec4(0.82f, 0.82f, 0.82, 1.f);

		float fWidth = m_CurTex.Get()->Width();
		float fHeight = m_CurTex.Get()->Height();

		Vec2 vLT = Frame.vLT / Vec2(fWidth, fHeight);
		Vec2 vSlice = Frame.vSlice / Vec2(fWidth, fHeight);
		Vec2 fOffset = Frame.vOffset / Vec2(fWidth, fHeight);

		ImVec2 v = ImGui::GetCursorPos();


		ImGui::Image(m_CurTex.Get()->GetSRV().Get(), ImVec2(250.f, 250.f), vLT + fOffset, vLT + vSlice - fOffset, tint_col, border_col);


		ImGui::SetCursorPos(ImVec2(280.f, 60.f));
		ImGui::Text("LeftTop ");
		ImGui::SameLine();
		ImGui::InputFloat("##LeftTopX", &Frame.vLT.x, 1.f); ImGui::SameLine(); ImGui::InputFloat("##LeftTopY", &Frame.vLT.y, 1.f);


		ImGui::SetCursorPos(ImVec2(280.f, 90.f));
		ImGui::Text("Slice   ");
		ImGui::SameLine();
		ImGui::InputFloat("##SliceX", &Frame.vSlice.x, 1.f); ImGui::SameLine(); ImGui::InputFloat("##SliceY", &Frame.vSlice.y, 1.f);

		ImGui::SetCursorPos(ImVec2(280.f, 120.f));
		ImGui::Text("Duration");
		ImGui::SameLine();
		ImGui::InputFloat("##Duration", &(Frame.fDuration), 0.01f);

		ImGui::SetCursorPos(ImVec2(280.f, 150.f));
		ImGui::Text("Offset  ");
		ImGui::SameLine();
		ImGui::InputFloat("##OffsetX", &Frame.vOffset.x, 1.f); ImGui::SameLine(); ImGui::InputFloat("##OffsetY", &Frame.vOffset.y, 1.f);

		ImGui::SetCursorPos(ImVec2(280.f, 180.f));

		if (ImGui::Button("Add", ImVec2(100.f, 50.f)))
		{
			Frame.fDuration = 0.1f;
			FrameVec.push_back(Frame);
		}

		ImGui::SameLine();

		if (ImGui::Button("Reset", ImVec2(100.f, 50.f)))
		{
			Frame.vLT = Vec2(0.f, 0.f);
			Frame.vSlice = Vec2(0.f, 0.f);
		}

		ImGui::SetCursorPos(ImVec2(1000.f, 20.f));
		if (ImGui::Button("Delete"))
		{

		}


		ImGui::EndChild();
	}
}

void AnimationUI::FrameVecInfo()
{
	ImGui::SetCursorPos(ImVec2(1020.f, 360.f));
	ImGui::PushItemWidth(190.f);
	ImGui::SetWindowFontScale(1.5f);

	if (ImGui::BeginChild("##FrameVec", ImVec2(980.0f, 700.0f), true, ImGuiWindowFlags_NoMove)) {

		// 재생중인 애니메이션
		PlayFrame();

		// 수정할 애니메이션
		ReviseFrame();

		// 이미지 띄우기
		EnumerateImage();



		ImGui::EndChild();
	}
}


void AnimationUI::SelectTexture()
{
	static int item_current_idx = 0;

	ImGui::PushItemWidth(250);
	ImGui::Text("Texture");
	ImGui::SameLine();


	const map<wstring, CRes*>& mapRes = CResMgr::GetInst()->GetResList(RES_TYPE::TEXTURE);

	for (const auto& pair : mapRes)
	{
		string s = string(pair.first.begin(), pair.first.end());
		m_vTexture.push_back(make_pair(s, (CTexture*)pair.second));
	}


	const char* combo_preview_value = m_vTexture[item_current_idx].first.c_str();

	if (ImGui::BeginCombo("##ComboTex", combo_preview_value))
	{
		for (int n = 0; n < mapRes.size(); n++)
		{
			const bool is_selected = (item_current_idx == n);
			if (ImGui::Selectable(m_vTexture[n].first.c_str(), is_selected))
				item_current_idx = n;
		}
		ImGui::EndCombo();
	}
	ImGui::SameLine();

	if (ImGui::Button("Change"))
	{
		// FrameVec Clear
		FrameVec.clear();
		m_idx = 0;
		FrameVec.reserve(0);
		// Frame Clear
		Frame.vLT = Vec2(0.f, 0.f);
		Frame.vSlice = Vec2(0.f, 0.f);
		Frame.fDuration = 0.f;
		Frame.vOffset = Vec2(0.f, 0.f);
		m_CurTex = m_vTexture[item_current_idx].second;

		m_ReviseFrame.vLT = Vec2(0.f, 0.f);
		m_ReviseFrame.vSlice = Vec2(0.f, 0.f);
		m_ReviseFrame.fDuration = 0.f;
		m_ReviseFrame.vOffset = Vec2(0.f, 0.f);
		m_ReviseIdx = 0;

	}
}



void AnimationUI::Canvas()
{
	ImVec2 CurScreen = ImGui::GetCursorScreenPos();
	ImGui::SetCursorPos(ImVec2(8.f, 70.f));

	if (ImGui::IsMouseClicked(2))
		m_RectPos = ImGui::GetMousePos();

	if (ImGui::BeginChild("canvas", ImVec2(1000.0f, 1000.0f), true, ImGuiWindowFlags_NoMove)) {
		static ImVec2 size(970.0f, 970.0f);
		static ImVec2 offset(30.0f, 30.0f);


		float fWidth = m_CurTex.Get()->Width();
		float fHeight = m_CurTex.Get()->Height();
		ImVec2 uv_min = ImVec2(0.0f, 0.0f);
		ImVec2 uv_max = ImVec2(1.0f, 1.0f);
		ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		ImVec4 border_col = ImVec4(0.82f, 0.82f, 0.82, 1.f);


		ImGui::SameLine();
		ImGui::BeginGroup(); // Lock X position

		ImGui::InvisibleButton("##empty", size);
		if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Right))
		{
			offset.x += ImGui::GetIO().MouseDelta.x;
			offset.y += ImGui::GetIO().MouseDelta.y;
		}
		const ImVec2 p0 = ImGui::GetItemRectMin();
		const ImVec2 p1 = ImGui::GetItemRectMax();
		const ImVec2 text_pos = ImVec2(p0.x + offset.x, p0.y + offset.y);
		ImDrawList* draw_list = ImGui::GetWindowDrawList();


		draw_list->PushClipRect(p0, p1, true);
		draw_list->AddRectFilled(p0, p1, IM_COL32(90, 90, 120, 2515));
		draw_list->AddRectFilled(text_pos, ImVec2(text_pos.x + fWidth, text_pos.y + fHeight), IM_COL32(10, 70, 100, 100));
		draw_list->AddImage(m_CurTex.Get()->GetSRV().Get(), text_pos, ImVec2(text_pos.x + fWidth, text_pos.y + fHeight));



		if (KEY_TAP(KEY::_0))
		{
			m_Check = !m_Check;
		}

		ImGuiIO& io = ImGui::GetIO();


		if (ImGui::IsMouseDragging(ImGuiMouseButton_Right) && !ImGui::IsMouseDown(ImGuiMouseButton_Left))
		{
			m_UpLine += io.MouseDelta.x;;
			m_LeftLine += io.MouseDelta.y;
		}

		if (ImGui::IsMouseDragging(ImGuiMouseButton_Left) && !ImGui::IsMouseDown(ImGuiMouseButton_Right))
		{
			m_RectPos.x += io.MouseDelta.x;
			m_RectPos.y += io.MouseDelta.y;
		}

		if (m_Check)
		{
			draw_list->AddRect(ImVec2(m_RectPos.x, m_RectPos.y), ImVec2(m_RectPos.x + m_UpLine, m_RectPos.y + m_LeftLine), IM_COL32(0.f, 255.f, 120.f, 40.f), 0, 0, 8.f);
			draw_list->AddRectFilled(ImVec2(m_RectPos.x + 4.f, m_RectPos.y + 4.f), ImVec2(m_RectPos.x - 4.f + m_UpLine, m_RectPos.y - 4.f + m_LeftLine), IM_COL32(200.f, 10.f, 10.f, 40.f));
		}

		if (KEY_TAP(KEY::_1))
		{
			float _X = m_RectPos.x - text_pos.x;
			float _Y = m_RectPos.y - text_pos.y;
			Frame.vLT = Vec2(_X + 4.f, _Y + 4.f);
			Frame.vSlice = Vec2(m_UpLine - 8.f, m_LeftLine - 8.f);
			Frame.fDuration = 0.1f;

			Vec2 vLT = Frame.vLT / Vec2(fWidth, fHeight);
			Vec2 vSlice = Frame.vSlice / Vec2(fWidth, fHeight);
			Vec2 fOffset = Frame.vOffset / Vec2(fWidth, fHeight);
		}

		else if (KEY_TAP(KEY::_3))
		{
			Frame.fDuration = 0.1f;
			FrameVec.push_back(Frame);
		}


		draw_list->PopClipRect();

		ImGui::EndGroup();


		ImGui::EndChild();
	}
}

void AnimationUI::PlayFrame()
{
	ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 border_col = ImVec4(0.82f, 0.82f, 0.82, 1.f);

	float fWidth = m_CurTex.Get()->Width();
	float fHeight = m_CurTex.Get()->Height();

	if (m_bCheck && FrameVec.size() != 0)
	{
		m_fAccTime += DT;

		if (FrameVec[m_idx].fDuration < m_fAccTime)
		{
			m_fAccTime -= FrameVec[m_idx].fDuration;

			if (FrameVec.size() - 1 <= m_idx)
			{
				m_fAccTime = 0.f;
				m_idx = 0;
			}
			else
			{
				++m_idx;
			}
		}
	}

	Vec2 vFrameLT;
	Vec2 vFrameSlice;
	Vec2 fFrameOffset;

	if (FrameVec.size() != 0) {
		vFrameLT = FrameVec[m_idx].vLT / Vec2(fWidth, fHeight);
		vFrameSlice = FrameVec[m_idx].vSlice / Vec2(fWidth, fHeight);
		fFrameOffset = FrameVec[m_idx].vOffset / Vec2(fWidth, fHeight);
	}

	ImGui::Image(m_CurTex.Get()->GetSRV().Get(), ImVec2(250.f, 250.f), vFrameLT - fFrameOffset, vFrameLT + vFrameSlice - fFrameOffset, tint_col, border_col);

	ImGui::SetCursorPos(ImVec2(9.f, 267.f));

	if (ImGui::Button("Play", ImVec2(122.f, 30.f)))
		m_bCheck = true;

	ImGui::SameLine();

	if (ImGui::Button("Stop", ImVec2(122.f, 30.f)))
		m_bCheck = false;
}

void AnimationUI::ReviseFrame()
{
	ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 border_col = ImVec4(0.82f, 0.82f, 0.82, 1.f);

	float fWidth = m_CurTex.Get()->Width();
	float fHeight = m_CurTex.Get()->Height();


	Vec2 vFrameLT;
	Vec2 vFrameSlice;
	Vec2 fFrameOffset;

	vFrameLT = m_ReviseFrame.vLT / Vec2(fWidth, fHeight);
	vFrameSlice = m_ReviseFrame.vSlice / Vec2(fWidth, fHeight);
	fFrameOffset = m_ReviseFrame.vOffset / Vec2(fWidth, fHeight);


	// 수정할 애니메이션 Image
	ImGui::SetCursorPos(ImVec2(280.f, 9.f));

	ImGui::Image(m_CurTex.Get()->GetSRV().Get(), ImVec2(250.f, 250.f), vFrameLT - fFrameOffset, vFrameLT + vFrameSlice - fFrameOffset, tint_col, border_col);

	ImGui::SetCursorPos(ImVec2(280.f, 267.f));
	if (ImGui::Button("Prev", ImVec2(122.f, 30.f)))
	{
		if (m_ReviseIdx > 0)
		{
			--m_ReviseIdx;
			m_ReviseFrame = FrameVec[m_ReviseIdx];
		}
	}

	ImGui::SameLine();

	if (ImGui::Button("Next", ImVec2(122.f, 30.f)))
	{
		if (m_ReviseIdx < FrameVec.size() - 1 && FrameVec.size() != 0)
		{
			++m_ReviseIdx;
			m_ReviseFrame = FrameVec[m_ReviseIdx];
		}
	}
	ImGui::PushItemWidth(150.f);

	m_MaxIdx = (int)FrameVec.size() - 1;

	ImGui::SetCursorPos(ImVec2(540.f, 20.f));
	ImGui::Text("MaxIdx  "); ImGui::SameLine();
	ImGui::InputInt("##MaxIdx", &m_MaxIdx, 0, 0, ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

	// 수정할 애니메이션 Image의 정보
	ImGui::SetCursorPos(ImVec2(540.f, 50.f));
	ImGui::Text("CurIdx  "); ImGui::SameLine();
	ImGui::InputInt("##CurIdx", &m_ReviseIdx, 0, 0, ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);


	// Info
	ImGui::SetCursorPos(ImVec2(540.f, 80.f));
	ImGui::Text("LeftTop ");
	ImGui::SameLine();
	ImGui::InputFloat("##LeftTopX", &m_ReviseFrame.vLT.x, 1.f); ImGui::SameLine(); ImGui::InputFloat("##LeftTopY", &m_ReviseFrame.vLT.y, 1.f);

	ImGui::SetCursorPos(ImVec2(540.f, 110.f));
	ImGui::Text("Slice   ");
	ImGui::SameLine();
	ImGui::InputFloat("##SliceX", &m_ReviseFrame.vSlice.x, 1.f); ImGui::SameLine(); ImGui::InputFloat("##SliceY", &m_ReviseFrame.vSlice.y, 1.f);

	ImGui::SetCursorPos(ImVec2(540.f, 140.f));
	ImGui::Text("Duration");
	ImGui::SameLine();
	ImGui::InputFloat("##Duration", &(m_ReviseFrame.fDuration), 0.01f);

	ImGui::SetCursorPos(ImVec2(540.f, 170.f));
	ImGui::Text("Offset  ");
	ImGui::SameLine();
	ImGui::InputFloat("##OffsetX", &m_ReviseFrame.vOffset.x, 1.f); ImGui::SameLine(); ImGui::InputFloat("##OffsetY", &m_ReviseFrame.vOffset.y, 1.f);


	ImGui::SetCursorPos(ImVec2(802.f, 230.f));
	if (ImGui::Button("Delete", ImVec2(130.f, 30.f)))
	{
		if (FrameVec.size() != 0)
		{
			m_ReviseFrame = {};

			for (int i = m_ReviseIdx; i < FrameVec.size() - 1; ++i)
			{
				FrameVec[i] = FrameVec[i + 1];
			}
			FrameVec.resize(FrameVec.size() - 1);
			m_ReviseIdx = 0;
			m_idx = 0;
		}
	}


	ImGui::SetCursorPos(ImVec2(665.f, 270.f));


	if (ImGui::Button("Apply", ImVec2(130.f, 30.f)))
	{
		if (FrameVec.size() != 0)
			FrameVec[m_ReviseIdx] = m_ReviseFrame;

	}


	ImGui::SameLine();

	if (ImGui::Button("Clear", ImVec2(130.f, 30.f)))
	{
		m_ReviseFrame = {};
		m_ReviseIdx = 0;
		FrameVec.clear();
		m_idx = 0;
	}

}

void AnimationUI::EnumerateImage()
{
	ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 border_col = ImVec4(0.82f, 0.82f, 0.82, 1.f);

	float fWidth = m_CurTex.Get()->Width();
	float fHeight = m_CurTex.Get()->Height();

	Vec2 vFrameLT;
	Vec2 vFrameSlice;
	Vec2 fFrameOffset;


	// 수정할 애니메이션 Image


	ImGui::SetCursorPos(ImVec2(9.f, 310.f));

	if (ImGui::BeginChild("##EnumerateImage", ImVec2(950.0f, 800.0f), true, ImGuiWindowFlags_NoMove))
	{
		for (int i = 0; i < FrameVec.size(); ++i) {
			tAnim2DFrame  m_TmpFrame;

			m_TmpFrame.vLT = FrameVec[i].vLT;
			m_TmpFrame.vSlice = FrameVec[i].vSlice;
			m_TmpFrame.vOffset = FrameVec[i].vOffset;
			m_TmpFrame.fDuration = FrameVec[i].fDuration;

			vFrameLT = m_TmpFrame.vLT / Vec2(fWidth, fHeight);
			vFrameSlice = m_TmpFrame.vSlice / Vec2(fWidth, fHeight);
			fFrameOffset = m_TmpFrame.vOffset / Vec2(fWidth, fHeight);

			string str = "##Image" + to_string(i);
			int j = i;
			if (j >= 5) j %= 5;

			ImGui::SetCursorPos(ImVec2(10.f + (j * 180.f), 20.f + 180.f * (int)(i * 0.2f)));
			ImVec2 CursorPos = ImGui::GetCursorPos();
			ImGui::Image(m_CurTex.Get()->GetSRV().Get(), ImVec2(160.f, 160.f), vFrameLT - fFrameOffset, vFrameLT + vFrameSlice - fFrameOffset, tint_col, border_col);
			ImGui::SetCursorPos(CursorPos);

			if (ImGui::InvisibleButton(str.c_str(), ImVec2(160.f, 160.f), 0))
			{
				m_ReviseIdx = i;
				m_ReviseFrame = FrameVec[i];
			}
		}
		ImGui::EndChild();
	}


}



// https://gist.github.com/ayamflow/c06bc0c8a64f985dd431bd0ac5b557cd
//
//vec2 rotateUV(vec2 uv, float rotation, vec2 mid)
//{
//	return vec2(
//		cos(rotation) * (uv.x - mid.x) + sin(rotation) * (uv.y - mid.y) + mid.x,
//		cos(rotation) * (uv.y - mid.y) - sin(rotation) * (uv.x - mid.x) + mid.y
//	);
//}
//
//vec2 rotateUV(vec2 uv, float rotation, float mid)
//{
//	return vec2(
//		cos(rotation) * (uv.x - mid) + sin(rotation) * (uv.y - mid) + mid,
//		cos(rotation) * (uv.y - mid) - sin(rotation) * (uv.x - mid) + mid
//	);
//}

//
//float2 rotateUV(float2 uv, float degrees) {
//	const float Deg2Rad = (UNITY_PI * 2.0) / 360.0;
//	float rotationRadians = degrees * Deg2Rad;
//	float s = sin(rotationRadians);
//	float c = cos(rotationRadians);
//	float2x2 rotationMatrix = float2x2(c, -s, s, c);
//	uv -= 0.5;
//	uv = mul(rotationMatrix, uv);
//	uv += 0.5;
//	return uv;
//}

