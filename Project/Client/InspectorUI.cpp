#include "pch.h"
#include "InspectorUI.h"

#include <Engine/CKeyMgr.h>
#include <Engine/CScript.h>

#include "ScriptUI.h"
#include "TransformUI.h"
#include "MeshRenderUI.h"
#include "CameraUI.h"
#include "AnimatorUI.h"
#include "Collider2DUI.h"
#include "ParticleUI.h"
#include "Light2DUI.h"

#include "MaterialUI.h"
#include "TextureUI.h"
#include "TileMapUI.h"
#include "GraphicsShaderUI.h"
#include "PrefabUI.h"
#include "ComputeShaderUI.h"


InspectorUI::InspectorUI()
	: UI("Inspector")
	, m_pTargetObject(nullptr)
	, m_pTargetRes(nullptr)
	, m_arrComUI{}
	, m_arrResUI{}
{
	// ComponentUI ����   
	ComponentUI* pComUI = nullptr;

	pComUI = new TransformUI;
	AddChild(pComUI);
	m_arrComUI[(UINT)COMPONENT_TYPE::TRANSFORM] = pComUI;

	pComUI = new MeshRenderUI;	
	AddChild(pComUI);
	m_arrComUI[(UINT)COMPONENT_TYPE::MESHRENDER] = pComUI;

	pComUI = new CameraUI;
	AddChild(pComUI);
	m_arrComUI[(UINT)COMPONENT_TYPE::CAMERA] = pComUI;

	pComUI = new AnimatorUI;
	AddChild(pComUI);
	m_arrComUI[(UINT)COMPONENT_TYPE::ANIMATOR2D] = pComUI;

	pComUI = new Collider2DUI;
	AddChild(pComUI);
	m_arrComUI[(UINT)COMPONENT_TYPE::COLLIDER2D] = pComUI;

	pComUI = new ParticleUI;
	AddChild(pComUI);
	m_arrComUI[(UINT)COMPONENT_TYPE::PARTICLESYSTEM] = pComUI;

	pComUI = new TileMapUI;
	AddChild(pComUI);
	m_arrComUI[(UINT)COMPONENT_TYPE::TILEMAP] = pComUI;

	pComUI = new Light2DUI;
	AddChild(pComUI);
	m_arrComUI[(UINT)COMPONENT_TYPE::LIGHT2D] = pComUI;


	// ==============
	// ResInfoUI ����
	// ==============
	ResInfoUI* pResInfoUI = nullptr;

	// MaterialUI
	pResInfoUI = new MaterialUI;
	AddChild(pResInfoUI);
	m_arrResUI[(UINT)RES_TYPE::MATERIAL] = pResInfoUI;

	// TextureUI
	pResInfoUI = new TextureUI;
	AddChild(pResInfoUI);
	m_arrResUI[(UINT)RES_TYPE::TEXTURE] = pResInfoUI;

	pResInfoUI = new GraphicsShaderUI;
	AddChild(pResInfoUI);
	m_arrResUI[(UINT)RES_TYPE::GRAPHICS_SHADER] = pResInfoUI;

	pResInfoUI = new PrefabUI;
	AddChild(pResInfoUI);
	m_arrResUI[(UINT)RES_TYPE::PREFAB] = pResInfoUI;

	pResInfoUI = new ComputeShaderUI;
	AddChild(pResInfoUI);
	m_arrResUI[(UINT)RES_TYPE::COMPUTE_SHADER] = pResInfoUI;
}

InspectorUI::~InspectorUI()
{
}


void InspectorUI::update()
{

}

void InspectorUI::render_update()
{
	
}



void InspectorUI::SetTargetObject(CGameObject* _pTarget)
{
	m_pTargetObject = _pTarget;

	for (int i = 0; i < (int)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrComUI[i])
		{
			// Object �� nullptr �� ���
			if(nullptr == m_pTargetObject)
				m_arrComUI[i]->Deactivate();
			else if (m_pTargetObject->GetComponent((COMPONENT_TYPE)i))
			{
				m_arrComUI[i]->Activate();
				m_arrComUI[i]->SetTargetObject(m_pTargetObject);
			}			
			else
			{
				m_arrComUI[i]->Deactivate();
			}
		}		
	}

	// ������Ʈ�� null �̸�
	if (nullptr == m_pTargetObject)
	{
		// ��� ��ũ��ƮUI ��Ȱ��ȭ
		for (int i = 0; i < m_vecScriptUI.size(); ++i)
		{
			m_vecScriptUI[i]->Deactivate();
		}
	}
	else
	{
		const vector<CScript*>& vecScripts = m_pTargetObject->GetScripts();
		ScriptUI* pScriptUI = nullptr;

		for (size_t i = 0; i < vecScripts.size(); ++i)
		{
			if (m_vecScriptUI.size() <= i)
				pScriptUI = AddScriptUI();
			else
				pScriptUI = m_vecScriptUI[i];

			pScriptUI->SetTargetObject(m_pTargetObject);
			pScriptUI->SetTargetScript(vecScripts[i]);
			pScriptUI->Activate();
		}

		// ScriptUI �� �� ���� ������
		if (vecScripts.size() < m_vecScriptUI.size())
		{
			// �����ϴ� UI �� ������ ������ ScriptUI ���� ��Ȱ��ȭ �Ѵ�.��
			for (size_t i = vecScripts.size(); i < m_vecScriptUI.size(); ++i)
			{
				m_vecScriptUI[i]->Deactivate();
			}
		}
	}

	



	// ResInfoUI ��Ȱ��ȭ
	for (int i = 0; i < (int)RES_TYPE::END; ++i)
	{
		if (nullptr != m_arrResUI[i] && m_arrResUI[i]->IsActive())
			m_arrResUI[i]->Deactivate();
	}
}

void InspectorUI::SetTargetResource(CRes* _pTargetRes)
{
	// ComponentUI ���� ��Ȱ��ȭ
	for (int i = 0; i < (int)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrComUI[i] && m_arrComUI[i]->IsActive())
			m_arrComUI[i]->Deactivate();
	}

	// ScriptUI ���� ��Ȱ��ȭ
	for (size_t i = 0; i < m_vecScriptUI.size(); ++i)
	{
		m_vecScriptUI[i]->Deactivate();
	}

	// Ȱ��ȭ ��ų RES_TYPE �� �˾Ƴ�
	RES_TYPE type = _pTargetRes->GetResType();

	// �ش� ���ҽ� ResInfoUI Ȱ��ȭ
	for (int i = 0; i < (int)RES_TYPE::END; ++i)
	{
		if (nullptr != m_arrResUI[(int)i])
		{
			if (i == (int)type)
			{
				m_arrResUI[i]->Activate();
				m_arrResUI[i]->SetTargetRes(_pTargetRes);
			}			
			else
				m_arrResUI[i]->Deactivate();
		}
	}
	
}

ScriptUI* InspectorUI::AddScriptUI()
{
	ScriptUI* pScriptUI = new ScriptUI;
	pScriptUI->Deactivate();
	AddChild(pScriptUI);

	m_vecScriptUI.push_back(pScriptUI);

	return pScriptUI;
}
