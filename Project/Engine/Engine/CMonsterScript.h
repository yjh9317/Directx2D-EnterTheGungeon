#pragma once
#include <Engine/CScript.h>

enum class MON_STATE
{
    IDLE,
    TRACE,
    ATK,
    HIT,
    SPECIAL,
    DEAD,
};

class CMonsterScript :
    public CScript
{


private:
    MON_STATE       m_eState;


protected:
    float           m_HP;
    float           m_fTime;
    float           m_DetectRange;
    float           m_AttackRange;
    float           m_fDistance;
    float           m_fSpeed;
    Vec2            m_Dir;
    float           m_Angle;
    bool            m_Atkable;
    float           m_BehaviorAngle;

public:
    virtual void start() override;
    virtual void update() override;
    virtual void ChangeState(MON_STATE _eState);


public:
    virtual void Idle();
    virtual void Trace();
    virtual void Attack();
    virtual void Hit() {};
    virtual void Special() {};
    virtual void Dead() {};



    CLONE(CMonsterScript);
public:
    CMonsterScript();
    ~CMonsterScript();

    friend class CEventMgr;
};

