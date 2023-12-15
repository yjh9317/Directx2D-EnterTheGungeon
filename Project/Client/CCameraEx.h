#pragma once
#include <Engine\CCamera.h>

class CCameraEx :
    public CCamera
{
public:
    virtual void update() override;
    virtual void finalupdate() override;

public:
    CCameraEx();
    ~CCameraEx();
};

