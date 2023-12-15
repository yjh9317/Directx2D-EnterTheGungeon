#include "pch.h"
#include "CAStarScript.h"

#include "CMovementScript.h"

#include <Engine/CTileMap.h>
#include <Engine/CCollider2D.h>
#include "CMonsterScript.h"

int g_iXCount = 0;
int g_iYCount = 0;

CAStarScript::CAStarScript()
	:CScript((int)SCRIPT_TYPE::ASTARSCRIPT)
	, m_iDestRow(0)
	, m_iDestCol(0)
{
}

CAStarScript::~CAStarScript()
{
}


void CAStarScript::start()
{
	pPlayer = CSceneMgr::GetInst()->GetCurScene()->GetPlayer();
}

void CAStarScript::update()
{
	CheckAndStart(0.1f);
}

void CAStarScript::CheckAndStart(float AstarTime)
{
	if (m_ftime > AstarTime)
	{
		m_ftime = 0.f;
		// Ÿ�ϸ� ã��
		CGameObject* OnTileMap = FindTileMapObject();
		if (OnTileMap == nullptr) return;

		// ������ Ÿ�� �ε��� ã��
		int Row, Col;
		SetTileIndex(OnTileMap, GetOwner(),Row,Col);
		m_iOriginRow = Row; m_iOriginCol = Col;

		// �÷��̾��� Ÿ�� �ε��� ã��
		SetTileIndex(OnTileMap, pPlayer, Row,Col);
		m_iDestRow = Row; m_iDestCol = Col;

		// ��ã��
		FindPath(OnTileMap);
	}
	else { m_ftime += DT; }
}


void CAStarScript::Rebuild(priority_queue<tNode*, vector<tNode*>, Compare>& _queue)
{
	priority_queue<tNode*, vector<tNode*>, Compare> tempQueue;

	while (!_queue.empty())
	{
		tempQueue.push(_queue.top());
		_queue.pop();
	}

	_queue.swap(tempQueue);
}

void CAStarScript::FindPath(CGameObject* _OtherObj)
{	
	
	for (int i = 0; i < g_iYCount; ++i)
	{
		for (int j = 0; j < g_iXCount; ++j)
		{
			arrNode[i][j].bPath = false;
			arrNode[i][j].bOpen = false;
			arrNode[i][j].bClosed = false;
			arrNode[i][j].bMove = true;
		}
	}

	WallCheck(_OtherObj);

	arrNode[m_iDestCol][m_iDestRow].bMove = true;
	int iCurX = m_iOriginRow;
	int iCurY = m_iOriginCol;

	tNode* pCurNode = &arrNode[iCurY][iCurX];
	pCurNode->bClosed = true;

	while (true)
	{
		// ���� �������� 4������ ��带 OpenList �� �ִ´�.
		
		// LEFT		
		AddOpenList(pCurNode->iIdxX - 1
			, pCurNode->iIdxY, pCurNode);
		// UP
		AddOpenList(pCurNode->iIdxX
			, pCurNode->iIdxY - 1, pCurNode);

		// RIGHT		
		AddOpenList(pCurNode->iIdxX + 1
			, pCurNode->iIdxY, pCurNode);

		// DOWN		
		AddOpenList(pCurNode->iIdxX
			, pCurNode->iIdxY + 1, pCurNode);

		// LEFT Up
		AddOpenList(pCurNode->iIdxX - 1
			, pCurNode->iIdxY - 1, pCurNode);

		// Right Up
		AddOpenList(pCurNode->iIdxX + 1
			, pCurNode->iIdxY - 1, pCurNode);

		// Right Down	
		AddOpenList(pCurNode->iIdxX + 1
			, pCurNode->iIdxY + 1, pCurNode);

		// Left Down	
		AddOpenList(pCurNode->iIdxX - 1
			, pCurNode->iIdxY + 1, pCurNode);


		// 2. Open List ���� ���� ȿ���� ���� ��带 ������.
		//  - �ش� ���� ClostList �� �ִ´�.
		pCurNode = OpenList.top();
		pCurNode->bClosed = true;

		OpenList.pop();

		// �������� ������ ��� Astar ����
		if (pCurNode->iIdxX == m_iDestRow && pCurNode->iIdxY == m_iDestCol)
		{
			tNode* TempNode = pCurNode;
			while (TempNode->iIdxX != iCurX && TempNode->iIdxY != iCurY)
			{
				pList.push_front(TempNode);
				TempNode = TempNode->pPrevNode;
				
			}
			break;
		}
	}

	while (!OpenList.empty())
		OpenList.pop();

	if (pList.size() > 1)
	{
		tNode* CurNode = pList.front(); pList.pop_front();
		tNode* NextNode = pList.front();
		vDir = Vec2(NextNode->iIdxX - CurNode->iIdxX, -(NextNode->iIdxY - CurNode->iIdxY)); vDir.Normalize();
		pList.clear();
	}
}


void CAStarScript::CalculateCost(tNode* _pCurNode, tNode* _pOrigin)
{
	_pCurNode->pPrevNode = _pOrigin;
	_pCurNode->fFromParent = 10.f + _pOrigin->fFromParent;
	_pCurNode->fToDest = abs(m_iDestRow - _pCurNode->iIdxX) * 10.f + abs(m_iDestCol - _pCurNode->iIdxY) * 10;
	_pCurNode->fFinal = _pCurNode->fFromParent + _pCurNode->fToDest;
}

void CAStarScript::AddOpenList(int _iXIdx, int _iYIdx, tNode* _pOrigin)
{
		// ���� �������� �� �� �ִ� ���� OpenList �� �ִ´�.
		// ��� ������ ��� ���
		if (_iXIdx < 0 || _iXIdx >= g_iXCount || _iYIdx < 0 || _iYIdx >= g_iYCount
			|| !arrNode[_iYIdx][_iXIdx].bMove)
			return;
	
		// �ش� ���� Clost List �� �ִ� ���, Open List �� ���� �ʴ´�.
		if (arrNode[_iYIdx][_iXIdx].bClosed)
			return;
	
		// Open List �� ����� ����ؼ� �ִ´�.
		if (false == arrNode[_iYIdx][_iXIdx].bOpen)
		{
			CalculateCost(&arrNode[_iYIdx][_iXIdx], _pOrigin);
			// Open List �� �ִ´�.
			arrNode[_iYIdx][_iXIdx].bOpen = true;
			OpenList.push(&arrNode[_iYIdx][_iXIdx]);
		}
		else // �̹� OpenList �� �ִ� ���,
		{
			//����� ����ؼ� �� ȿ���� ���� ������ ��ü�Ѵ�.
			tNode node = arrNode[_iYIdx][_iXIdx];
			CalculateCost(&node, _pOrigin);
	
			if (arrNode[_iYIdx][_iXIdx].fFinal > node.fFinal)
			{
				arrNode[_iYIdx][_iXIdx] = node;
	
				// ���¸���Ʈ(�켱����ť) �缳��
				Rebuild(OpenList);
			}
		}
}

void CAStarScript::WallCheck(CGameObject* _OtherObj)
{
	if (nullptr == _OtherObj->TileMap())
		return;

	int XCount = _OtherObj->TileMap()->GetTileCountX();
	int YCount = _OtherObj->TileMap()->GetTileCountY();

	vector<tTileData> vTileData = _OtherObj->TileMap()->GetTileData();

	for (int i = 0; i < YCount; ++i)
	{
		for (int j = 0; j < XCount; ++j)
		{
			arrNode[i][j].iIdxX = j;
			arrNode[i][j].iIdxY = i;

			int Idx = i * (XCount - 1) + j;

			if (vTileData[Idx].TilePass == 1)
				arrNode[i][j].bMove = false;
		}
	}
}

CGameObject* CAStarScript::FindTileMapObject()
{
	CScene* Scene = CSceneMgr::GetInst()->GetCurScene();
	const vector<CGameObject*>& RootObjects = Scene->GetLayer(1)->GetRootObjects();

	for (CGameObject* Object : RootObjects)
	{
		Vec3 MonsterPos = Transform()->GetWorldPos();
		Vec3 TilePos = Object->Transform()->GetWorldPos();
		Vec3 TileScale = Object->Transform()->GetWorldScale();

		// Ÿ�ϸ� ������Ʈ�� ������ �̹Ƿ� ���� ���� �ִ� Ÿ�ϸ� ���ϱ�
		if (MonsterPos.x >= TilePos.x - TileScale.x &&
			MonsterPos.x <= TilePos.x + TileScale.x &&
			MonsterPos.y >= TilePos.y - TileScale.y &&
			MonsterPos.y >= TilePos.y - TileScale.y)
		{
			CTileMap* TileMap = Object->TileMap();
			g_iXCount = TileMap->GetTileCountX();
			g_iYCount =	TileMap->GetTileCountY();
			return Object;
		}
	}
	return nullptr;
}

bool CAStarScript::IsTraceState()
{
	CMonsterScript* MonsterScript = GetOwner()->GetScript<CMonsterScript>();
	return MonsterScript->GetState() == MON_STATE::TRACE;
}



void CAStarScript::SetTileIndex(CGameObject* Tilemap , CGameObject* Obj, int& Row, int& Col)
{
	CTileMap* pTileMap = Tilemap->TileMap();
	Vec3 TileMapPos = Tilemap->Transform()->GetWorldPos();
	Vec3 TileMapScale = Tilemap->Transform()->GetWorldScale();

	Vec3 vPos = Obj->Transform()->GetWorldPos();
	Vec3 vScale = Obj->Transform()->GetWorldScale();
	Vec2 vColOffset = Obj->Collider2D()->GetOffsetPos();

	// ������Ʈ�� ���� ��ġ�� Collider ����
	Vec2 vObjFinalPos = Vec2(vPos.x + vColOffset.x, vPos.y + vColOffset.y);

	// Ÿ�ϸ� �»��
	Vec2 TileMapLT = Vec2(TileMapPos.x - (TileMapScale.x / 2.f), TileMapPos.y + (TileMapScale.y / 2.f));
	// 
	Vec2 ObjOnTile =  Vec2(TileMapLT.x - vObjFinalPos.x, TileMapLT.y - vObjFinalPos.y);
	//ObjOnTile.y -= vScale.y / 2.f;
	int TileXSize = (int)TileMapScale.x / pTileMap->GetTileCountX();
	int TileYSize = (int)TileMapScale.y / pTileMap->GetTileCountY();

	Row = abs(ObjOnTile.x) / TileXSize;
	Col = abs(ObjOnTile.y) / TileYSize;

	// m_iTileIdx = m_iCol * (pTileMap->GetTileCountX()) + m_iRow;
}




void CAStarScript::OnCollisionEnter(CGameObject* _OtherObj)
{

}

void CAStarScript::OnCollision(CGameObject* _OtherObj)
{


}
