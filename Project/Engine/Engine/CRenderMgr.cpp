#include "pch.h"
#include "CRenderMgr.h"

#include "CDevice.h"
#include "CConstBuffer.h"

#include "CCamera.h"
#include "CResMgr.h"
#include "CLight2D.h"
#include "CTransform.h"

#include "CStructuredBuffer.h"
#include "CSceneMgr.h"
#include "CScene.h"

CRenderMgr::CRenderMgr()
	: m_pEditorCam(nullptr)
	, m_pLight2DBuffer(nullptr)

{
	m_pLight2DBuffer = new CStructuredBuffer;
	m_pLight2DBuffer->Create(sizeof(tLightInfo), 2, SB_TYPE::READ_ONLY, true, nullptr);
}

CRenderMgr::~CRenderMgr()
{
	SAFE_DELETE(m_pLight2DBuffer);
}

void CRenderMgr::update()
{

}

void CRenderMgr::render()
{
	// Rendering ����
	CDevice::GetInst()->SetRenderTarget();
	CDevice::GetInst()->ClearTarget();
	
	// Light ������Ʈ
	UpdateLight2D();

	// Global ��� ������Ʈ
	static CConstBuffer* pGlobalCB = CDevice::GetInst()->GetCB(CB_TYPE::GLOBAL);
	pGlobalCB->SetData(&g_global, sizeof(tGlobal));
	pGlobalCB->UpdateData();
	pGlobalCB->UpdateData_CS();

	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	if (pCurScene->GetSceneState() == SCENE_STATE::PLAY)
	{
		render_play();
	}

	else
	{
		render_editor();
	}	
}

void CRenderMgr::render_play()
{
	if (m_vecCam.empty())
		return;

	// ���� ī�޶� �������� ������
	CCamera* pMainCam = m_vecCam[0];

	m_MainCamPos = pMainCam->Transform()->GetRelativePos();
	m_MainCamWidth =pMainCam->GetWidth();
	m_MainCamAspect = pMainCam->GetAspectRatio();

	// Camera �� ��� Layer �� ������Ʈ���� Shader Domain �� ���� �з��ص�
	pMainCam->SortGameObject();

	g_transform.matView = pMainCam->GetViewMat();
	g_transform.matProj = pMainCam->GetProjMat();

	// Foward ��ü ������
	pMainCam->render_forward();

	// Masked ��ü ������
	pMainCam->render_masked();

	// Alpha ��ü ������
	pMainCam->render_translucent();

	// PostProcess ��ü ������
	pMainCam->render_postprocess();


	// Sub ī�޶� �������� ������
	for (int i = 1; i < m_vecCam.size(); ++i)
	{
		if (nullptr == m_vecCam[i])
			continue;

		m_vecCam[i]->SortGameObject();

		g_transform.matView = m_vecCam[i]->GetViewMat();
		g_transform.matProj = m_vecCam[i]->GetProjMat();

		// Foward ��ü ������
		m_vecCam[i]->render_forward();

		// Masked ��ü ������
		m_vecCam[i]->render_masked();

		// Alpha ��ü ������
		m_vecCam[i]->render_translucent();
	}
}

void CRenderMgr::render_editor()
{
	if (nullptr == m_pEditorCam)
		return;

	// ������ ī�޶� �������� ������
	// Camera �� ��� Layer �� ������Ʈ���� Shader Domain �� ���� �з��ص�
	m_pEditorCam->SortGameObject();

	g_transform.matView = m_pEditorCam->GetViewMat();
	g_transform.matProj = m_pEditorCam->GetProjMat();

	// Foward ��ü ������
	m_pEditorCam->render_forward();

	// Masked ��ü ������
	m_pEditorCam->render_masked();

	// Alpha ��ü ������
	m_pEditorCam->render_translucent();

	// PostProcess ��ü ������
	m_pEditorCam->render_postprocess();	

	CopyRenderTarget();
}

void CRenderMgr::RegisterCamera(CCamera* _pCam)
{
	// ī�޶� RenderMgr�� ���� ��� �Ǵ� ���
	if (-1 == _pCam->m_iCamIdx)
	{
		m_vecCam.push_back(_pCam);
		int iIdx = (int)m_vecCam.size() - 1;
		_pCam->m_iCamIdx = iIdx;
	}	
	else
	{
		if (m_vecCam.size() <= _pCam->m_iCamIdx)
		{
			m_vecCam.resize((size_t)_pCam->m_iCamIdx + 1);
		}

		m_vecCam[_pCam->m_iCamIdx] = _pCam;
	}
}

void CRenderMgr::SwapCameraIndex(CCamera* _pCam, int _iChangeIdx)
{
	for (size_t i = 0; i < m_vecCam.size(); ++i)
	{
		if (_pCam == m_vecCam[i])
		{
			if (nullptr != m_vecCam[_iChangeIdx])
			{
				m_vecCam[_iChangeIdx]->m_iCamIdx = (int)i;
				_pCam->m_iCamIdx = _iChangeIdx;

				return;
			}
		}
	}

	assert(nullptr);
}

void CRenderMgr::CopyTargetToPostProcess()
{
	Ptr<CTexture> pRenderTarget = CResMgr::GetInst()->FindRes<CTexture>(L"RenderTargetTex");
	Ptr<CTexture> pPostProcess = CResMgr::GetInst()->FindRes<CTexture>(L"PostProcessTex");

	CONTEXT->CopyResource(pPostProcess->GetTex2D().Get(), pRenderTarget->GetTex2D().Get());
}

void CRenderMgr::CopyRenderTarget()
{
	Ptr<CTexture> pRenderTarget = CResMgr::GetInst()->FindRes<CTexture>(L"RenderTargetTex");
	Ptr<CTexture> pRTUI = CResMgr::GetInst()->FindRes<CTexture>(L"RenderTarget");

	CONTEXT->CopyResource(pRTUI->GetTex2D().Get(), pRenderTarget->GetTex2D().Get());
}

void CRenderMgr::UpdateLight2D()
{
	if (m_pLight2DBuffer->GetElementCount() < m_vecLight2D.size())
	{
		m_pLight2DBuffer->Create(sizeof(tLightInfo), m_vecLight2D.size(), SB_TYPE::READ_ONLY, true, nullptr);
	}

	static vector<tLightInfo> vecLight2DInfo;
	vecLight2DInfo.clear();

	for (size_t i = 0; i < m_vecLight2D.size(); ++i)
	{
		vecLight2DInfo.push_back(m_vecLight2D[i]->GetLightInfo());
	}
	m_pLight2DBuffer->SetData(vecLight2DInfo.data(), (UINT)vecLight2DInfo.size());
	m_pLight2DBuffer->UpdateData(PIPELINE_STAGE::PS, 60);

	g_global.Light2DCount = (int)m_vecLight2D.size();

	m_vecLight2D.clear();
}
