#pragma once
#include "CMonsterScript.h"

class CMon_Gigi :
    public CMonsterScript
{
private:
    bool    m_Sound;

protected:
    virtual void start() override;
    virtual void Idle() override;
    virtual void Trace() override;
    virtual void Attack() override;
    virtual void Hit() override;
    virtual void Dead() override;
    virtual void OnCollisionEnter(CGameObject* _OtherObject) override;

private:
    void DecideIdleAnimation();
    void IsAttackPossible();
    void Shoot();
    void InitBulletAndCreate(CGameObject* pObj, Vec3& pTrans, Vec2& vDir);
    void DecideTraceAnimation();
    void SetPosition();
    void IsAttackStateAvailable();
    void DecideAttackDirection();
    void CheckAttackTime();
    void CheckAndTransitionHitToIdle();
    void DecideHitDirection();
    void TimeDestroyCheck(float Time);
    void CheckDeathSound();
    void CheckHit();
    void CheckDeath();

public:
    CMon_Gigi();
    ~CMon_Gigi();
    CLONE(CMon_Gigi)

};

