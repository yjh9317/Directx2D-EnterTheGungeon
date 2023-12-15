#pragma once
#include <Engine/CScript.h>

enum class CHEST_TYPE
{
    Idle,
    Open,
    Opened,
};

class CChestScript :
    public CScript
{
private:
    float   m_fTime;
    bool    m_bOpen;
    bool    m_Gun;
    CHEST_TYPE m_type;


public:
    virtual void update() override;
    void SetOpened();
    void CreateIceGun();
    void TryOpen();
    void AnimationBasedOnState();
    virtual void OnCollisionEnter(CGameObject* _OtherObject) override;

    CLONE(CChestScript)

public:
    void Check();

public:
    CChestScript();
    ~CChestScript();
};

