#pragma once
#include <Engine/CScript.h>

class CBarrier_Bullet :
	public CScript
{

public:
    virtual void update() override;

    

    CLONE(CBarrier_Bullet)
public:
    CBarrier_Bullet();
    ~CBarrier_Bullet();
};

