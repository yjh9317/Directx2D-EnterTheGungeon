#pragma once
#include <Engine/CScript.h>

class CM_Bullet :
    public CScript
{
private:
    Vec3    m_vDir;
    float   m_fSpeed;
    float   m_fTime;

    bool    m_bWait;
    float   m_fWaitTime;


public:
    virtual void update();
    virtual void OnCollisionEnter(CGameObject* _OtherObject) override;

    void SetDir(Vec3 _v) { m_vDir = _v; }
    void SetSpeed(float _f) { m_fSpeed=_f; }

    void WaitCheck(bool _b) { m_bWait = _b; }
    void WaitTime(float _f) { m_fWaitTime = _f; }

private:
    void IsWait();
    void TimeDestroyCheck();
    void UpdatePosition();
public:
    CM_Bullet();
    ~CM_Bullet();
    CLONE(CM_Bullet)
};

