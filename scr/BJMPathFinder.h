/****************************************************************************
Author : CaiLi @Bojoy 2017
FileName: 
Description: 
****************************************************************************/

#pragma once

#include <list>
#include <vector>
#include "cocos2d.h"
#include "BJMPath.h"

namespace BJMGame
{

#define _MAX(a, b) ((a)>(b) ? (a) : (b))
#define _MIN(a, b) ((a)<(b) ? (a) : (b))
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
	bool bOpened, bClosed;	//mark in the openList or the closedList
	GridNode* pParent;
};

typedef  std::vector<GridNode*> GridNodeVec;

//simple binary_heap functions
bool checkBinaryHeapValid(GridNodeVec& vec);
bool cmp(GridNode* nodeA, GridNode* nodeB);
void percolate_up(GridNodeVec& vec, int index);
void percolate_down(GridNodeVec& vec, int index);
void push(GridNodeVec& vec,GridNode* item);
GridNode* pop(GridNodeVec& vec);
void heapify(GridNodeVec& vec, GridNode* item);

class BJMPathFinder
{
private:
	int	m_GridWidth;		//uniform-cost grid maps width
	int m_GridHeight;		//uniform-cost grid maps height
	GridNode*** m_grid;
	
public:
	BJMPathFinder(void);
	~BJMPathFinder(void);

	bool Init(int width, int height);

	bool WalkAble(int x, int y);
	int	GetDir(int deltaX, int deltaY);
	GridNode* GetNodeAt(int x, int y);
	

	virtual void FindNeighbours(GridNodeVec& output, GridNode* node);
	float GetH(GridNode* nodeA, GridNode* nodeB);

	//reset each search node in openList and closedList to origin, it must be called when finding a new path;
	void	Reset();
	//remove every newed search node
	void	Clear();

	//path
	BJMPath* PrintPath(GridNode* endNode);
	virtual BJMPath* PathFinding(const cocos2d::Point & from, const cocos2d::Point & to)=0;

	// methods to calculate the Heuristic Value
	float MANHATTAN(GridNode* nodeA, GridNode* nodeB);
	float EUCLIDIAN(GridNode* nodeA, GridNode* nodeB);
	float DIAGONAL(GridNode* nodeA, GridNode* nodeB);
	float CARDINTCARD(GridNode* nodeA, GridNode* nodeB);

};

}
