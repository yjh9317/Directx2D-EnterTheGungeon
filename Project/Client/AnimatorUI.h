#pragma once
#include "ComponentUI.h"
class AnimatorUI :
    public ComponentUI
{
private:
    CGameObject*    m_PrevObj;
    CGameObject*    m_pObj;
    string          m_SaveName;
    string          m_LoadName;

    int             m_PreviewIdx;
    int             m_PreviewFrameIdx;
    float           m_Time;
    bool            m_PreviewChange;

    // Load«“ File Name Vector
    vector<string> LoadNameVector;

public:
    virtual void update() override;
    virtual void render_update() override;

    void Save(const wstring& AnimName);
    void Load(const wstring& AnimName);
    void SelectAnim(DWORD_PTR _dw);

    void PreviewZero() { m_PreviewIdx = 0; }

private:
    void Preview();
    void AnimList();
    void CreateAnimByTool();
    void SaveLoadButton();

    void FindAnimFile(const wstring& _strFolderPath);
    void LoadCombo();
public:
    AnimatorUI();
    ~AnimatorUI();
};

