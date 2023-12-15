#pragma once

class CGameObject;
class TileMapUI;

#include "UI.h"
class TileMapTex :
    public UI
{
    CGameObject* m_pCurObj;
    TileMapUI* m_pTileMapUI;

    int         m_iCol;
    int         m_iRow;

    //렌더타겟 텍스쳐 사이즈
    ImVec2 ImageSize;
    ImVec2 Pos;
    ImVec2 WindowPos;

    // RenderTex 에서 Tile의 Grid 체크변수
    int bGrid;

    // 자식오브젝트 Collider 추가하기 위한 변수들
    string Collider_Name;           // 자식 오브젝트 콜라이더 이름
    int bCollider;                  // 라디오 버튼 체크용
    int bIdxCheck;                  // 체크가 켜져있다면 인덱스 저장
    

    // 마우스 좌클릭으로 인덱스를 기억하여 크기를 조절하여 콜라이더 위치계산
    int _col;
    int _row;

  

public:
    virtual void update() override;
    virtual void render_update() override;


    void SetObj(CGameObject* Obj) { m_pCurObj = Obj; }
    void SetTileMapUI(TileMapUI* _UI) { m_pTileMapUI = _UI; }

    void RTTex();
    void AddCollider();
    void CheckColliderIdx();
public:
    TileMapTex();
    ~TileMapTex();
};


