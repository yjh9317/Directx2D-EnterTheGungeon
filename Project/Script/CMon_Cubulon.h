#pragma once
#include "CMonsterScript.h"
class CMon_Cubulon :
    public CMonsterScript
{
private:
    bool    m_bSound;
protected:
    virtual void start() override;
    virtual void Idle() override;
    virtual void Trace() override;
    virtual void Attack() override;

    virtual void Hit() override;
    virtual void Dead() override;
    virtual void OnCollisionEnter(CGameObject* _OtherObject) override;

private:
    void Shoot();
    void CreateAllBullet(Ptr<CPrefab>& pPrefab);
    void CreateBullet(CGameObject* pBullet, const Vec3& Pos, float Speed, const Vec3& Dir,int Count);
    void DecideIdleAnimation();
    void IsAttackStateAvailable();
    void SetPosition();
    void TimeDestroyCheck(float Time);
    void CheckDeadSound();
    void DecideTraceAnimation();
    void IsAttackPossible();
    void DecideAttackAnimation();
    void ChangeAttackToIdle();
    void CheckAndTransitionHitToIdle();
    void DecideHitAnimation();
    void CheckDeath();
    void CheckHit();
    void CheckAttackTime();
public:
    CMon_Cubulon();
    ~CMon_Cubulon();
    CLONE(CMon_Cubulon)
};

