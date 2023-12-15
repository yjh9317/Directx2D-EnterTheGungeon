#pragma once
#include <Engine/CScript.h>

enum class DOOR_TYPE
{
    Idle,
    Open,
    Opened,
    Close,
};

class CDoorScript :
    public CScript
{
private:
    float   m_fTime;
    bool    m_Open;
    DOOR_TYPE m_type;

public:
    virtual void start() override;
    virtual void update() override;
    virtual void OnCollisionEnter(CGameObject* _OtherObject);

public:
    DOOR_TYPE GetDoorType() { return m_type; }
    void SetDoorType(DOOR_TYPE _type) { m_type = _type; }

private:
    void IsCollide(class CMovementScript* pScript, CGameObject* _OtherObject);
    void SetDoorOpen();
    void IsOpened();
    void AnimationBasedOnState();
    
public:
    CDoorScript();
    ~CDoorScript();
    CLONE(CDoorScript)
};

