#pragma once
#include <Engine/CScript.h>

class CBoss_Bullet :
    public CScript
{
private:
    Vec3    m_Dir;
    float   m_Speed;
    float   m_fTime;
    float   m_Angle;

    bool    m_bStop;
    float   m_fWaitTime;

public:
    virtual void update() override;

    void SetDir(Vec3 _v) { m_Dir = _v; }
    void SetSpeed(float _f) { m_Speed = _f; }
    void SetAngle(float _f) { m_Angle = _f; }

    void WaitCheck(bool _b) { m_bStop = _b; }
    void WaitTime(float _f) { m_fWaitTime = _f; }

    CLONE(CBoss_Bullet)

public:
    virtual void OnCollisionEnter(CGameObject* _OtherObject) override;

private:
    void checkIfTimeIsUp(float Time);
    void SetPosition();
    void CheckIsStop();
public:
    CBoss_Bullet();
    ~CBoss_Bullet();
};

