#pragma once
#include "ResInfoUI.h"
class ComputeShaderUI :
    public ResInfoUI
{
public:
    virtual void update() override;
    virtual void render_update() override;

public:
    ComputeShaderUI();
    ~ComputeShaderUI();
};