#pragma once
#include "CRenderComponent.h"

#include "CStructuredBuffer.h"
#include "CParticleUpdateShader.h"

class CParticleSystem :
    public CRenderComponent
{
private:
    Ptr<CParticleUpdateShader>      m_CS;           // ��ƼŬ ������Ʈ ���̴�

    CStructuredBuffer*              m_ParticleBuffer;   // ��ƼŬ ����ȭ ����
    CStructuredBuffer*              m_DataBuffer;       // �߰� ������ ����(��ƼŬ AliveCount)
    UINT                            m_iMaxCount;        // ��ƼŬ �ִ� ����

    int                             m_bPosInherit;      // ������Ʈ ��ǥ�� ����
    int                             m_iAliveCount;      // �̹� �����ӿ� ����(Ȱ��ȭ) �� ��ƼŬ ��
    
    float                           m_fMinLifeTime;     // ��ƼŬ�� �ּ� ���� �ð�
    float                           m_fMaxLifeTime;     // ��ƼŬ�� �ִ� ���� �ð�

    float                           m_fStartSpeed;      // ��ƼŬ�� �ּ� �ӷ�
    float                           m_fEndSpeed;        // ��ƼŬ�� �ִ� �ӷ�

    Vec4                            m_vStartColor;      // ��ƼŬ �ʱ� ����
    Vec4                            m_vEndColor;        // ��ƼŬ ���� ����

    Vec3                            m_vStartScale;      // ��ƼŬ �ʱ� ũ��
    Vec3                            m_vEndScale;        // ��ƼŬ ���� ũ��


    float                           m_fParticleCreateDistance;  // ��ƼŬ ���� ����
    float                           m_fParticleCreateTerm;      // ��ƼŬ ���� ����
    float                           m_fAccTime;
    
    float                           m_fTime;


public:
    void SetMaxParticleCount(UINT _iMax);


public:
    virtual void finalupdate() override;
    virtual void render()   override;


    UINT GetMaxCount() { return m_iMaxCount; }
    void SetMaxCount(UINT _uint) { m_iMaxCount = _uint; }

    int GetAliveCount() { return m_iAliveCount; }
    void SetAliveCount(int _int) { m_iAliveCount = _int; }

    const float& GetMinLife() { return m_fMinLifeTime; }
    const float& GetMaxLife() { return m_fMaxLifeTime; }
    void SetMinLife(float _f) { m_fMinLifeTime = _f; }
    void SetMaxLife(float _f) { m_fMaxLifeTime = _f; }

    const Vec4& GetStartColor() { return m_vStartColor; }
    const Vec4& GetEndColor() { return m_vEndColor; }
    void SetStartColor(Vec4 _v) { m_vStartColor = _v; }
    void SetEndColor(Vec4 _v) { m_vEndColor = _v; }

    const Vec3& GetStartScale() { return m_vStartScale; }
    const Vec3& GetEndScale() { return m_vEndScale; }
    void SetStartScale(Vec3 _v) { m_vStartScale = _v; }
    void SetEndScale(Vec3 _v) { m_vEndScale = _v; }

    const float& GetCreateDis() { return m_fParticleCreateDistance; }
    const float& GetCreateTerm() { return m_fParticleCreateTerm; }
    void SetCreateDis(float _f) { m_fParticleCreateDistance = _f; }
    void SetCreateTerm(float _f) { m_fParticleCreateTerm = _f; }

public:
    virtual void SaveToScene(FILE* _pFile) override;
    virtual void LoadFromScene(FILE* _pFile) override;
    CLONE(CParticleSystem);
public:
    CParticleSystem();
    CParticleSystem(const CParticleSystem& _origin);
    ~CParticleSystem();
};

