#include "BJMJPSPathFinder.h"
#include <iostream>

namespace BJMGame
{

BJMJPSPathFinder::BJMJPSPathFinder(void)
{
}


BJMJPSPathFinder::~BJMJPSPathFinder(void)
{
}

/*
	Looks for the neighbours of a given node.
	Returns its natural neighbours plus forced neighbours when the given
	node has no parent (generally occurs with the starting node).
	Otherwise, based on the direction of move from the parent, returns
	neighbours while pruning directions which will lead to symmetric paths.

	In case diagonal moves are forbidden, when the given node has no
	parent, we return straight neighbours (up, down, left and right).
	Otherwise, we add left and right node (perpendicular to the direction
	of move) in the neighbours list.
*/
void BJMJPSPathFinder::FindNeighbours(GridNodeVec& vec, GridNode* node)
{
	if(node->pParent)
	{
		int x = node->x;
		int y = node->y;

		int dx = (x - node->pParent->x) / _MAX(abs(x - node->pParent->x), 1);	//[-1,0,1]
		int dy = (y - node->pParent->y) / _MAX(abs(y - node->pParent->y), 1);	//[-1,0,1]

		if (dx != 0 && dy != 0)
		{
			bool walkX = false;
			bool walkY = false;
			//find natural neighbours
			if(WalkAble(x + dx, y))
			{
				vec.push_back(GetNodeAt(x + dx,y));
				walkX = true;
			}
			if(WalkAble(x, y + dy))
			{
				vec.push_back(GetNodeAt(x, y + dy));
				walkY = true;
			}
			if(walkX || walkY)
			{
				if(WalkAble(x + dx,y + dy))
				{
					vec.push_back(GetNodeAt(x + dx, y + dy));
				}
			}

			//find forced neighbours
			if(walkY && !WalkAble(x-dx, y))
			{
				if(WalkAble(x-dx, y + dy))
				{
					vec.push_back(GetNodeAt(x - dx, y + dy));
				}
			}

			if(walkX && !WalkAble(x, y - dy))
			{
				if(WalkAble(x + dx, y - dy))
				{
					vec.push_back(GetNodeAt(x + dx, y - dy));
				}
			}
		}
		else
		{
			
			if(dx == 0)
			{
				//Move along Y-axis case
				if(WalkAble(x, y + dy))
				{
					vec.push_back(GetNodeAt(x, y + dy));
				}
				// Forced neighbours are left and right ahead along Y
				if(!WalkAble(x+1, y))
				{
					if(WalkAble(x + 1, y + dy))
					{
						vec.push_back(GetNodeAt(x + 1, y + dy));
					}
				}
				if(!WalkAble(x - 1, y))
				{
					if(WalkAble(x - 1, y + dy))
					{
						vec.push_back(GetNodeAt(x - 1, y + dy));
					}
				}
			}
			else
			{
				//Move along X-axis case
				if(WalkAble(x + dx, y))
				{
					vec.push_back(GetNodeAt(x + dx, y));
				}
				//Forced neighbours are up and down ahead along X
				if(!WalkAble(x, y + 1))
				{
					if(WalkAble(x + dx, y + 1))
					{
						vec.push_back(GetNodeAt(x + dx, y + 1));
					}
				}
				if(!WalkAble(x, y - 1))
				{
					if(WalkAble(x + dx, y - 1))
					{
						vec.push_back(GetNodeAt(x + dx, y - 1));
					}
				}
			}
		}
	}
	else
	{
		BJMPathFinder::FindNeighbours(vec, node);
	}
	
}

GridNode* BJMJPSPathFinder::Jump(GridNode* node, GridNode* parent, GridNode* endNode)
{
	if(!parent || !node || !endNode) return NULL;

	int x = node->x;
	int y = node->y;

	int dx = x - parent->x;
	int dy = y - parent->y;

	//If the node to be examined is unwalkable, return nil
	if(!WalkAble(x, y)) return NULL;

	//If the node to be examined is the endNode, return this node
	if(node == endNode)
	{
		return node;
	}

	//Diagonal search case
	if(dx != 0 && dy != 0)
	{
		// Current node is a jump point if one of his leftside/rightside neighbours ahead is forced
		if( (WalkAble(x - dx, y + dy) && !WalkAble(x - dx, y)) ||
			(WalkAble(x + dx, y - dy) && !WalkAble(x, y - dy)))
		{
			return node;
		}
	}
	else
	{
		if(dx != 0)
		{
			//Current node is a jump point if one of his upside/downside neighbours is forced
			if((WalkAble(x + dx, y + 1) && !WalkAble(x, y + 1)) ||
				(WalkAble(x + dx, y -1) && !WalkAble(x, y - 1)))
			{
				return node;
			}
		}
		else
		{
			//Current node is a jump point if one of his leftside/rightside neighbours is forced
			//dy != 0
			if((WalkAble(x + 1, y + dy) && !WalkAble(x + 1, y)) ||
				(WalkAble(x -1, y + dy) && !WalkAble(x - 1, y)))
			{
				return node;
			}
		}
	}

	//Recursive horizontal/vertical search
	if(dx != 0 && dy != 0)
	{
		if(Jump(GetNodeAt(x + dx, y), node, endNode)) return node;
		if(Jump(GetNodeAt(x, y + dy), node, endNode)) return node;
	}

	//Recursive diagonal search
	if(WalkAble(x + dx, y) || WalkAble(x, y + dy)) 
		return Jump(GetNodeAt(x + dx, y + dy), node, endNode);

	return NULL;
}

void BJMJPSPathFinder::IdentifySuccessors( GridNodeVec& openList, GridNode* node, GridNode* endNode )
{
	GridNodeVec neighbours;
	neighbours.reserve(BJMGame::MAX_MOVE_DIRECTIONS);
	FindNeighbours(neighbours, node);
	for(GridNodeVec::iterator it = neighbours.begin(); it != neighbours.end(); ++it)
	{
		GridNode* neighbour = *it;
		GridNode* jumpNode = Jump(neighbour, node, endNode);
		if(jumpNode && !jumpNode->bClosed)
		{
			float extraG = EUCLIDIAN(jumpNode, node);
			float newG = node->g + extraG;
			if(!jumpNode->bOpened || newG < jumpNode->g)
			{
				jumpNode->g = newG;
				float hValue = GetH(jumpNode, endNode);
				jumpNode->f = jumpNode->g + hValue;
				jumpNode->pParent = node;

				if(!jumpNode->bOpened)
				{
					//not in openList, insert
					jumpNode->bOpened = true;
					push(openList, jumpNode);
				}
				else
				{
					//already in openList, reOrder
					heapify(openList, jumpNode);
				}
			}
		}
	}
}

BJMPath* BJMJPSPathFinder::PathFinding(const cocos2d::Point & from, const cocos2d::Point & to)
{
	
	if(from.x == to.x && from.y == to.y) return false;
	//clear before a new pathfinding
	Reset();

	GridNode* startNode = GetNodeAt(from.x, from.y);
	GridNode* endNode = GetNodeAt(to.x, to.y);
	if(!startNode || !endNode) return false;

	GridNodeVec openList;
	push(openList, startNode);
	startNode->bOpened = true;

	GridNode* node;
	while (!openList.empty())
	{
		node = pop(openList);
		node->bClosed = true;

		if(node == endNode)
		{
			FillPath(endNode);
			return PrintPath(endNode);
		}
		IdentifySuccessors(openList, node, endNode);
	}

	return NULL;
}

BJMJPSPathFinder* BJMJPSPathFinder::Create(int width, int height)
{
	BJMJPSPathFinder* p = new BJMJPSPathFinder();
	if(!p->Init(width, height))
	{
		delete p;
		p = NULL;
	}
	return p;
}

void BJMJPSPathFinder::FillPath(GridNode* endNode)
{
	if(!endNode) return;

	GridNode* node = endNode;
	while(node->pParent)
	{
		GridNode* parent = node->pParent;
		int dx = parent->x - node->x;
		int dy = parent->y - node->y;
		int incrX = dx / _MAX(abs(dx), 1);
		int incrY = dy / _MAX(abs(dy), 1);

		GridNode* preNode = GetNodeAt(node->x + incrX, node->y + incrY);
		while(preNode != parent)
		{
			node->pParent = preNode;
			node = preNode;
			preNode = GetNodeAt(node->x + incrX, node->y + incrY);
		}

		node->pParent = parent;
		node = parent;
	}
}

}
