/****************************************************************************
Author : Caili @Bojoy 2015
FileName: BJMAstarPathfinder.cpp
Description: 
****************************************************************************/

#include "BJMAstarPathfinder.h"
#include <queue>


namespace BJMGame
{

/////////BJMAstarPathfinder/////////////////////////////////////////
BJMAstarPathfinder::BJMAstarPathfinder()
{

}


BJMAstarPathfinder::~BJMAstarPathfinder()
{
}

BJMAstarPathfinder * BJMAstarPathfinder::Create(int width, int height)
{
	BJMAstarPathfinder* p = new BJMAstarPathfinder();
	if(!p->Init(width, height))
	{
		delete p;
		p = NULL;
	}
	return p;
}

void BJMAstarPathfinder::updateVertex(GridNodeVec& openList, GridNode* node, GridNode* neighbour, GridNode* endNode )
{
	float oldG = neighbour->g;
	float cost = EUCLIDIAN(node, neighbour);
	if(node->g + cost < oldG)
	{
		neighbour->pParent = node;
		neighbour->g = node->g + cost;

		float hValue = GetH(neighbour, endNode);
		neighbour->f = neighbour->g + hValue;
		push(openList, neighbour);
		neighbour->bOpened = true;
	}
}


BJMPath* BJMAstarPathfinder::PathFinding(const cocos2d::Point &from, const cocos2d::Point &to)
{
	if(from.x == to.x && from.y == to.y) return false;
	//clear before a new pathfinding
	Reset();

	GridNode* startNode = GetNodeAt(from.x, from.y);
	GridNode* endNode = GetNodeAt(to.x, to.y);
	if(!startNode || !endNode) return false;

	startNode->g = 0;
	startNode->f = MANHATTAN(startNode, endNode);
	GridNodeVec openList;
	openList.reserve(abs(int(from.x - to.x)) + abs(int(from.y - to.y)));
	push(openList, startNode);
	startNode->bOpened = true;

	GridNode* node;
	while (!openList.empty())
	{
		node = pop(openList);
		node->bClosed = true;

		if(node == endNode)
		{
			return PrintPath(endNode);
		}

		GridNodeVec neighbours;
		neighbours.reserve(BJMGame::MAX_MOVE_DIRECTIONS);
		FindNeighbours(neighbours, node);
		for(unsigned i = 0; i < neighbours.size(); ++i)
		{
			GridNode* neighbour = neighbours[i];
			if(!neighbour->bClosed)
			{
				if(!neighbour->bOpened)
				{
					neighbour->g = FLT_MAX ;
					neighbour->pParent = NULL;
				}
				updateVertex(openList, node, neighbour, endNode);
			}
		}
	}
	
	return NULL;
}




}
