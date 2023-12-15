#pragma once
#include <Engine/CScript.h>

class CRoomEvent_4 :
    public CScript
{
private:
    bool  m_Start;
    vector<CGameObject*> vObj;
    bool  m_b;

public:
    virtual void update() override;
    virtual void OnCollisionEnter(CGameObject* _pOtherObj) override;

    CLONE(CRoomEvent_4)
public:
    CRoomEvent_4();
    ~CRoomEvent_4();
};

