#pragma once
#include <Engine/CScript.h>

class CMovementScript :
    public CScript
{
private:
    Vec3    m_PrevPos;

    int     m_iRow;
    int     m_iCol;

    int     m_iTileIdx;

public:
    virtual void update() override;
    Vec3 GetPrevPos() { return m_PrevPos; }

    CLONE(CMovementScript)

    virtual void OnCollision(CGameObject* _OtherObj) override;

public:
    int GetRow() { return m_iRow; }
    int GetCol() { return m_iCol; }
    int GetTileIdx() { return m_iTileIdx; }

public:
    CMovementScript();
    ~CMovementScript();
};

