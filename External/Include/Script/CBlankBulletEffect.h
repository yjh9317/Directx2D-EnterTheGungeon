#pragma once
#include <Engine/CScript.h>

class CBlankBulletEffect :
    public CScript
{
private:
    float m_fTime;

public:
    virtual void update() override;

    CLONE(CBlankBulletEffect)
public:
    virtual void start() override;
    CBlankBulletEffect();
    ~CBlankBulletEffect();
};

