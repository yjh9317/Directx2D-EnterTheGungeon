#pragma once
#include "ResInfoUI.h"
class PrefabUI :
    public ResInfoUI
{
private:
    CGameObject* m_pObj;

public:
    virtual void update() override;
    virtual void render_update() override;

    void AboutTransform(CGameObject* _Obj);
    void AboutMtrl(CGameObject* _Obj);
    void AboutCollider2D(CGameObject* _Obj);
    void AboutAnimation2D(CGameObject* _Obj);
    

    void AboutScript(CGameObject* _Obj);

public:
    PrefabUI();
    ~PrefabUI();
};

