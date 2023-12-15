#pragma once
#include <Engine/CScript.h>

class CHandScript :
    public CScript
{
private:
    Vec3    m_Pos;
    Vec2    m_MousePos;

public:
    virtual void start() override;
    virtual void update() override;
    void SetPos();
    CLONE(CHandScript)

private:
    void SetScale();

public:
    CHandScript();
    ~CHandScript();
};

