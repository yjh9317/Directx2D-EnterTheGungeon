#pragma once
#include <Engine/CScript.h>

class CP_Bullet :
    public CScript
{
private:
    Vec3    m_Dir;
    float   m_Speed;
    float   m_fTime;

public:
    virtual void update();
    void SetPosition();
    virtual void OnCollisionEnter(CGameObject* _OtherObject) override;
    void SetDir(Vec3 _v) { m_Dir = _v; }
    void SetSpeed(float _f) { m_Speed = _f; }
   
private:
    void TimeDestroyCheck(float Time);
public:
    CP_Bullet();
    ~CP_Bullet();
    CLONE(CP_Bullet)
};

