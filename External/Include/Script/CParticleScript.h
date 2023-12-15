#pragma once
#include <Engine/CScript.h>

class CParticleScript :
    public CScript
{
private:
    float   m_fTime;

public:
    virtual void update() override;
    CLONE(CParticleScript)
public:
    CParticleScript();
    ~CParticleScript();
};

