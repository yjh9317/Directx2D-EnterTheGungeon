#pragma once
#include <Engine/CScript.h>

class CBossRoomEvent :
    public CScript
{
private:
    bool  m_Start;
    bool  m_b;

    bool  m_Sound;
public:
    virtual void update() override;

    virtual void OnCollisionEnter(CGameObject* _pOtherObj) override;

    CLONE(CBossRoomEvent)

public:
    CBossRoomEvent();
    ~CBossRoomEvent();
};

