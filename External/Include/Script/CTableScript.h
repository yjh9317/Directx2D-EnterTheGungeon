#pragma once
#include <Engine/CScript.h>

enum class TABLE_TYPE
{
    Idle,
    Stand,
    Break,
};


class CTableScript :
    public CScript
{
private:
    TABLE_TYPE  m_type;
    bool    m_bDamagedAble;

    float   m_fTime;
    bool    m_bTime;

    int    m_iDir;
    int    m_HP;

public:
    virtual void update() override;
    void CheckIsDamaged();
    void CheckDamagedTable_Left();
    void CheckDamagedTable_Right();
    void CheckDamagedTable_Down();
    void CheckDamagedTable_Up();
    virtual void OnCollisionEnter(CGameObject* _OtherObject) override;
    TABLE_TYPE GetType() const { return m_type; }
private:
    void IsCollide(CGameObject* _OtherObject);
    void Check();
public:
    CTableScript();
    ~CTableScript();
    CLONE(CTableScript)

};

