#pragma once
#include <Engine/CScript.h>

class CMagnumScript :
    public CScript
{
private:
    Vec3    m_Pos;
    float   m_fTime;
    bool    m_bIsAttack;
    bool    m_bRight;

    bool    m_ShootCheck;
    float   m_ShootTime;

public:
    virtual void start() override;
    void SetScale();
    virtual void update() override;

    void SetRotation(float Radian, float Angle);

    void TryShoot(Vec2& Dir);

    

    CLONE(CMagnumScript)

private:
    Vec2 GetMousePose() const;
    void CanAttack(float AttackDelay);
public:
    CMagnumScript();
    ~CMagnumScript();
};

