#pragma once
#include <Engine/CScript.h>

#include <Engine/define.h>

class CPlayerStateScript :
    public CScript
{
private:
    // State
    PLAYER_TYPE m_eStateType;
    float       m_fSpeed;
    float       m_fTime;
    double      m_Angle;
    bool        m_HandCheck;

    // MousePos
    Vec2        m_MousePos;
    Vec2        m_ClickMousePos;

    // Damage
    int         m_iDamaged;
    int         m_iDamagedCnt;
    float       m_fDamagedTime;
    bool        m_bDamagedAble;

    // Weapon
    map<wstring, CGameObject*> m_mapGun;
    CGameObject* m_CurrentGun;

    // Heart And Bullet
    vector<CGameObject*> m_vHeart;
    vector<CGameObject*> m_vBullet;
    int         m_iHeart;
    int         m_iBlankBullet;

public:
    virtual void start() override;
    virtual void update() override;
    virtual void lateupdate() override;

    virtual void OnCollisionEnter(CGameObject* _OtherObject) override;
    PLAYER_TYPE GetPlayerType() const { return m_eStateType; }

    vector<CGameObject*>& GetHeart() { return m_vHeart; }
    vector<CGameObject*>& GetBlankBullet() { return m_vBullet; }
    void RegistGun(CGameObject* pGun);
private:
    virtual void SetState(PLAYER_TYPE _eState) override;
    void ChangeState(PLAYER_TYPE);
   
    CGameObject* FindWeaponWithName(wstring GunName);
    

public:
    void Idle();
    void Walk();
    void Roll();
    void Dead();
    void Fall();

    bool GetHandCheck() const { return m_HandCheck; }

private:
    void InitPlayerState();
    void CheckGunChange();
    void CheckState();
    void IsShowHand();
    void UpdateMousePos();
    void TakingDamage(int Count, float Time);

public:
    CPlayerStateScript();
    ~CPlayerStateScript();
    CLONE(CPlayerStateScript);
};

