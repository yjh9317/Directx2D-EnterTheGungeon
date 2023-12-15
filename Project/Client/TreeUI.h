#pragma once
#include "UI.h"

#include <Engine/CKeyMgr.h>

class TreeUI;

class TreeNode
{
private:
    TreeUI*             m_pTreeUI;
    TreeNode*           m_pParent;
    vector<TreeNode*>   m_vecChild;
    bool                m_bLeaf;
    bool                m_bSelected;

    string              m_strName;
    DWORD_PTR           m_dwData;    


public:
    void update();
    void render_update();

public:
    void AddChild(TreeNode* _pChild) 
    { 
        _pChild->m_pParent = this;
        m_vecChild.push_back(_pChild); 
        m_bLeaf = false; 
    }

    void RemoveChild(string _str);

    const vector<TreeNode*>& GetChild() { return m_vecChild; }
    const string& GetName() { return m_strName; }
    DWORD_PTR GetData(){return m_dwData;}
    void SetData(DWORD_PTR _dw) { m_dwData = _dw; }
    TreeNode* GetParent() { return m_pParent; }



public:
    TreeNode();
    TreeNode(const string& _strName, DWORD_PTR _dwData);
    ~TreeNode();

    friend class TreeUI;
};


typedef void(UI::* CLICKED)(DWORD_PTR);
typedef void(UI::* DRAG_DROP)(DWORD_PTR, DWORD_PTR);
typedef CLICKED KEY_FUNC;


struct tTreeKey
{
    KEY eKey;
    UI* pInst;
    CLICKED pFunc;
};


class TreeUI :
    public UI
{
private:
    TreeNode*           m_pRootNode;
    TreeNode*           m_pSelectedNode;
    TreeNode*           m_pDragNode;
    TreeNode*           m_pDropNode;

    const bool          m_bUseDummyRoot;
    bool                m_bShowDummy;
    bool                m_bUseFrame;

    bool                m_bUseDragDropSelf;
    bool                m_bUseDragDropOuter;

    int                 m_iTest;

    // Clicked Delegate
    UI*                 m_pCInst;
    CLICKED             m_CFunc;

    // Double Clicked
    UI*                 m_pDBCInst;
    CLICKED             m_DBCFunc;

    // Drag And Drop
    UI*                 m_pDADInst;
    DRAG_DROP           m_DADFunc;

    // Key Binding Delegate
    vector<tTreeKey>    m_vecKeyBind;

public:
    virtual void update() override;
    virtual void render_update() override;

public:    
    void ShowDummyRoot(bool _bTrue){m_bShowDummy = _bTrue;}
    void UseFrame(bool _b) { m_bUseFrame = _b; }
    void UseDragDropOuter(bool _b) { m_bUseDragDropOuter = _b; }
    void UseDragDropSelf(bool _b) { m_bUseDragDropSelf = _b; }
    TreeNode* AddTreeNode(TreeNode* _pParentNode, const string& _strName, DWORD_PTR _dwData = 0);
    TreeNode* GetDummyNode() { return m_pRootNode; }

    void SetClickedDelegate(UI* _pInst, CLICKED _Func){m_pCInst = _pInst; m_CFunc = _Func;}
    void SetDoubleClickedDelegate(UI* _pInst, CLICKED _Func){m_pDBCInst = _pInst;m_DBCFunc = _Func;}
    void SetDragAndDropDelegate(UI* _pInst, DRAG_DROP _Func){m_pDADInst = _pInst;m_DADFunc = _Func;}
    void SetKeyBinding(KEY _eKey, UI* _pInst, KEY_FUNC _Func);

    TreeNode* GetSelectedNode() { return m_pSelectedNode; }
    void Clear();

private:
    void SetSelectedNode(TreeNode* _pNode);   
    void SetDBClickedNode(TreeNode* _pNode);

public:
    TreeUI(bool _bDummyRoot);
    ~TreeUI();

    friend class TreeNode;
};

