/****************************************************************************
Author : CaiLi @Bojoy 2017
FileName: 
Description: 
****************************************************************************/

#pragma once

#include <list>
#include <vector>
#include "cocos2d.h"

namespace BJMGame
{
//------------------------------------- GridNode -------------------------------------//
static const short	MAX_MOVE_DIRECTIONS = 8;
static const short	MOVE_DIRECTION_OFFSETS[MAX_MOVE_DIRECTIONS][2] = 
{
	{0,-1},{1,-1},{1,0},{1,1},{0,1},{-1,1},{-1,0},{-1,-1}
};

struct GridNode
{
	GridNode()
		:x(0)
		,y(0)
		,f(0)
		,g(0)
		,pParent(NULL)
		,bOpened(false)
		,bClosed(false)
	{}

	GridNode(int x,int y)
		:x(x)
		,y(y)
		,f(0)
		,g(0)
		,pParent(NULL)
		,bOpened(false)
		,bClosed(false)
	{}

	bool operator < (const GridNode &node) const
	{
		return f > node.f;
	}

	void Reset()
	{
		f = 0;
		g = 0;
		pParent = NULL;
		bOpened = false;
		bClosed = false;
	}

	short x,y;
	float f,g;
	bool bOpened, bClosed;	//标记是否在openList列表，或closeList列表
	GridNode* pParent;
};

typedef  std::vector<GridNode*> GridNodeVec;

class BJMPathFinder
{
private:
	int	m_GridWidth;		//uniform-cost grid maps width
	int m_GridHeight;		//uniform-cost grid maps height
	GridNode*** m_grid;

public:
	std::list<short> m_shortestPath;
	
public:
	BJMPathFinder(void);
	~BJMPathFinder(void);

	bool Init(int width, int height);

	bool WalkAble(int x, int y);
	int	GetDir(int deltaX, int deltaY);
	GridNode* GetNodeAt(int x, int y);
	virtual bool PathFinding(const cocos2d::Point & from, const cocos2d::Point & to)=0;

	virtual GridNodeVec FindNeighbours(GridNode* node);
	float GetH(GridNode* nodeA, GridNode* nodeB);

	void PrintPath(GridNode* endNode);
	bool	HasNextStep();
	short	GetNextStep();
	short	WatchNextStep();
	int		GetLeftStepNum();
	std::string	GetLeftStepStr();
	void	Clear();


	//GetHValue
	float MANHATTAN(GridNode* nodeA, GridNode* nodeB);
	float EUCLIDIAN(GridNode* nodeA, GridNode* nodeB);
	float DIAGONAL(GridNode* nodeA, GridNode* nodeB);
	float CARDINTCARD(GridNode* nodeA, GridNode* nodeB);

};

}
