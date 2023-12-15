#pragma once
#include "UI.h"

class TreeUI;
class TreeNode;

class ResourceUI :
    public UI
{
private:
    TreeUI* m_TreeUI;

    vector<wstring> m_vecResPath;
    bool m_RightClicked;

    string PrefabName;

public:
    virtual void update() override;
    virtual void render_update() override;

public:
    void Reset();
    TreeUI* GetTreeUI() { return m_TreeUI; }

private:
    void Reload();
    void Renew();
    void FindFileName(const wstring& _strFolderPath);

    void ItemClicked(DWORD_PTR _dwNode);
    void ItemDBClicked(DWORD_PTR _dwNode);

    void ClickPopUp();
    
    

    RES_TYPE GetResTypeFromExt(const wstring& _strExt);


public:
    ResourceUI();
    ~ResourceUI();
};

