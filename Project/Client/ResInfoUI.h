#pragma once

#include "UI.h"

#include <Engine/CRes.h>

class ResInfoUI :
    public UI
{
private:
    CRes*               m_pRes;     // ������ ����� ���ҽ�
    const RES_TYPE      m_eResType;    // �ش� ���ҽ� Ÿ��

public:
    void SetTargetRes(CRes* _pRes){m_pRes = _pRes;}
    CRes* GetTargetRes() {return m_pRes;}

public:
    virtual void render_update() override;

public:
    ResInfoUI(const string& _strName, RES_TYPE _eType);
    ~ResInfoUI();
};

