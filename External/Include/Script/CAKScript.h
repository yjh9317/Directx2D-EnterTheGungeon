#pragma once
#include <Engine/CScript.h>

class CAKScript :
    public CScript
{
private:
    Vec3    m_Pos;
    float   m_fTime;
    bool    m_Atk;
    bool    m_bRight;

public:
    virtual void start() override;
    virtual void update() override;
    void Attack(float Dist, Vec2& Dir);
    void SetRotation(float Angle, float Radian);
    CLONE(CAKScript)

private:
    void IsAttackPossible(float Time);
public:
    CAKScript();
    ~CAKScript();
};

