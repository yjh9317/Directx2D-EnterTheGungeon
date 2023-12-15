#pragma once
#include <Engine/CScript.h>

class CGlobeScript :
    public CScript
{
private:
    bool    m_bBreak;

public:
    virtual void update() override;

    virtual void OnCollisionEnter(CGameObject* _OtherObj) override;
    CLONE(CGlobeScript)

public:
    CGlobeScript();
    ~CGlobeScript();
};

