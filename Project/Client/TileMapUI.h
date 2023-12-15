#pragma once
#include "ComponentUI.h"
class TileMapUI :
    public ComponentUI
{
private:
    CGameObject* m_pObj;
    string m_SaveName;
    string m_LoadName;


public:
    virtual void update() override;
    virtual void render_update() override;

    void TileMapSave(const wstring& TileName);
    void TileMapLoad(const wstring& TileName);



public:
    TileMapUI();
    ~TileMapUI();
};

