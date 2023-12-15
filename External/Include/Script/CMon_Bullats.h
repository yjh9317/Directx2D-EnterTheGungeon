#pragma once
#include "CMonsterScript.h"
class CMon_Bullats :
    public CMonsterScript
{
private:
    bool    m_bSound;
    bool    m_bDeathSound;
    Vec2    m_vDir;
protected:
    virtual void Idle() override;
    virtual void Trace() override;
    virtual void Attack() override;
    
    virtual void Dead() override;
    virtual void OnCollisionEnter(CGameObject* _OtherObject) override;

private:
    void IsAttackStateAvailable(Vec3& vMonPos, Vec3& vPlayerPos);
    void CheckTackleSound();
    void CheckChargeSound();
    void CheckDeathSound();
    void CheckAttackTime();
    void SetTracePosition(Vec3& vMonPos, Vec3& vPlayerPos);
    void TimeDestroyCheck(float Time);
    void CheckDeath();

public:
    CMon_Bullats();
    ~CMon_Bullats();
    CLONE(CMon_Bullats)
};

