#pragma once

#include <Engine/CScript.h>
class CPlayerStateScript;

class CPlayerScript :
    public CScript
{
private:
    float               m_fSpeed;
    

public:
    virtual void start();
    virtual void update();
    virtual void lateupdate();

    virtual void OnCollision(CGameObject* _OtherObject) override;


public:
    virtual void SaveToScene(FILE* _pFile) override;
    virtual void LoadFromScene(FILE* _pFile) override;

    CLONE(CPlayerScript)

public:
    CPlayerScript();
    ~CPlayerScript();

};

//float           m_fBurnStrength;
//bool            m_bBurn;
//    void Burnning();