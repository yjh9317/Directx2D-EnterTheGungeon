#pragma once
#include <Engine/CScript.h>

class CBlankBulletScript :
    public CScript
{
private:
    int     m_Num;
public:
    virtual void update() override;

    CLONE(CBlankBulletScript)
public:
    void SetNum(int _i) { m_Num = _i; }
    int GetNum() { return m_Num; }

public:
    CBlankBulletScript();
    ~CBlankBulletScript();
};

