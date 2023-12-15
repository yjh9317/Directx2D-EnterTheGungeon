#pragma once
#include "CMonsterScript.h"
class CMon_Maiden :
    public CMonsterScript
{
private:
    bool    m_bAtk;
    bool    m_bAtk2;
    float   i_Dir;
    
    bool    m_Sound;

    float   m_WalkTime;
    bool    m_WalkSound;

protected:
    virtual void Idle() override;
    virtual void Trace() override;
    virtual void Attack() override;
    virtual void Dead() override;
    virtual void OnCollisionEnter(CGameObject* _OtherObject) override;

private:
    void ShowDeathAnimation();
    void CanAttack();
    void IsFinishAttack();
    void AttackSound();
    void Attack2();
    void Attack1();
    void IsAttackPossible();
    void Move();
    void CheckWalkSound();
    void HitIceBullet(CGameObject* _OtherObject);
    void HitBaseBullet(CGameObject* _OtherObject);
    void Shoot();
    void Shoot2();

public:
    CMon_Maiden();
    ~CMon_Maiden();
    CLONE(CMon_Maiden)
};

