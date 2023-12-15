#pragma once

#include <Engine/CScript.h>

class CBarrelScript :
    public CScript
{
private:
    bool    m_bBreak;

public:
    virtual void start() override;
    virtual void update() override;

    virtual void OnCollisionEnter(CGameObject* _OtherObj) override;

    CLONE(CBarrelScript)
public:
    CBarrelScript();
    ~CBarrelScript();
};

