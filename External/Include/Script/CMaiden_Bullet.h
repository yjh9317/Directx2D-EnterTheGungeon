#pragma once
#include <Engine/CScript.h>

class CMaiden_Bullet :
    public CScript
{
private:
    Vec3    m_vDir;
    float   m_fSpeed;
    float   m_fTime;

    bool    m_bWait;
    float   m_fWaitTime;

public:
    virtual void update() override;
    virtual void OnCollisionEnter(CGameObject* _OtherObject) override;

    void SetDir(Vec3 _v) { m_vDir = _v; }
    void SetSpeed(float _f) { m_fSpeed = _f; }
    void WaitCheck(bool _b) { m_bWait = _b; }
    void WaitTime(float _f) { m_fWaitTime = _f; }
    bool GetWait() const { return m_bWait; }
    void SetWait(bool _b) { m_bWait = _b; }

private:
    void SetRotation();
    void SetPosition();
    void IsDestroy(float Time);
public:
    CMaiden_Bullet();
    ~CMaiden_Bullet();
    CLONE(CMaiden_Bullet)
};

