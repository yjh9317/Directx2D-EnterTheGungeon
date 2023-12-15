#pragma once
#include "CComponent.h"

#include "CTexture.h"

class CAnimation2D;

class CAnimator2D :
    public CComponent
{
private:
    map<wstring, CAnimation2D*> m_mapAnim;
    CAnimation2D*               m_pCurAnim;
    bool                        m_bRepeat;

public:
    virtual void finalupdate() override;
    virtual void UpdateData() override;
    static void Clear();

public:
    CAnimation2D* FindAnim(const wstring& _strName);
    void CreateAnim(const wstring& _strName, Ptr<CTexture> _pAtlas, Vec2 _vBackgroundSizePixel, Vec2 _vLeftTopPixel, Vec2 _vSlicePixel, Vec2 _vStepPixel, float _fDuration, int _iFrameCount);
    void Play(const wstring& _strName, bool _bRepeat);

    void CreateAnimByTool(const wstring& _strName, Ptr<CTexture> _pAtlas, Vec2 _vBackgroundSizePixel, const vector<tAnim2DFrame>& VecFrame, int _iFrameCount);
    void SetRepeat(bool _b) { m_bRepeat = _b; }

    map<wstring, CAnimation2D*>& GetAnimList() { return m_mapAnim; }
    CAnimation2D* GetCurAnim() { return m_pCurAnim; }
    void SetCurAnim(CAnimation2D* m_CurAnim) { m_pCurAnim = m_CurAnim; }

public:
    virtual void SaveToScene(FILE* _pFile) override;
    virtual void LoadFromScene(FILE* _pFile) override;
    CLONE(CAnimator2D)

public:
    CAnimator2D();
    CAnimator2D(const CAnimator2D& _origin);
    ~CAnimator2D();
};

