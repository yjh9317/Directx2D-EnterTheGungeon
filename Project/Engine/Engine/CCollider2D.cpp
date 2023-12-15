#include "pch.h"
#include "CCollider2D.h"

#include "CResMgr.h"

#include "CDevice.h"
#include "CConstBuffer.h"

#include "CTransform.h"
#include "CScript.h"

#include "CKeyMgr.h"

CCollider2D::CCollider2D()
	: CComponent(COMPONENT_TYPE::COLLIDER2D)
	, m_eColliderType(COLLIDER2D_TYPE::BOX)
	, m_vOffsetPos(Vec2(0.f, 0.f))
	, m_vOffsetScale(Vec2(1.f, 1.f))
	, m_iCollisionCount(0)
	, m_ColShow(1)
{
	// Collider2D ��翡 �´� �޽� ����
	SetCollider2DType(m_eColliderType);	

	// Collider2D ���� ���� ����	
	m_pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\Collider2DMtrl.mtrl");
}

CCollider2D::CCollider2D(const CCollider2D& _Origin)
	: CComponent(_Origin)
	, m_eColliderType(_Origin.m_eColliderType)
	, m_vOffsetPos(_Origin.m_vOffsetPos)
	, m_vOffsetScale(_Origin.m_vOffsetScale)
	, m_iCollisionCount(0)
{
	// Collider2D ��翡 �´� �޽� ����
	SetCollider2DType(m_eColliderType);

	// Collider2D ���� ���� ����	
	m_pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\Collider2DMtrl.mtrl");
}

CCollider2D::~CCollider2D()
{

}

void CCollider2D::SetCollider2DType(COLLIDER2D_TYPE _type)
{
	m_eColliderType = _type;

	if (COLLIDER2D_TYPE::BOX == m_eColliderType)
	{
		m_pMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh_LineStrip");		
	}
	else
	{
		m_pMesh = CResMgr::GetInst()->FindRes<CMesh>(L"CircleMesh_LineStrip");
	}
}

void CCollider2D::SetOffsetScale(Vec2 _vOffsetScale)
{
	m_vOffsetScale = _vOffsetScale;

	if (COLLIDER2D_TYPE::CIRCLE == m_eColliderType)
	{
		m_vOffsetScale.y = m_vOffsetScale.x;
	}
}

void CCollider2D::SetOffsetScale(float _x, float _y)
{
	m_vOffsetScale = Vec2(_x, _y);

	if (COLLIDER2D_TYPE::CIRCLE == m_eColliderType)
	{
		m_vOffsetScale.y = m_vOffsetScale.x;
	}
}

void CCollider2D::finalupdate()
{
	Matrix matTrans = XMMatrixTranslation(m_vOffsetPos.x, m_vOffsetPos.y, 0.f);
	Matrix matScale = XMMatrixScaling(m_vOffsetScale.x, m_vOffsetScale.y, 1.f);	
	m_matColWorld = matScale * matTrans;
		
	Vec3 vObjScale = Transform()->GetWorldScale();
	Matrix matObjScaleInv = XMMatrixInverse(nullptr, XMMatrixScaling(vObjScale.x, vObjScale.y, vObjScale.z));

	// �浹ü ������ * ������Ʈ ���� ũ�� �����(ũ��^-1) * ������Ʈ ���� ���(ũ�� * ȸ�� * �̵�)
	m_matColWorld = m_matColWorld * matObjScaleInv * Transform()->GetWorldMat();
}

void CCollider2D::UpdateData()
{
	g_transform.matWorld = m_matColWorld;
	g_transform.matWV = g_transform.matWorld * g_transform.matView;
	g_transform.matWVP = g_transform.matWV * g_transform.matProj;

	CConstBuffer* pCB = CDevice::GetInst()->GetCB(CB_TYPE::TRANSFORM);
	pCB->SetData(&g_transform, sizeof(tTransform));
	pCB->UpdateData();
}

void CCollider2D::render()
{
	UpdateData();

	if (KEY_TAP(KEY::_8))
	{
		if (m_ColShow == 0)
			m_ColShow = 1;
		else if (m_ColShow == 1)
			m_ColShow = 0;
	}	

	if (KEY_TAP(KEY::_9))
	{
		m_ColShow = 0;
	}

	m_pMtrl->SetScalarParam(SCALAR_PARAM::INT_0, &m_iCollisionCount);
	m_pMtrl->SetScalarParam(SCALAR_PARAM::INT_1, &m_ColShow);
	m_pMtrl->UpdateData();

	m_pMesh->render();
}

void CCollider2D::OnCollisionEnter(CCollider2D* _Other)
{
	++m_iCollisionCount;

	const vector<CScript*>& vecScript = GetOwner()->GetScripts();
	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		vecScript[i]->OnCollisionEnter(_Other->GetOwner());
	}		
}

void CCollider2D::OnCollision(CCollider2D* _Other)
{	
	const vector<CScript*>& vecScript = GetOwner()->GetScripts();
	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		vecScript[i]->OnCollision(_Other->GetOwner());
	}
}

void CCollider2D::OnCollisionExit(CCollider2D* _Other)
{
	--m_iCollisionCount;

	const vector<CScript*>& vecScript = GetOwner()->GetScripts();
	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		vecScript[i]->OnCollisionExit(_Other->GetOwner());
	}
}

void CCollider2D::SaveToScene(FILE* _pFile)
{
	CComponent::SaveToScene(_pFile);

	fwrite(&m_eColliderType, sizeof(UINT), 1, _pFile);
	fwrite(&m_vOffsetPos, sizeof(Vec2), 1, _pFile);
	fwrite(&m_vOffsetScale, sizeof(Vec2), 1, _pFile);
}

void CCollider2D::LoadFromScene(FILE* _pFile)
{
	CComponent::LoadFromScene(_pFile);

	fread(&m_eColliderType, sizeof(UINT), 1, _pFile);
	fread(&m_vOffsetPos, sizeof(Vec2), 1, _pFile);
	fread(&m_vOffsetScale, sizeof(Vec2), 1, _pFile);

	SetCollider2DType(m_eColliderType);
}
