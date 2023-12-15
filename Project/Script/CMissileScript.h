#pragma once
#include <Engine/CScript.h>

class CMissileScript :
    public CScript
{
private:
    float       m_fSpeed;
    float       m_fAccTime;
    Vec3        m_vDir;
    bool        m_bExplosion;

    float       m_fExTime;
public:

    virtual void update();
    void UpdatePosition();
    void TimeOver();
    void Bomb();
    void SetRotation();
    virtual void OnCollisionEnter(CGameObject* _pOtherObj) override;

    CLONE(CMissileScript);

public:
    void SetDir(Vec3 _v) { m_vDir = _v; }

public:
    CMissileScript();
    ~CMissileScript();
};

