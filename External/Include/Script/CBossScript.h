#pragma once
#include "CMonsterScript.h"

class CBossScript :
    public CMonsterScript
{
private:
    float   m_fBossBulletTime;
    int     i_BossBulletCnt;

    float   f_Dir;
    bool    b_Dir;

    bool    m_bSummonHeadGun;
    bool    m_Sound;

    int     m_iOrder;
    CGameObject* m_HeadGun;

public:
    virtual void start() override;
    virtual void StartState() override;
    virtual void Idle() override;
    virtual void Trace() override;
    
    virtual void Attack() override;
    virtual void Skill1() override;
    virtual void Skill2() override;
    virtual void Skill3() override;
    virtual void Skill4() override;
    virtual void Skill5() override;
    virtual void Dead() override;

public:
    virtual void OnCollisionEnter(CGameObject* _OtherObject) override;

    CLONE(CBossScript)


private:
    void Shoot_Atk();
    void Execute_Skill1();
    void Execute_Skill2();
    void Execute_Skill3();
    void Execute_Skill4();
    void Execute_Skill5();

    void TracePlayer();
    void ChangeToAttackState();
    int GetRandomInt();
    void StartBossAnimation();
    void SummonHeadGun();
    bool CanAttack() const;
    class CBoss_Bullet* CreateBossBulletScript();
    void IsAttackPossible(float AttackDelay);
public:
    CBossScript();
    ~CBossScript();
};


