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

    //����Ÿ�� �ؽ��� ������
    ImVec2 ImageSize;
    ImVec2 Pos;
    ImVec2 WindowPos;

    // RenderTex ���� Tile�� Grid üũ����
    int bGrid;

    // �ڽĿ�����Ʈ Collider �߰��ϱ� ���� ������
    string Collider_Name;           // �ڽ� ������Ʈ �ݶ��̴� �̸�
    int bCollider;                  // ���� ��ư üũ��
    int bIdxCheck;                  // üũ�� �����ִٸ� �ε��� ����
    

    // ���콺 ��Ŭ������ �ε����� ����Ͽ� ũ�⸦ �����Ͽ� �ݶ��̴� ��ġ���
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


