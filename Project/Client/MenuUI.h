#pragma once
#include "UI.h"


class MenuUI :
    public UI
{
private:
    string  m_strPlayPause;

    //오브젝트 생성
    string  m_ObjName;
    int     m_Layer;

    bool    m_bPackaging;
    bool    m_bSceneSave;
    bool    m_bSceneLoad;
    bool    m_bScenePlayPause;
    bool    m_bSceneStop;
    bool    m_bObjCreate;

    // Componenet
    bool    m_MtrlCheck;
    bool    m_CamCheck;
    bool    m_Col2DCheck;
    bool    m_Anim2DCheck;
    bool    m_Light2DCheck;
    bool    m_TileMapCheck;
    bool    m_ParticleCheck;
    bool    m_bCheck;

    // Script
    bool    m_ScriptCheck;
    size_t  m_ScriptIdx;

    //CollisionCheck
    bool LayerCheck[MAX_LAYER][MAX_LAYER];
   

public:
    virtual void update() override;
    virtual void render() override;
    virtual void render_update() override;

private:
    void Task();
    void ComponenetCheck();
    void ScriptCheck();

public:
    MenuUI();
    ~MenuUI();
};

