#pragma once
#include "CMonsterScript.h"
class CMon_Kin :
    public CMonsterScript
{

protected:
    virtual void start() override;
    virtual void Idle() override;
    void DecideIdleAnimation();
    virtual void Trace() override;
    void SetPosition();
    void DecideTraceAnimation();
    virtual void Hit() override;
    void DecideHitAnimation();
    void CheckAndTransitionHitToIdle();
    virtual void Dead() override;
    void TimeDestroyCheck(float Time);
    virtual void OnCollisionEnter(CGameObject* _OtherObject) override;

    void PlayHurtSound();

    void CheckHit();

    void CheckDeath();

public:
    CMon_Kin();
    ~CMon_Kin();
    CLONE(CMon_Kin)
};

