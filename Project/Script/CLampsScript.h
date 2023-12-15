#pragma once
#include <Engine/CScript.h>

class CLampsScript :
    public CScript
{

public:
    virtual void start() override;
    virtual void update() override;

    CLONE(CLampsScript)

public:
    CLampsScript();
    ~CLampsScript();
};

