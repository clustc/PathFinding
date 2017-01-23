/****************************************************************************
Author : CaiLi @Bojoy 2017
FileName: 
Description: 
****************************************************************************/

#pragma once

#include "BJMPathFinder.h"

namespace BJMGame
{

class BJMJPSPathFinder : public BJMPathFinder
{
public:
	BJMJPSPathFinder(void);
	~BJMJPSPathFinder(void);

	void FindNeighbours(GridNodeVec& output, GridNode* node);

	//Searches for a jump point (or a turning point) in a specific direction.
	GridNode* Jump(GridNode* node, GridNode* parent, GridNode* endNode);

	//Searches for successors of a given node in the direction of each of its neighbours.
	void IdentifySuccessors(GridNodeVec& openList, GridNode* node, GridNode* endNode);

	BJMPath* PathFinding(const cocos2d::Point & from, const cocos2d::Point & to);

	//`Path` filling modifier. Interpolates between non contiguous nodes along a `path`
	void FillPath(GridNode* endNode);

	static BJMJPSPathFinder* Create(int width, int height);

};

}
