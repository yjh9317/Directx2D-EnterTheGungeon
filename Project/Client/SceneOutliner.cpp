#include "pch.h"
#include "SceneOutliner.h"

#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/CTransform.h>
#include <Engine/CEventMgr.h>

#include "TreeUI.h"
#include "CImGuiMgr.h"
#include "InspectorUI.h"
#include "ResourceUI.h"



SceneOutliner::SceneOutliner()
	: UI("SceneOutliner")
{
	m_TreeUI = new TreeUI(true);
	m_TreeUI->SetTitle("SceneOutliner");
	m_TreeUI->UseFrame(false);
	m_TreeUI->ShowDummyRoot(false);
	m_TreeUI->UseDragDropOuter(true);
	m_TreeUI->UseDragDropSelf(true);


	AddChild(m_TreeUI);

	// Clicked Delegate 등록
	m_TreeUI->SetClickedDelegate(this, (CLICKED)&SceneOutliner::ObjectClicked);

	// Drag and Drop Delegate 등록
	m_TreeUI->SetDragAndDropDelegate(this, (DRAG_DROP)&SceneOutliner::DragAndDropDelegate);

	// Key Delegate 등록
	m_TreeUI->SetKeyBinding(KEY::DEL, this, (CLICKED)&SceneOutliner::PressDelete);
	

	Reset();
}

SceneOutliner::~SceneOutliner()
{

}

void SceneOutliner::update()
{
	if (CEventMgr::GetInst()->HasOccurObjEvent())
	{
		Reset();
	}

	UI::update();
}

void SceneOutliner::render_update()
{
	AddPrefab();

	ImGui::SameLine();

	CloneButton();

	ImGui::SameLine();

	DeleteButton();
	
	ImGui::Separator();
}

void SceneOutliner::Reset()
{
	m_TreeUI->Clear();

	// 현재 Scene 을 가져온다.
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		CLayer* pLayer = pCurScene->GetLayer(i);

		vector<CGameObject*>& vecRoots = pLayer->GetRootObjects();

		for (size_t i = 0; i < vecRoots.size(); ++i)
		{
			AddGameObjectToTree(vecRoots[i], m_TreeUI->GetDummyNode());
		}
	}

	// InspectorUI 를 얻어옴
	InspectorUI* pInspectorUI = (InspectorUI*)CImGuiMgr::GetInst()->FindUI("Inspector");
	pInspectorUI->SetTargetObject(nullptr);
}

void SceneOutliner::ObjectClicked(DWORD_PTR _dw)
{
	TreeNode* pNode = (TreeNode*)_dw;

	string strKey = pNode->GetName();
	CGameObject* pObject = (CGameObject*)pNode->GetData();
		
	assert(pObject);		

	// InspectorUI 를 얻어옴
	InspectorUI* pInspectorUI = (InspectorUI*)CImGuiMgr::GetInst()->FindUI("Inspector");
	pInspectorUI->SetTargetObject(pObject);
}

void SceneOutliner::AddGameObjectToTree(CGameObject* _pObject, TreeNode* _pDestNode)
{
	TreeNode* pNode = m_TreeUI->AddTreeNode(_pDestNode
											, string(_pObject->GetName().begin(), _pObject->GetName().end())
											, (DWORD_PTR)_pObject);

	const vector<CGameObject*>& vecChild = _pObject->GetChild();

	for (size_t i = 0; i < vecChild.size(); ++i)
	{
		AddGameObjectToTree(vecChild[i], pNode);
	}
}

void SceneOutliner::PressDelete(DWORD_PTR _dw)
{
	TreeNode* pNode = (TreeNode*)_dw;

	if (nullptr == pNode)
		return;

	CGameObject* pTargetObj = (CGameObject*)pNode->GetData();
	pTargetObj->Destroy();

	// InspectorUI 를 찾아서 Object 를 nullptr 로 세팅한다.
		
	InspectorUI* pInspectorUI = (InspectorUI*)CImGuiMgr::GetInst()->FindUI("Inspector");
	pInspectorUI->SetTargetObject(nullptr);
}

void SceneOutliner::DragAndDropDelegate(DWORD_PTR _dwDrag, DWORD_PTR _dwDrop)
{
	CGameObject* pChildObject = (CGameObject*)_dwDrag;
	CGameObject* pDropTargetObject = (CGameObject*)_dwDrop;


	// 드롭 목적지가 제대로 들어 온 경우
	if (nullptr != pDropTargetObject)
	{
		if (pChildObject == pDropTargetObject
			|| pDropTargetObject->IsAncestor(pChildObject))
		{
			return;
		}

		CSceneMgr::GetInst()->AddChild(pDropTargetObject, pChildObject);
	}

	// 자식 오브젝트의 목적지가 nullptr 인 경우
	else
	{
		// 이미 최상위 부모 오브젝트는 이벤트 처리가 필요없다.
		if (nullptr == pChildObject->GetParent())
		{
			return;
		}

		CSceneMgr::GetInst()->DisconnectParent(pChildObject);		
	}
}

void SceneOutliner::AddPrefab()
{
	if (ImGui::Button("Add to Prefab"))
	{

		TreeNode* pNode = m_TreeUI->GetSelectedNode();


		if (nullptr != pNode)
		{
			// 리소스 UI
			ResourceUI* pResourceUI = (ResourceUI*)CImGuiMgr::GetInst()->FindUI("Resource");

			// 리소스 UI에있는 Prefab이 있는 pChild중에 있는 PrefabNode
			TreeNode* pParentNode = pResourceUI->GetTreeUI()->GetDummyNode();
			vector<TreeNode*> pChild = pParentNode->GetChild();
			vector<TreeNode*> pPrefabVector = pChild[0]->GetChild();

			string str = "prefab\\" + pNode->GetName() + ".pref";

			for (size_t i = 0; i < pPrefabVector.size(); ++i)
			{
				if (str == pPrefabVector[i]->GetName())
					return;
			}

			// 선택노드의 오브젝트 가져오기
			CGameObject* pObj = (CGameObject*)pNode->GetData();

			// 프리팹 생성하고 선택노드의 클론을 Proto로 설정
			CPrefab* pPrefab = new CPrefab;
			CGameObject* pClone = pObj->Clone();

			
			pPrefab->SetProto(pClone);


			wstring wstr = wstring(str.begin(), str.end());


			pResourceUI->GetTreeUI()->AddTreeNode(pChild[0], str, (DWORD_PTR)pPrefab);
			CResMgr::GetInst()->AddRes<CPrefab>(wstr, pPrefab);
		}
	}
}

void SceneOutliner::CloneButton()
{
	static int Num = 0;

	if (ImGui::Button("Clone"))
	{
		TreeNode* pNode = m_TreeUI->GetSelectedNode();

		if (nullptr == pNode)
			return;

			CGameObject* pObj = (CGameObject*)pNode->GetData();
			CGameObject* pClone = pObj->Clone();

			wstring wstr = pClone->GetName() + L"_" + to_wstring(++Num);
			pClone->SetName(wstr);

			CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

			tEventInfo evninfo;
			evninfo.eType = EVENT_TYPE::CREATE_OBJ;
			evninfo.lParam = (DWORD_PTR)pClone;
			evninfo.wParam = pObj->GetLayerIndex();

			CEventMgr::GetInst()->AddEvent(evninfo);
		
	}
}

void SceneOutliner::DeleteButton()
{
	if (ImGui::Button("Delete"))
	{
		TreeNode* pNode = m_TreeUI->GetSelectedNode();

		if (nullptr == pNode)
			return;

		CGameObject* pObj = (CGameObject*)pNode->GetData();

		tEventInfo evninfo;
		evninfo.eType = EVENT_TYPE::DELETE_OBJ;
		evninfo.lParam = (DWORD_PTR)pObj;

		m_TreeUI->GetDummyNode()->RemoveChild(pNode->GetName());
		CEventMgr::GetInst()->AddEvent(evninfo);
	}
}

