#pragma once
#include <Engine/CScript.h>

enum class PLAYER_TYPE
{
    IDLE,
    WALK,
    ROLL,
    FALL,
    HIT,
    DEAD,
};

class CPlayerStateScript :
    public CScript
{
public:
    PLAYER_TYPE m_eStateType;
    CGameObject* m_pOwner;
    float       m_fSpeed;
    float       m_fTime;
    Vec2        m_MousePos;
    Vec2        m_ClickMousePos;

public:
    virtual void start() override;
    virtual void update() override;
    virtual void lateupdate() override;

    void SetOwner(CGameObject* m_pObj) {m_pOwner = m_pObj; }
    void ChangeState(PLAYER_TYPE);

public:
    void Idle();
    void Walk();
    void Roll();
    void Dead();
    void Fall();


public:
    void SetClickMousePos(Vec2 _v) { m_ClickMousePos = _v; }
    Vec2 GetClickMousePos() { return m_ClickMousePos; }
    CLONE(CPlayerStateScript);

public:
    CPlayerStateScript();
    ~CPlayerStateScript();
};

