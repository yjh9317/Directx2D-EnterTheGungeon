#include "pch.h"
#include "CScene.h"

#include "CTransform.h"
#include "CLayer.h"
#include "CGameObject.h"
#include "CResMgr.h"
#include "CSceneFile.h"



CScene::CScene()
	: m_arrLayer{}
	, m_eSceneState(SCENE_STATE::STOP)
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i] = new CLayer;
		m_arrLayer[i]->m_iLayerIdx = i;
	}
}

CScene::~CScene()
{
	Safe_Del_Arr(m_arrLayer);
}


void CScene::start()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i]->start();
	}
}

void CScene::update()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{		
		m_arrLayer[i]->update();
	}
}

void CScene::lateupdate()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i]->lateupdate();
	}
}

void CScene::finalupdate()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i]->finalupdate();
	}
}


void CScene::SetLayerName(int _iLayerIdx, const wstring& _strName)
{
	assert(0 <= _iLayerIdx && _iLayerIdx < MAX_LAYER);
	m_arrLayer[_iLayerIdx]->SetName(_strName);
}


int CScene::GetLayerIdxFromName(const wstring& _strName)
{
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		if (m_arrLayer[i]->GetName() == _strName)
		{
			return i;
		}
	}

	return -1;
}

void CScene::AddObject(CGameObject* _pRootObj, const wstring& _strLayerName)
{
	int iLayerIdx = GetLayerIdxFromName(_strLayerName);

	assert(iLayerIdx != -1);
	assert(!_pRootObj->m_pParent);

	AddObject(_pRootObj, iLayerIdx);
}

void CScene::AddObject(CGameObject* _pRootObj, int _iLayerIdx)
{
	assert(0 <= _iLayerIdx && _iLayerIdx < MAX_LAYER);
	assert(!_pRootObj->m_pParent);

	m_arrLayer[_iLayerIdx]->AddObject(_pRootObj);

	// �ڽĿ�����Ʈ�鵵 �ش� ���̾��� �ε����� �˷��ش�.
	list<CGameObject*> queue;
	queue.push_back(_pRootObj);

	// �θ� ������Ʈ ����, �ڽĵ� ��� �ش� ���̾��� �ε����� �˷��ش�(Ư�� ���̾� �Ҽ��� �ƴѰ�쿡)
	while (!queue.empty())
	{
		CGameObject* pTargetObj = queue.front();
		queue.pop_front();

		if(-1 == pTargetObj->m_iLayerIdx)
			pTargetObj->m_iLayerIdx = _iLayerIdx;

		const vector<CGameObject*>& vecChild = pTargetObj->GetChild();
		for (size_t i = 0; i < vecChild.size(); ++i)
		{
			queue.push_back(vecChild[i]);
		}
	}
}

CLayer* CScene::GetLayer(const wstring& _strLayerName)
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		if (_strLayerName == m_arrLayer[i]->GetName())
		{
			return m_arrLayer[i];
		}
	}

	return nullptr;
}

Vec3 CScene::GetPlayerPos()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		if (L"Player" == m_arrLayer[i]->GetName())
		{
			vector<CGameObject*> vObj = m_arrLayer[i]->GetRootObjects();

			for (int j = 0; j < vObj.size(); ++j)
			{
				if (vObj[j]->GetName() == L"Player")
				{
					return vObj[j]->Transform()->GetWorldPos();
				}
			}
		}
	}
}

CGameObject* CScene::GetPlayer()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		if (L"Player" == m_arrLayer[i]->GetName())
		{
			vector<CGameObject*> vObj = m_arrLayer[i]->GetRootObjects();

			for (int j = 0; j < vObj.size(); ++j)
			{
				if (vObj[j]->GetName() == L"Player")
				{
					return vObj[j];
				}
			}
		}
	}
	return nullptr;
}

Ptr<CSceneFile> CScene::GetSceneFile()
{
	return CResMgr::GetInst()->FindRes<CSceneFile>(m_strResKey);
}

void CScene::SetSceneState(SCENE_STATE _eState)
{
	if (m_eSceneState == _eState)
		return;

	if (SCENE_STATE::STOP == m_eSceneState)
	{
		if (SCENE_STATE::PLAY == _eState)
		{
			m_eSceneState = _eState;
			start();
		}
	}

	else if (SCENE_STATE::PLAY == m_eSceneState)
	{
		if (SCENE_STATE::PAUSE == _eState)
		{
			m_eSceneState = _eState;			
		}
		else if (SCENE_STATE::STOP == _eState)
		{
			m_eSceneState = _eState;
			assert(CResMgr::GetInst()->FindRes<CSceneFile>(m_strResKey).Get());
		}
	}

	else if (SCENE_STATE::PAUSE == m_eSceneState)
	{
		if (SCENE_STATE::PLAY == _eState)
		{
			m_eSceneState = _eState;
			start();
		}
	}
}
