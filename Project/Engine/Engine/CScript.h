#pragma once

#include "global.h"
#include "CComponent.h"

#include "CTransform.h"
#include "CMeshRender.h"

#include "CResMgr.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "CLayer.h"


enum class SCRIPTPARAM_TYPE
{
    INT,
    FLOAT,
    VEC2,
    VEC4,

    TEX,
    PREFAB,
};


struct tScriptParamInfo
{
    string              strParamName;
    SCRIPTPARAM_TYPE    eType;
    void*               pParam;
};


class CScript :
    public CComponent
{
private:
    const int                   m_iScriptID;    // ��ũ��Ʈ �����뵵(��ũ��Ʈ Ÿ��)
    vector<tScriptParamInfo>    m_vecParamInfo; // ������ ���� ����


public:
    int GetScriptType() { return m_iScriptID; }
    const vector<tScriptParamInfo>& GetScriptParam() { return m_vecParamInfo; }

protected:
    void AddScriptParam(string _strParamName, SCRIPTPARAM_TYPE _eType, void* _pData) { m_vecParamInfo.push_back(tScriptParamInfo{ _strParamName , _eType, _pData });}
    

public:
    virtual void start() {}
    virtual void update() {}
    virtual void lateupdate() {}
    virtual void finalupdate() final {}

    virtual void OnCollisionEnter(CGameObject* _OtherObject) {};
    virtual void OnCollision(CGameObject* _OtherObject) {};
    virtual void OnCollisionExit(CGameObject* _OtherObject) {};

    virtual CScript* Clone() = 0;
    
    virtual void SetState(MON_STATE _eState) {};
    virtual void SetState(PLAYER_TYPE _eState) {};

public:
    CScript(int _iScriptType);
    virtual ~CScript();
};


