#pragma once
#include <Engine/CScript.h>

class CHeadGunScript :
    public CScript
{
private:
    float   m_fTime;
    float   m_fDeathTime;
    bool    m_bAttack;


public:
    virtual void start() override;
    virtual void update() override;
    
    void SetDestroyTime(float Time) { m_fDeathTime = Time; }
    
private:
    void Shoot();
    void IdleAnimationBasedOnAngle(float Angle);
public:
    CHeadGunScript();
    ~CHeadGunScript();
    CLONE(CHeadGunScript)
};

