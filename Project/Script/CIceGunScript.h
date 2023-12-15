#pragma once
#include <Engine/CScript.h>

enum class GUN_TYPE
{
    LIE,
    HOLD,
};

class CIceGunScript :
    public CScript
{
private:
    GUN_TYPE m_type;
    Vec3    m_Pos;
    float   m_fTime;
    bool    m_Atk;
    bool    m_Dir;

    bool    m_ShootCheck;
    float   m_ShootTime;

public:
    virtual void start() override;
    virtual void update() override;
private:
    Vec3 GetWindowMousePos() const;
    void IsAttackPossible(float AttackDelay);
    void PickupIceGun();
    void SetScale();
    void Attack(Vec2& Dir);
    void SetRotation(float Angle, float Radian);
public:
    CIceGunScript();
    ~CIceGunScript();
    CLONE(CIceGunScript)
};

