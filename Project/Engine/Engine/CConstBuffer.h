#pragma once
#include "CEntity.h"

class CConstBuffer :
    public CEntity
{
private:
    ComPtr<ID3D11Buffer>		m_CB;
    D3D11_BUFFER_DESC           m_Desc;

    CB_TYPE                     m_eCBType; // ������۸� ���ε� �� �������� ��ȣ

public:
    virtual void UpdateData() override;
    void UpdateData_CS();
    int Create(UINT _iBufferSize);

    void SetData(void* _pData, UINT _iSize);
        
    virtual CConstBuffer* Clone() { return nullptr; }

public:
    CConstBuffer(CB_TYPE _eType);
    ~CConstBuffer();
};

