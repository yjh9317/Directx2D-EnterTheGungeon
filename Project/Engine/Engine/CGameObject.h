
#pragma once
#include "CEntity.h"

#define GET_COMPONENT(type, TYPE)  class C##type* type() { return (C##type*)m_arrCom[(UINT)COMPONENT_TYPE::TYPE]; }

class CComponent;
class CTransform;
class CCollider2D;
class CAnimator2D;
class CCamera;
class CLight2D;
//class CLight3D;
class CMeshRender;
class CTimeMap;
class CParticleSystem;
class CScript;
class CRenderComponent;

class CGameObject :
    public CEntity
{
private:
    vector<CGameObject*>    m_vecChild;
    vector<CScript*>        m_vecScript;

    CComponent*             m_arrCom[(UINT)COMPONENT_TYPE::END];
    CRenderComponent*       m_pRenderComponent;    

    CGameObject*            m_pParent;
    int                     m_iLayerIdx; // ���� ������Ʈ �Ҽ� ���̾� �ε���   
    bool                    m_bDead;

    bool                    m_bActive;
    bool                    m_bDynamicShadow;   // ���� �׸��� ����
    bool                    m_bFrustumCulling;  // ����ü �ø� ��� ����


public:
    void start();
    void update();
    void lateupdate();
    void finalupdate();
    void render();

private:
    void active();
    void deactive();

public:
    CGameObject* GetParent() { return m_pParent; }
    const vector<CGameObject*>& GetChild() { return m_vecChild; }

    // Deregister ==> ��� ���(���->�̵��)
    // Unregister ==> ��� �ȵ�(��� x == ���->�̵��, ���ʿ� ��ϵ��� ����)
    void Deregister();
    void DisconnectBetweenParent();
    void Activate();
    void Deactivate();

    bool IsDead() { return m_bDead; }
    bool IsActive() { return m_bActive; }
    bool IsAncestor(CGameObject* _pObj);

public:
    void AddChild(CGameObject* _pChild);
    void AddComponent(CComponent* _component);
    CComponent* GetComponent(COMPONENT_TYPE _eType){ return m_arrCom[(UINT)_eType];  }
    CRenderComponent* GetRenderComponent() { return m_pRenderComponent; }

    void Destroy();

    GET_COMPONENT(Transform, TRANSFORM)
    GET_COMPONENT(MeshRender, MESHRENDER)
    GET_COMPONENT(Camera, CAMERA)
    GET_COMPONENT(Collider2D, COLLIDER2D)
    GET_COMPONENT(Animator2D, ANIMATOR2D)
    GET_COMPONENT(TileMap, TILEMAP)
    GET_COMPONENT(ParticleSystem, PARTICLESYSTEM)
    GET_COMPONENT(Light2D, LIGHT2D)

    const vector<CScript*>& GetScripts() { return m_vecScript; }
    CScript* GetScript(UINT _iIdx);
    CScript* GetScriptByName(const wstring& _strName);

    int GetLayerIndex() { return m_iLayerIdx; }

    template<typename T>
    T* GetScript();

public: 
    virtual void SaveToScene(FILE* _pFile) override;
    virtual void LoadFromScene(FILE* _pFile) override;
    CLONE(CGameObject)

public:
    CGameObject();
    CGameObject(const CGameObject& _origin);
    ~CGameObject();

    friend class CEventMgr;
    friend class CScene;
    friend class CLayer;
};

template<typename T>
inline T* CGameObject::GetScript()
{
    for (size_t i = 0; i < m_vecScript.size(); ++i)
    {
        T* pScript = dynamic_cast<T*>(m_vecScript[i]);
        if (nullptr != pScript)
            return pScript;
    }

    return nullptr;
}
