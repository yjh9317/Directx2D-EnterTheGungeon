#pragma once
#include <Engine/CScript.h>

class CCameraMoveScript :
    public CScript
{
private:
    CGameObject*    m_Filter;
    float           m_fCamSpeed;

    bool            m_PlayerTrace;

    float           m_DistortionTime;
    float           m_DistortionDist;
    bool            m_bDistortion;

    Vec2            vWinMouse;

    bool            m_Event;
    float           m_fTime;
    Vec3            m_BossPos;

public:
    void SetFilter(CGameObject* _pFilterObject){m_Filter = _pFilterObject;}
    void SetbEvent(bool _b) { m_Event = _b; }
    void SetBossPos(Vec3 _v) { m_BossPos = _v; }

public:
    virtual void SaveToScene(FILE* _pFile) override;
    virtual void LoadFromScene(FILE* _pFile) override;

public:
    virtual void start() override;
    virtual void update() override;
    virtual void lateupdate() override;
    CLONE(CCameraMoveScript)

private:
    void KeyInput(Vec3& ObjectPos);
    void PersCamera(Vec3& ObjectPos, float Speed);
    void OrthoCamera(Vec3& vPos);
    void RegisterPostProcess();
    void ExecuteDistortion();
    void TurnOnFilter();
    void CreateDistortion() const;
    void RemoveAllMonsterBullet() const;
    bool CheckBulletCount() const;
    void CheckPlayerTrace(Vec3& vPos);
    void RenewFilterValue();
    void SetBulletPos(class CPlayerStateScript* pScript);
    void SetHeartPos(class CPlayerStateScript* pScript);
public:
    CCameraMoveScript();
    ~CCameraMoveScript();
};

