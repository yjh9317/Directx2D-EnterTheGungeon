#pragma once
#include "CComponent.h"

#include "CMesh.h"
#include "CMaterial.h"


enum class COLLIDER2D_TYPE
{
    BOX,
    CIRCLE,
};

class CCollider2D :
    public CComponent
{
private:
    Matrix              m_matColWorld;

    COLLIDER2D_TYPE     m_eColliderType;

    Vec2                m_vOffsetPos;       // ������Ʈ�� ������ �浹ü�� ��� �Ÿ�
    Vec2                m_vOffsetScale;     // ������Ʈ ũ��� ���� �浹ü�� ����

    Ptr<CMesh>          m_pMesh;            // �浹ü ���
    Ptr<CMaterial>      m_pMtrl;            // �浹ü ������ ����

    int                 m_iCollisionCount;  // �浹 Ƚ��
    int                 m_ColShow;
    


public:
    void SetCollider2DType(COLLIDER2D_TYPE _type);

    Vec2 GetOffsetPos() { return m_vOffsetPos; }
    void SetOffsetPos(Vec2 _vOffsetPos) { m_vOffsetPos = _vOffsetPos; }
    void SetOffsetPos(float _x, float _y) { m_vOffsetPos = Vec2(_x, _y); }

    Vec2 GetOffsetScale() { return m_vOffsetScale; }
    void SetOffsetScale(Vec2 _vOffsetScale);
    void SetOffsetScale(float _x, float _y);

    COLLIDER2D_TYPE GetCollider2DType() { return m_eColliderType; }
    Vec3 GetWorldPos() { return m_matColWorld.Translation(); }
    Vec3 GetWorldScale() { return Vec3(m_vOffsetScale); }

    Matrix GetWorldMat() { return m_matColWorld; }

    void SetColLook() {  m_ColShow = 1; }
    void SetColLose() { m_ColShow = 0; }
    int  GetColShow() { return m_ColShow; }


public:
    virtual void finalupdate() override;
    virtual void UpdateData() override;
    void render();

public:

    // ���� �����ӿ��� �浹 X, �̹� �����ӿ� �浹 ������ ��
    void OnCollisionEnter(CCollider2D* _Other);

    // ���� �����ӿ��� �浹 ��, ���ݵ� �浹 ���� ��
    void OnCollision(CCollider2D* _Other);

    // ���� �����ӿ� �浹 ��, ������ �浹 X �϶�
    void OnCollisionExit(CCollider2D* _Other);

public:
    virtual void SaveToScene(FILE* _pFile) override;
    virtual void LoadFromScene(FILE* _pFile) override;
    CLONE(CCollider2D)
public:
    CCollider2D();
    CCollider2D(const CCollider2D& _Origin);
    ~CCollider2D();
};

