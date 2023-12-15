#pragma once
#include <Engine/CScript.h>

class CIce_Bullet :
    public CScript
{
private:
    Vec3    m_vDir;
    float   m_Speed;
    float   m_fTime;

public:
    virtual void start();
    virtual void update();
    virtual void OnCollisionEnter(CGameObject* _OtherObject) override;
    void SetDir(Vec3 _v) { m_vDir = _v; }
    void SetSpeed(float _f) { m_Speed = _f; }
private:
    void TimeDestroyCheck(float Time);
    void UpdatePosition();
public:
    CIce_Bullet();
    ~CIce_Bullet();
    
    CLONE(CIce_Bullet)
};

