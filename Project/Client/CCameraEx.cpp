#include "pch.h"
#include "CCameraEx.h"

#include <Engine\CTransform.h>

#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>

CCameraEx::CCameraEx()
{
}

CCameraEx::~CCameraEx()
{
}

void CCameraEx::update()
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	if (pCurScene->GetSceneState() == SCENE_STATE::STOP)
	{
		Ptr<CSound> pSound = CResMgr::GetInst()->Load<CSound>(L"sound\\ost\\Fight.mp3", L"sound\\ost\\Fight.mp3");
		pSound->Stop();
	}
}

void CCameraEx::finalupdate()
{
	// View ��� ���
	Vec3 vCamPos = Transform()->GetRelativePos();

	// View �̵����
	Matrix matViewTrans = XMMatrixTranslation(-vCamPos.x, -vCamPos.y, -vCamPos.z);

	// View ȸ�����
	// Right, Up, Front �� �����´�.
	Matrix matViewRot = XMMatrixIdentity();

	Vec3 vRight = Transform()->GetWorldRightDir();
	Vec3 vUp = Transform()->GetWorldUpDir();
	Vec3 vFront = Transform()->GetWorldFrontDir();

	matViewRot._11 = vRight.x;	matViewRot._12 = vUp.x; matViewRot._13 = vFront.x;
	matViewRot._21 = vRight.y;	matViewRot._22 = vUp.y;	matViewRot._23 = vFront.y;
	matViewRot._31 = vRight.z;	matViewRot._32 = vUp.z;	matViewRot._33 = vFront.z;

	m_matView = matViewTrans * matViewRot;

	// ������� ���
	if (PROJ_TYPE::ORTHOGRAPHIC == m_eProjType)
	{
		float fHeight = m_fWidth / m_fAspectRatio;
		m_matProj = XMMatrixOrthographicLH(m_fWidth, fHeight, 0.f, 5000.f);
	}
	else
	{
		m_matProj = XMMatrixPerspectiveFovLH(m_fFOV, m_fAspectRatio, 1.f, m_fFar);
	}

	g_transform.matView = m_matView;
	g_transform.matProj = m_matProj;
}
