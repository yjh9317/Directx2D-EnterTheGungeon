#pragma once
#include "CEntity.h"

#include "Ptr.h"

class CRes : 
    public CEntity
{
private:
    const RES_TYPE      m_eResType;
    wstring             m_strKey;
    wstring             m_strRelativePath;
    UINT                m_iRefCount;    
    bool                m_bEngineRes;   // ���� ���ο��� ����ϴ� �ڿ�
    bool                m_bChanged;     // ���ҽ� ������ ����üũ

protected:
    void SetKey(const wstring& _strKey) { m_strKey = _strKey; }
    void SetRelativePath(const wstring& _strRelativePath){m_strRelativePath = _strRelativePath;}
    bool CheckFail(HRESULT _hr);
    void Changed() { m_bChanged = true; }

public:
    const wstring& GetKey() { return m_strKey; }
    const wstring& GetRelativePath() { return m_strRelativePath; }
    UINT GetRefCount() { return m_iRefCount; }
    RES_TYPE GetResType() { return m_eResType; }
    bool IsEngineRes() { return m_bEngineRes; }
    bool IsChanged() { return m_bChanged; }

protected:   
    virtual int Save(const wstring& _strFilePath) { m_bChanged = false; return S_OK; }
    virtual int Load(const wstring& _strFilePath) = 0;    
    CLONE_DISABLE(CRes)

private:
    void AddRef()   {   ++m_iRefCount;  }
    void SubRef()   {   --m_iRefCount;  }

public:
    CRes(RES_TYPE _eType);
    virtual ~CRes();

    friend class CResMgr;

    template<typename T>
    friend class Ptr;
};

