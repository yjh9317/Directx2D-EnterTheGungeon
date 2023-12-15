#pragma once

#include "CEntity.h"

#include "CTexture.h"

class CAnimator2D;

class CAnimation2D
	: public CEntity
{
private:
	vector<tAnim2DFrame>	m_vecFrm;
	Ptr<CTexture>			m_pAtlasTex;
	Vec2					m_vBackgroundSize;

	CAnimator2D*			m_pOwner;
	int						m_iCurFrmIdx;
	float					m_fAccTime;
	bool					m_bFinish;


public:
	tAnim2DFrame& GetFrame(int _iFrmIdx) { return m_vecFrm[_iFrmIdx]; }
	UINT GetMaxFrame() { return (UINT)m_vecFrm.size(); }
	Ptr<CTexture> GetPtrTex() { return m_pAtlasTex; }

public:
	void finalupdate();

	virtual void UpdateData() override;

public:
	void Create(Ptr<CTexture> _Atlas, Vec2 _vBackgroundSizePixel, Vec2 _vLT, Vec2 _vSlice, Vec2 _vStep, float _fDuration, int _iFrameCount);
	bool IsFinish() { return m_bFinish; }
	void Reset()
	{
		m_fAccTime = 0.f;
		m_bFinish = false;
		m_iCurFrmIdx = 0;			
	}

	CTexture* GetTex() { return m_pAtlasTex.Get(); }
	void CreateByTool(Ptr<CTexture> _pAtlas, Vec2 _vBackgroundSizePixel, const vector<tAnim2DFrame>& VecFrame, int _iFrameCount);
	Vec2 GetBackgroundSize() { return m_vBackgroundSize; }

private:
	void SetFrame(tAnim2DFrame _Anim) { m_vecFrm.push_back(_Anim); }
	void SetTex(Ptr<CTexture> _ptr) { m_pAtlasTex = _ptr; }
	void SetBackgroundSize(Vec2 _v) { m_vBackgroundSize = _v; }

public:
	virtual void SaveToScene(FILE* _pFile) override;
	virtual void LoadFromScene(FILE* _pFile) override;
	CLONE(CAnimation2D)

public:
	CAnimation2D();
	~CAnimation2D();

	friend class CAnimator2D;
	friend class AnimatorUI;
	friend class AnimationUI;
};