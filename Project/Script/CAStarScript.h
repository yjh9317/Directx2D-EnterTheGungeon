#pragma once
#include <Engine/Cscript.h>



typedef struct _tagNode
{
	_tagNode*   pPrevNode;   // ���� ���
	float		fFromParent; // ���� ��忡�� ���� �������� �Ÿ�
	float		fToDest;	 // ���� ��忡�� ������ ������ �Ÿ�
	float		fFinal;		 // ���� �� ���� ��ģ ��(�켱���� ����)

	int			iIdxX;
	int			iIdxY;
	bool		bMove;   // �̵� ���� �Ұ���
	bool		bOpen;   // OpenList �� ������
	bool		bClosed; // ClostList �� ����ִ���

	bool		bPath;
}tNode;

// Open List
class Compare
{
public:
	bool operator() (tNode* _pLeft, tNode* _pRight)
	{
		return _pLeft->fFinal > _pRight->fFinal;
	}
};


class CAStarScript :
    public CScript
{
private:
    priority_queue<tNode*, vector<tNode*>, Compare>	OpenList;
	list<tNode*> pList;

	tNode   arrNode[60][60];

	float	m_ftime;

	int		m_iDestRow;
	int		m_iDestCol;

	int		m_iOriginRow;
	int		m_iOriginCol;

	CGameObject* pPlayer;
	Vec2 vDir;

public:
	virtual void start() override;
    virtual void update() override;


	void SetTileIndex(CGameObject* Tilemap, CGameObject* Obj, int& Row, int& Col);

public:
	Vec2 GetDir() const { return vDir; }
	virtual void OnCollisionEnter(CGameObject* _OtherObj) override;
	virtual void OnCollision(CGameObject* _OtherObj) override;
    CLONE(CAStarScript)    

private:
	void Rebuild(priority_queue<tNode*, vector<tNode*>, Compare>& _queue);
	void FindPath(CGameObject* _OtherObj);
	void CalculateCost(tNode* _pCurNode, tNode* _pOrigin);
	void AddOpenList(int _iXIdx, int _iYIdx, tNode* _pOrigin);
	void WallCheck(CGameObject* _OtherObj);


	void CheckAndStart(float AstarTime);
	CGameObject* FindTileMapObject();
	bool IsTraceState();
public:
    CAStarScript();
    ~CAStarScript();
};

