#include "pch.h"
#include "CAnimator2D.h"

#include "CAnimation2D.h"
#include "CDevice.h"
#include "CConstBuffer.h"



CAnimator2D::CAnimator2D()
	: CComponent(COMPONENT_TYPE::ANIMATOR2D)
	, m_pCurAnim(nullptr)
	, m_bRepeat(false)
{
}

CAnimator2D::CAnimator2D(const CAnimator2D& _origin)
	: CComponent(_origin)
	, m_pCurAnim(nullptr)
	, m_bRepeat(_origin.m_bRepeat)
{
	for (auto& pair : _origin.m_mapAnim)
	{
		CAnimation2D* pCloneAnim = pair.second->Clone();
		m_mapAnim.insert(make_pair(pair.first, pCloneAnim));
		pCloneAnim->m_pOwner = this;
	}

	if (nullptr != _origin.m_pCurAnim)
	{
		m_pCurAnim = FindAnim(_origin.m_pCurAnim->GetName());
	}
}

CAnimator2D::~CAnimator2D()
{
	Safe_Del_Map(m_mapAnim);
}

void CAnimator2D::finalupdate()
{
	if (nullptr == m_pCurAnim)
		return;
	
	m_pCurAnim->finalupdate();

	if (m_pCurAnim->IsFinish() && m_bRepeat)
	{
		m_pCurAnim->Reset();
	}
}

void CAnimator2D::UpdateData()
{
	if (nullptr == m_pCurAnim)
		return;

	m_pCurAnim->UpdateData();
}

void CAnimator2D::Clear()
{
	static CConstBuffer* pBuffer = CDevice::GetInst()->GetCB(CB_TYPE::ANIM2D);
	tAnim2D info = {};
	pBuffer->SetData(&info, sizeof(tAnim2D));
	pBuffer->UpdateData();
}

CAnimation2D* CAnimator2D::FindAnim(const wstring& _strName)
{
	map<wstring, CAnimation2D*>::iterator iter = m_mapAnim.find(_strName);

	if (iter == m_mapAnim.end())
	{
		return nullptr;
	}

	return iter->second;
}

void CAnimator2D::CreateAnim(const wstring& _strName, Ptr<CTexture> _pAtlas, Vec2 _vBackgroundSizePixel, Vec2 _vLeftTopPixel, Vec2 _vSlicePixel, Vec2 _vStepPixel, float _fDuration, int _iFrameCount)
{
	assert(!FindAnim(_strName));

	CAnimation2D* anim = new CAnimation2D;
	anim->SetName(_strName);
	anim->Create(_pAtlas, _vBackgroundSizePixel,  _vLeftTopPixel, _vSlicePixel, _vStepPixel, _fDuration, _iFrameCount);

	m_mapAnim.insert(make_pair(_strName, anim));
	anim->m_pOwner = this;
}

void CAnimator2D::Play(const wstring& _strName, bool _bRepeat)
{
	CAnimation2D* pAnim = FindAnim(_strName);
	assert(pAnim);

	m_bRepeat = _bRepeat;

	m_pCurAnim = pAnim;
}

void CAnimator2D::CreateAnimByTool(const wstring& _strName, Ptr<CTexture> _pAtlas, Vec2 _vBackgroundSizePixel, const vector<tAnim2DFrame>& VecFrame, int _iFrameCount)
{
	//	assert(!FindAnim(_strName));

	// 만약 같은 이름의 애니메이션이 있다면
	if (FindAnim(_strName))
	{
		delete(m_mapAnim[_strName]);
		m_mapAnim.erase(_strName);
	}

	CAnimation2D* anim = new CAnimation2D;
	anim->SetName(_strName);
	anim->CreateByTool(_pAtlas, _vBackgroundSizePixel, VecFrame, _iFrameCount);

	m_mapAnim.insert(make_pair(_strName, anim));
	anim->m_pOwner = this;
}

void CAnimator2D::SaveToScene(FILE* _pFile)
{
	CComponent::SaveToScene(_pFile);

	size_t iAnimCount = m_mapAnim.size();
	fwrite(&iAnimCount, sizeof(size_t), 1, _pFile);

	map<wstring, CAnimation2D*>::iterator iter =  m_mapAnim.begin();
	for (; iter != m_mapAnim.end(); ++iter)
	{
		iter->second->SaveToScene(_pFile);
	}

	wstring strCurAnimName;
	if (nullptr != m_pCurAnim)
		strCurAnimName = m_pCurAnim->GetName();

	SaveWStringToFile(strCurAnimName, _pFile);
	fwrite(&m_bRepeat, sizeof(bool), 1, _pFile);
}

void CAnimator2D::LoadFromScene(FILE* _pFile)
{
	CComponent::LoadFromScene(_pFile);

	size_t iAnimCount = 0;
	fread(&iAnimCount, sizeof(size_t), 1, _pFile);

	for(size_t i = 0; i < iAnimCount; ++i)
	{
		CAnimation2D* pAnim = new CAnimation2D;
		pAnim->LoadFromScene(_pFile);

		m_mapAnim.insert(make_pair(pAnim->GetName(), pAnim));
		pAnim->m_pOwner = this;
	}

	wstring strCurAnimName;
	LoadWStringFromFile(strCurAnimName, _pFile);
	m_pCurAnim = FindAnim(strCurAnimName);

	fread(&m_bRepeat, sizeof(bool), 1, _pFile);
}