#pragma once
#include <Engine/CScript.h>
#include <Engine/define.h>

class CMonsterScript :
    public CScript
{
private:
    MON_STATE       m_eState;

protected:
    float           m_HP;
    float           m_fTime;
    float           m_fAttackTime;
    float           m_DetectRange;
    float           m_AttackRange;
    float           m_fDistance;
    float           m_fSpeed;
    Vec2            m_Dir;
    
    bool            m_bAttack;
    bool            m_Atk;

    float           m_Angle;
    float           m_BehaviorAngle;

    int             m_IceCheck;
    float           m_IceTime;
    bool            m_bPrevSpeed;
    float           m_fPrevSpeed;

public:
    virtual void start() override;
    virtual void update() override;
    void IsFrozen();
    virtual void ChangeState(MON_STATE _eState);
    MON_STATE GetState() { return m_eState; }

protected:
    virtual void SetState(MON_STATE _eState);
    virtual void StartState() {};
    virtual void Idle() {};
    virtual void Trace() {};
    virtual void Attack() {};
    virtual void Hit() {};
    virtual void Skill1() {};
    virtual void Skill2() {};
    virtual void Skill3() {};
    virtual void Skill4() {};
    virtual void Skill5() {};
    virtual void Dead() {};

    virtual void OnCollisionEnter(CGameObject* _OtherObject) {};
    virtual void OnCollision(CGameObject* _OtherObject) {};
    virtual void OnCollisionExit(CGameObject* _OtherObject) {};
protected:
    CGameObject* CreateBullet() const;

public:
    CMonsterScript();
    CMonsterScript(int _iScript);
    ~CMonsterScript();
    CLONE(CMonsterScript);

    friend class CEventMgr;

private:
    bool IsDead() const;
};

