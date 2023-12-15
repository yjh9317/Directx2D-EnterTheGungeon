#pragma once
#include <Engine/CScript.h>

class CMaidenDead :
    public CScript
{
private:
    float m_fTime;

public:
    virtual void start() override;
    virtual void update() override;
    
    CLONE(CMaidenDead)
public:
    CMaidenDead();
    ~CMaidenDead();
};

