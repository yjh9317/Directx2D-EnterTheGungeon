#pragma once
#include <Engine/CScript.h>

class CHeartScript :
    public CScript
{
private:
    int     m_Num;
    int     m_HP;

public:
    virtual void update() override;
    CLONE(CHeartScript)

public:
    void SetNum(int _i) { m_Num = _i; }
    int GetNum() { return m_Num; }

    void Damaged() { --m_HP; }
    int GetHP() { return m_HP; }
     
public:
    CHeartScript();
    ~CHeartScript();
};

