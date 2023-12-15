#pragma once
#include "UI.h"

class CGameObject;


class AnimationUI :
    public UI
{
public:
    CGameObject* m_pCurObj;
    Ptr<CTexture>    m_CurTex;

    // 저장할 애니메이션 이름
    char                    Anim_Name[256];
    int                     m_idx;
    int                     m_MaxIdx;

    // 저장할 애니메이션 벡터
    vector<tAnim2DFrame>    FrameVec;

    // 수정할 애니메이션 벡터
    tAnim2DFrame            m_ReviseFrame;
    int                     m_ReviseIdx;
    

    // 현재 프레임 정보
    tAnim2DFrame    Frame;
    Vec2            m_vBackSize;

    
    // 이미지 박스 관련 변수
    ImVec2                  m_RectPos;  // 이미지박스 좌상단 위치
    bool                    m_Check;    // 이미지박스 띄울지 결정
    float                   m_UpLine;   // 이미지박스 가로 길이
    float                   m_LeftLine; // 이미지박스 세로 길이
      
    //FrameVector 이미지 띄우기 위한 변수
    bool          m_bCheck;
    float         m_fAccTime;
    
    // 리소스 텍스쳐 이름 저장하는 벡터
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

