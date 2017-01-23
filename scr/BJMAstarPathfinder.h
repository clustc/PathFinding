/****************************************************************************
Author : Caili @Bojoy 2015
FileName: 
Description: 
****************************************************************************/

#pragma once

#include "BJMPathFinder.h"

namespace BJMGame
{
//------------------------------------- BJMAstarPathfinder -------------------------------------//
class BJMAstarPathfinder : public BJMPathFinder
{
public:
	BJMAstarPathfinder(void);
	~BJMAstarPathfinder(void);

	static BJMAstarPathfinder* Create(int width, int height);

	void updateVertex(GridNodeVec& openList, GridNode* node, GridNode* neighbour, GridNode* endNode);

	BJMPath* PathFinding(const cocos2d::Point & from, const cocos2d::Point & to);

};

}
