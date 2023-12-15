#pragma once
#include "ComponentUI.h"
class ParticleUI :
    public ComponentUI
{
private:

public:
    virtual void update() override;
    virtual void render_update() override;
public:
    ParticleUI();
    ~ParticleUI();
};

