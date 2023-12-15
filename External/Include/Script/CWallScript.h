#pragma once
#include <Engine/CScript.h>

class CWallScript :
    public CScript
{

public:
    virtual void update() override;
    virtual void OnCollisionEnter(CGameObject* _OtherObject);

public:
    CWallScript();
    ~CWallScript();
    CLONE(CWallScript)
};

