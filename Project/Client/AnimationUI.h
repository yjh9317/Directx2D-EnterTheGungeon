#pragma once
#include "UI.h"

class CGameObject;


class AnimationUI :
    public UI
{
public:
    CGameObject* m_pCurObj;
    Ptr<CTexture>    m_CurTex;

    // ������ �ִϸ��̼� �̸�
    char                    Anim_Name[256];
    int                     m_idx;
    int                     m_MaxIdx;

    // ������ �ִϸ��̼� ����
    vector<tAnim2DFrame>    FrameVec;

    // ������ �ִϸ��̼� ����
    tAnim2DFrame            m_ReviseFrame;
    int                     m_ReviseIdx;
    

    // ���� ������ ����
    tAnim2DFrame    Frame;
    Vec2            m_vBackSize;

    
    // �̹��� �ڽ� ���� ����
    ImVec2                  m_RectPos;  // �̹����ڽ� �»�� ��ġ
    bool                    m_Check;    // �̹����ڽ� ����� ����
    float                   m_UpLine;   // �̹����ڽ� ���� ����
    float                   m_LeftLine; // �̹����ڽ� ���� ����
      
    //FrameVector �̹��� ���� ���� ����
    bool          m_bCheck;
    float         m_fAccTime;
    
    // ���ҽ� �ؽ��� �̸� �����ϴ� ����
    vector<pair<string,CTexture*>> m_vTexture;

public:
    virtual void update() override;
    virtual void render_update() override;

    void SetObj(CGameObject* Obj) { m_pCurObj = Obj; }

    void CurFrame();
    void FrameVecInfo();
    void SelectTexture();
    void Canvas();

    void PlayFrame();
    void ReviseFrame();

    void EnumerateImage();


public:
    AnimationUI();
    ~AnimationUI();
};

