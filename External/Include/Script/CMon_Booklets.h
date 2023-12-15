#pragma once

#include "CMonsterScript.h"

class CMon_Booklets :
    public CMonsterScript
{
private:
    float   m_AtkCycle;

    bool    m_Sound;
    bool    m_DeathSound;

protected:
    virtual void Idle() override;
    virtual void Trace() override;
    virtual void Attack() override;
    virtual void Dead() override;
    virtual void OnCollisionEnter(CGameObject* _OtherObject) override;

    void CheckDeath();

private:
    void IsAttackPossible();
    void IsAttackStateAvailable();
    void SetPosition();
    
    void AttackCycle();
    void AttackToIdle();
    void SetAttackSound();

    void Shoot();
    
    void TimeDestroyCheck(float Time);
    void SetDeathSound();
public:
    CMon_Booklets();
    ~CMon_Booklets();

    CLONE(CMon_Booklets)
};

