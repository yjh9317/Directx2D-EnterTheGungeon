#include "pch.h"
#include "CMesh.h"

#include "CDevice.h"

CMesh::CMesh()
	: CRes(RES_TYPE::MESH)
	, m_tVBDesc{}
	, m_tIBDesc{}
	, m_iVtxCount(0)
	, m_iIdxCount(0)
	, m_pVtxSys(nullptr)
	, m_pIdxSys(nullptr)
{
}

CMesh::~CMesh()
{
	SAFE_DELETE(m_pVtxSys);
	SAFE_DELETE(m_pIdxSys);
}

int CMesh::Create(void* _pVtxSys, UINT _iVtxCount, void* _pIdxSys, UINT _iIdxCount)
{
	m_iVtxCount = _iVtxCount;
	m_iIdxCount = _iIdxCount;

	// ���� �����͸� ������ ���ؽ� ���۸� �����Ѵ�.	
	m_tVBDesc.ByteWidth = sizeof(Vertex) * _iVtxCount;

	// ���� ���۴� ó�� �������Ŀ� ���۸� �������� �ʴ´�.
	m_tVBDesc.CPUAccessFlags = 0;
	m_tVBDesc.Usage = D3D11_USAGE_DEFAULT;

	// ������ �����ϴ� ������ ���� ���� �˸�
	m_tVBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	m_tVBDesc.MiscFlags = 0;
	m_tVBDesc.StructureByteStride = 0;

	// �ʱ� �����͸� �Ѱ��ֱ� ���� ���� ����ü
	D3D11_SUBRESOURCE_DATA tSubDesc = {};
	tSubDesc.pSysMem = _pVtxSys;

	if (FAILED(DEVICE->CreateBuffer(&m_tVBDesc, &tSubDesc, m_VB.GetAddressOf())))
	{
		return E_FAIL;
	}


	m_tIBDesc.ByteWidth = sizeof(UINT) * _iIdxCount;

	// ���� ���� ���Ŀ���, ������ ������ ���� �� �� �ִ� �ɼ�
	m_tIBDesc.CPUAccessFlags = 0;
	m_tIBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;

	// ������ �����ϴ� ������ ���� ���� �˸�
	m_tIBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
	m_tIBDesc.MiscFlags = 0;
	m_tIBDesc.StructureByteStride = 0;

	// �ʱ� �����͸� �Ѱ��ֱ� ���� ���� ����ü
	tSubDesc = {};
	tSubDesc.pSysMem = _pIdxSys;

	if (FAILED(DEVICE->CreateBuffer(&m_tIBDesc, &tSubDesc, m_IB.GetAddressOf())))
	{
		return E_FAIL;
	}

	// �ý��� �޸� �Ҵ�
	m_pVtxSys = new Vtx[m_iVtxCount];
	memcpy(m_pVtxSys, _pVtxSys, sizeof(Vtx) * m_iVtxCount);
	
	m_pIdxSys = new UINT[m_iIdxCount];
	memcpy(m_pIdxSys, _pIdxSys, sizeof(UINT) * m_iIdxCount);

    return S_OK;
}

void CMesh::render()
{
	UpdateData();

	CONTEXT->DrawIndexed(m_iIdxCount, 0, 0);
}

void CMesh::render_particle(UINT _iCount)
{
	UpdateData();

	CONTEXT->DrawIndexedInstanced(m_iIdxCount, _iCount, 0, 0, 0);
}

void CMesh::UpdateData()
{
	UINT iStride = sizeof(Vtx);
	UINT iOffset = 0;

	CONTEXT->IASetVertexBuffers(0, 1, m_VB.GetAddressOf(), &iStride, &iOffset);
	CONTEXT->IASetIndexBuffer(m_IB.Get(), DXGI_FORMAT_R32_UINT, 0);
}