#include "pch.h"
#include "CGameObject.h"

#include "CSceneMgr.h"
#include "CScene.h"
#include "CLayer.h"
#include "CEventMgr.h"

#include "CComponent.h"
#include "CTransform.h"
#include "CMeshRender.h"
#include "CCollider2D.h"
#include "CLight2D.h"
#include "CRenderComponent.h"


#include "CScript.h"

using std::pair;

CGameObject::CGameObject()
	: m_arrCom{}
	, m_vecScript{}
	, m_vecChild{}
	, m_pParent(nullptr)
	, m_pRenderComponent(nullptr)
	, m_iLayerIdx(-1)	
	, m_bDead(false)
	, m_bActive(true)
	, m_bDynamicShadow(false)
	, m_bFrustumCulling(false)
{
}

CGameObject::CGameObject(const CGameObject& _origin)
	: CEntity(_origin)
	, m_arrCom{}
	, m_vecScript{}
	, m_vecChild{}
	, m_pParent(nullptr)
	, m_pRenderComponent(nullptr)
	, m_iLayerIdx(-1)
	, m_bActive(true)
	, m_bDead(false)
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != _origin.m_arrCom[i])
		{
			AddComponent(_origin.m_arrCom[i]->Clone());
		}		
	}

	for (size_t i = 0; i < _origin.m_vecScript.size(); ++i)
	{
		AddComponent(_origin.m_vecScript[i]->Clone());
	}

	for (size_t i = 0; i < _origin.m_vecChild.size(); ++i)
	{
		AddChild(_origin.m_vecChild[i]->Clone());
	}
}

CGameObject::~CGameObject()
{
	Safe_Del_Arr(m_arrCom);
	Safe_Del_Vec(m_vecScript);
	Safe_Del_Vec(m_vecChild);
}

void CGameObject::start()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->start();
	}

	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		m_vecScript[i]->start();
	}


	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->start();
	}
}

void CGameObject::update()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if(nullptr != m_arrCom[i] && m_arrCom[i]->IsActive())
			m_arrCom[i]->update();
	}

	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		if (m_vecScript[i]->IsActive())
			m_vecScript[i]->update();
	}

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		if(m_vecChild[i]->IsActive())
			m_vecChild[i]->update();
	}
}

void CGameObject::lateupdate()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i] && m_arrCom[i]->IsActive())
			m_arrCom[i]->lateupdate();
	}

	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		if (m_vecScript[i]->IsActive())
			m_vecScript[i]->lateupdate();
	}

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		if (m_vecChild[i]->IsActive())
			m_vecChild[i]->lateupdate();
	}
}

void CGameObject::finalupdate()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->finalupdate();
	}

	// Layer �� ���
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CLayer* pLayer = pCurScene->GetLayer(m_iLayerIdx);
	pLayer->RegisterObject(this);

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->finalupdate();
	}	
}

void CGameObject::render()
{
	if (m_pRenderComponent->IsActive())
		m_pRenderComponent->render();

	if (nullptr != Collider2D())
		Collider2D()->render();
	
}

CScript* CGameObject::GetScript(UINT _iIdx)
{
	return m_vecScript[_iIdx];
}

CScript* CGameObject::GetScriptByName(const wstring& _strName)
{
	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		if (m_vecScript[i]->GetName() == _strName)
			return m_vecScript[i];
	}

	return nullptr;
}


void CGameObject::active()
{
	for (UINT i = 1; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->active();
	}

	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		m_vecScript[i]->active();
	}


	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->active();
	}
}

void CGameObject::deactive()
{
	for (UINT i = 1; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->deactive();
	}

	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		m_vecScript[i]->deactive();
	}

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->deactive();
	}
}

void CGameObject::Deregister()
{
	if (-1 == m_iLayerIdx)
	{
		return;
	}

	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CLayer* pCurLayer = pCurScene->GetLayer(m_iLayerIdx);
	pCurLayer->DeregisterObject(this);	
}

void CGameObject::DisconnectBetweenParent()
{
	assert(m_pParent);

	vector<CGameObject*>::iterator iter = m_pParent->m_vecChild.begin();
	for (; iter != m_pParent->m_vecChild.end(); ++iter)
	{
		if ((*iter) == this)
		{
			m_pParent->m_vecChild.erase(iter);
			m_pParent = nullptr;
			return;
		}		
	}
}

void CGameObject::Activate()
{
	tEventInfo info = {};

	info.eType = EVENT_TYPE::ACTIVATE_OBJECT;
	info.lParam = (DWORD_PTR)this;

	CEventMgr::GetInst()->AddEvent(info);
}

void CGameObject::Deactivate()
{
	tEventInfo info = {};

	info.eType = EVENT_TYPE::DEACTIVATE_OBJECT;
	info.lParam = (DWORD_PTR)this;

	CEventMgr::GetInst()->AddEvent(info);
}

bool CGameObject::IsAncestor(CGameObject* _pObj)
{
	CGameObject* pObj = m_pParent;

	while (pObj)
	{
		if (pObj == _pObj)
			return true;

		pObj = pObj->m_pParent;
	}

	return false;
}

void CGameObject::AddChild(CGameObject* _pChild)
{
	int iLayerIdx = _pChild->m_iLayerIdx;

	// �ڽ����� ������ ������Ʈ�� ��Ʈ ������Ʈ�̰�, Ư�� ���̾� �Ҽ��̶��
	if (nullptr == _pChild->GetParent() && -1 != iLayerIdx)
	{
		// ���̾�� ��Ʈ ������Ʈ�μ� ��� ����
		_pChild->Deregister();
		_pChild->m_iLayerIdx = iLayerIdx;
	}

	// �ٸ� �θ������Ʈ�� �̹� �ִٸ�
	if (_pChild->GetParent())
	{
		_pChild->DisconnectBetweenParent();
	}
		

	m_vecChild.push_back(_pChild);
	_pChild->m_pParent = this;
}

void CGameObject::AddComponent(CComponent* _component)
{
	COMPONENT_TYPE eType = _component->GetType();

	if (COMPONENT_TYPE::SCRIPT != eType)
	{
		assert(nullptr == m_arrCom[(UINT)eType]);

		m_arrCom[(UINT)eType] = _component;
		_component->m_pOwner = this;

		switch (_component->GetType())
		{
		case COMPONENT_TYPE::MESHRENDER:
		case COMPONENT_TYPE::TILEMAP:
		case COMPONENT_TYPE::PARTICLESYSTEM:
		case COMPONENT_TYPE::LANDSCAPE:
		case COMPONENT_TYPE::DECAL:
		{
			// �ϳ��� ������Ʈ�� Render ����� ���� ������Ʈ�� 2���̻� ���� �� ����.
			assert(!m_pRenderComponent);
			m_pRenderComponent = (CRenderComponent*)_component;
		}
		break;
		}
	}
	else
	{
		m_vecScript.push_back((CScript*)_component);
		_component->m_pOwner = this;
	}
}

void CGameObject::Destroy()
{	
	if (m_bDead)
		return;

	tEventInfo info = {};

	info.eType = EVENT_TYPE::DELETE_OBJ;
	info.lParam = (DWORD_PTR)this;

	CEventMgr::GetInst()->AddEvent(info);
}


#include "CCamera.h"
#include "CCollider2D.h"
//#include "CCollider3D.h"
#include "CAnimator2D.h"
//#include "CAnimator3D.h"
#include "CParticleSystem.h"
#include "CTileMap.h"

void CGameObject::SaveToScene(FILE* _pFile)
{
	CEntity::SaveToScene(_pFile);		
	fwrite(&m_bActive, sizeof(BYTE), 3, _pFile);	

	// Component ����
	for (int i = 0; i < (int)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
		{
			SaveWStringToFile(ToWString((COMPONENT_TYPE)i), _pFile);
			m_arrCom[i]->SaveToScene(_pFile);
			
		}		
	}
	SaveWStringToFile(L"END", _pFile);
}

void CGameObject::LoadFromScene(FILE* _pFile)
{
	CEntity::LoadFromScene(_pFile);	
	fread(&m_bActive, sizeof(BYTE), 3, _pFile);

	// Component �ҷ�����
	wstring strComponentName;

	while (true)
	{
		LoadWStringFromFile(strComponentName, _pFile);		
		if (strComponentName == L"END")
			break;
		
		if (strComponentName == ToWString(COMPONENT_TYPE::TRANSFORM))
		{
			AddComponent(new CTransform);
			Transform()->LoadFromScene(_pFile);
		}		
		else if (strComponentName == ToWString(COMPONENT_TYPE::CAMERA))
		{
			AddComponent(new CCamera);
			Camera()->LoadFromScene(_pFile);
		}
		else if (strComponentName == ToWString(COMPONENT_TYPE::COLLIDER2D))
		{
			AddComponent(new CCollider2D);
			Collider2D()->LoadFromScene(_pFile);
		}
		else if (strComponentName == ToWString(COMPONENT_TYPE::COLLIDER3D))
		{
			
		}
		else if (strComponentName == ToWString(COMPONENT_TYPE::ANIMATOR2D))
		{
			AddComponent(new CAnimator2D);
			Animator2D()->LoadFromScene(_pFile);
		}
		else if (strComponentName == ToWString(COMPONENT_TYPE::ANIMATOR3D))
		{

		}
		else if (strComponentName == ToWString(COMPONENT_TYPE::LIGHT2D))
		{
			AddComponent(new CLight2D);
			Light2D()->LoadFromScene(_pFile);
		}
		else if (strComponentName == ToWString(COMPONENT_TYPE::LIGHT3D))
		{

		}


		else if (strComponentName == ToWString(COMPONENT_TYPE::BOUNDINGBOX))
		{

		}
		else if (strComponentName == ToWString(COMPONENT_TYPE::MESHRENDER))
		{
			AddComponent(new CMeshRender);
			MeshRender()->LoadFromScene(_pFile);
		}
		else if (strComponentName == ToWString(COMPONENT_TYPE::PARTICLESYSTEM))
		{
			AddComponent(new CParticleSystem);
			ParticleSystem()->LoadFromScene(_pFile);
		}
		else if (strComponentName == ToWString(COMPONENT_TYPE::TILEMAP))
		{
			AddComponent(new CTileMap);
			TileMap()->LoadFromScene(_pFile);
		}
		else if (strComponentName == ToWString(COMPONENT_TYPE::LANDSCAPE))
		{

		}
		else if (strComponentName == ToWString(COMPONENT_TYPE::DECAL))
		{

		}
	}
}