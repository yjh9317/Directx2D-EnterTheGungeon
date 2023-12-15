#pragma once

#include "Ptr.h"

#include "CPathMgr.h"

#include "CPrefab.h"
#include "CMaterial.h"
#include "CMesh.h"
#include "CTexture.h"
#include "CGraphicsShader.h"
#include "CComputeShader.h"
#include "CSceneFile.h"
#include "CSound.h"
//#include "CMeshData.h"



class CResMgr
	: public CSingleton<CResMgr>
{
	SINGLE(CResMgr);
private:
	map<wstring, CRes*>		m_Res[(UINT)RES_TYPE::END];

public:
	void init();
	void update();
	void SaveChangedRes();

private:
	void InitSound();
	void CreateEngineMesh();
	void CreateEngineTexture();
	void CreateEngineShader();
	void CreateEngineComputeShader();
	void CreateEngineMaterial();
	void MakeInputLayoutInfo();

public:
	template<typename type>
	RES_TYPE GetResType();

	template<typename type>
	Ptr<type> Load(const wstring& _strKey, const wstring& _strRelativePath, bool _bEngineRes = false);

	template<typename type>
	Ptr<type> FindRes(const wstring& _strKey);
		

	const map<wstring, CRes*>& GetResList(RES_TYPE _eType) { return m_Res[(UINT)_eType]; }

	template<typename type>
	void AddRes(const wstring& _strKey, type* _pRes, bool _bEngineRes = false);
		
	// _flag : D3D11_BIND_FLAG
	Ptr<CTexture> CreateTexture(const wstring& _strKey, UINT _iWidth, UINT _iHeight
		, DXGI_FORMAT _format, UINT _flag, bool _bEngineRes = false);

	Ptr<CTexture> CreateTexture(const wstring& _strKey, ComPtr<ID3D11Texture2D> _pTex2D, bool _bEngineRes = false);

private:
	void DeleteRes(const wstring& _strKey);


	friend class CEventMgr;
};

template<typename type>
inline RES_TYPE CResMgr::GetResType()
{
	const type_info& info = typeid(type);

	if (info.hash_code() == typeid(CPrefab).hash_code())
		return RES_TYPE::PREFAB;
	else if (info.hash_code() == typeid(CMesh).hash_code())
		return RES_TYPE::MESH;
	else if (info.hash_code() == typeid(CSound).hash_code())
		return RES_TYPE::SOUND;
	else if (info.hash_code() == typeid(CGraphicsShader).hash_code())
		return RES_TYPE::GRAPHICS_SHADER;
	else if (info.hash_code() == typeid(CComputeShader).hash_code())
		return RES_TYPE::COMPUTE_SHADER;
	else if (info.hash_code() == typeid(CMaterial).hash_code())
		return RES_TYPE::MATERIAL;
	else if (info.hash_code() == typeid(CTexture).hash_code())
		return RES_TYPE::TEXTURE;
	else if (info.hash_code() == typeid(CSceneFile).hash_code())
		return RES_TYPE::SCENEFILE;
	

	return RES_TYPE::END;
}

template<typename type>
Ptr<type> CResMgr::Load(const wstring& _strKey, const wstring& _strRelativePath, bool _bEngineRes)
{
	RES_TYPE eType = GetResType<type>();

	CRes* pRes = FindRes<type>(_strKey).Get();
	if (nullptr != pRes)
		return Ptr<type>((type*)pRes);

	wstring strContentPath = CPathMgr::GetInst()->GetContentPath();
	wstring strFilePath = strContentPath + _strRelativePath;

	pRes = new type;
	if (FAILED(pRes->Load(strFilePath)))
	{
		MessageBox(nullptr, L"���ҽ� �ε� ����", L"���ҽ� �ε� ����", MB_OK);
		return nullptr;
	}

	pRes->SetKey(_strKey);
	pRes->SetRelativePath(_strRelativePath);
	pRes->m_bEngineRes = _bEngineRes;

	m_Res[(UINT)eType].insert(make_pair(_strKey, pRes));

	return Ptr<type>((type*)pRes);
}

template<typename type>
Ptr<type> CResMgr::FindRes(const wstring& _strKey)
{
	RES_TYPE eType = GetResType<type>();

	map<wstring, CRes*>::iterator iter = m_Res[(UINT)eType].find(_strKey);

	if (iter == m_Res[(UINT)eType].end())
		return nullptr;

	return (type*)iter->second;
}

template<typename type>
void CResMgr::AddRes(const wstring& _strKey, type* _pRes, bool _bEngineRes)
{
	RES_TYPE eType = GetResType<type>();

	Ptr<type> pRes = FindRes<type>(_strKey);

	assert(nullptr == pRes);

	_pRes->SetKey(_strKey);
	_pRes->SetRelativePath(_strKey);
	_pRes->m_bEngineRes = _bEngineRes;

	m_Res[(UINT)eType].insert(make_pair(_strKey, _pRes));

	if (_bEngineRes)
		return;

	wstring strContent = CPathMgr::GetInst()->GetContentPath();
	if (FAILED(_pRes->Load(strContent + _pRes->GetRelativePath())))
	{
		_pRes->Save(strContent + _pRes->GetKey());
	}
}