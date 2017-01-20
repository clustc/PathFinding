#include "BJMPathFinder.h"
#include "features/game/2darpg/map/BJMMapManager.h"

namespace BJMGame
{


BJMPathFinder::BJMPathFinder(void)
	: m_GridWidth(0)
	,m_GridHeight(0)
	,m_grid(NULL)
{
}


BJMPathFinder::~BJMPathFinder(void)
{
	if(!m_shortestPath.empty())
	{
		m_shortestPath.clear();
	}

	if(m_grid)
	{
		for(int i = 0; i < m_GridHeight; ++i )
		{
			for(int j = 0; j<m_GridWidth; ++j)
			{
				GridNode* node = m_grid[i][j];
				if(node)
				{
					delete node;
					node = NULL;
				}
			}
			delete []m_grid[i];
		}
		delete []m_grid;
		m_grid = NULL;
	}
}

void BJMPathFinder::PrintPath( GridNode* endNode )
{
	if (!m_shortestPath.empty())
	{
		m_shortestPath.clear();
	}
	GridNode* node = endNode;
	while (node->pParent)
	{
		GridNode* parent = node->pParent;
		int dx = node->x - parent->x;
		int dy = node->y - parent->y;

		m_shortestPath.push_back(GetDir(dx, dy));

		node = parent;
	}
}

bool BJMPathFinder::HasNextStep()
{
	return !m_shortestPath.empty();
}

short BJMPathFinder::GetNextStep()
{
	if(HasNextStep())
	{
		short nextPos = m_shortestPath.back();
		m_shortestPath.pop_back();
		return nextPos;
	}
	else
	{
		n_warning("No next step!");
		return 0;
	}
}

int BJMPathFinder::GetLeftStepNum()
{
	return m_shortestPath.size();
}

std::string BJMPathFinder::GetLeftStepStr()
{
	if(!m_shortestPath.empty())
	{
		std::string result = "";
		std::list<short>::iterator i; 
		for (i = m_shortestPath.begin(); i != m_shortestPath.end(); ++i)  
		{
			result = result + StringUtils::toString(*i);
		}
		return result;
	}
	return "";
}

short BJMPathFinder::WatchNextStep()
{
	if(HasNextStep())
	{
		short nextPos = m_shortestPath.back();
		return nextPos;
	}
	else
	{
		n_warning("No next step!");
		return 0;
	}
}

void BJMPathFinder::Clear()
{
	if(!m_shortestPath.empty())
	{
		m_shortestPath.clear();
	}

	for(int i = 0; i < m_GridHeight; ++i )
	{
		for(int j = 0; j<m_GridWidth; ++j)
		{
			GridNode* node = m_grid[i][j];
			if(node)
			{
				node->Reset();
			}
		}

	}
}


bool BJMGame::BJMPathFinder::Init(int width, int height)
{
	if(width <= 0 || height <= 0) return false;
	m_GridWidth = width;
	m_GridHeight = height;

	m_grid = new GridNode**[m_GridHeight];
	for(int i = 0; i < m_GridHeight; ++i )
	{
		m_grid[i] = new GridNode*[m_GridWidth];
		for(int j = 0; j<m_GridWidth; ++j)
		{
			m_grid[i][j] = NULL;
		}
	}
	

	return true;
}


bool BJMGame::BJMPathFinder::WalkAble( int x, int y )
{
	bool bBlocked = BJMApp::BJMMapManager::Instance()->IsBlocked(x,y);
	return  !bBlocked;
}

BJMGame::GridNodeVec BJMGame::BJMPathFinder::FindNeighbours( GridNode* node )
{
	GridNodeVec vec;
	for(int i = 0; i<MAX_MOVE_DIRECTIONS; i++)
	{
		int x = node->x + BJMGame::MOVE_DIRECTION_OFFSETS[i][0];
		int y = node->y + BJMGame::MOVE_DIRECTION_OFFSETS[i][1];

		GridNode* pNode = GetNodeAt(x,y);
		if(pNode)
		{
			vec.push_back(pNode);
		}

	}
	return vec;
}

int BJMGame::BJMPathFinder::GetDir( int deltaX, int deltaY )
{
	for(int i = 0; i<MAX_MOVE_DIRECTIONS; i++)
	{
		if(deltaX == BJMGame::MOVE_DIRECTION_OFFSETS[i][0] && deltaY == BJMGame::MOVE_DIRECTION_OFFSETS[i][1])  
		{
			return i;
		}
	}
	return -1;
}

GridNode* BJMGame::BJMPathFinder::GetNodeAt(int x, int y)
{
	if (x < 0 || x >= m_GridWidth || y < 0 || y >= m_GridHeight)
	{
		return NULL;
	}

	if(!m_grid[y][x])
	{
		m_grid[y][x] = n_new(GridNode(x,y));
	}
	return m_grid[y][x];
}

float BJMGame::BJMPathFinder::GetH( GridNode* nodeA, GridNode* nodeB )
{
	return MANHATTAN(nodeA, nodeB);
}

float BJMGame::BJMPathFinder::MANHATTAN( GridNode* nodeA, GridNode* nodeB )
{
	if(!nodeA || !nodeB) return 0;
	
	float dx = fabs(float(nodeA->x - nodeB->x));
	float dy = fabs(float(nodeA->y - nodeB->y));
	float hValue = dx + dy;
	return hValue;
}

float BJMGame::BJMPathFinder::EUCLIDIAN( GridNode* nodeA, GridNode* nodeB )
{
	if(!nodeA || !nodeB) return 0;
	
	float dx = fabs(float(nodeA->x - nodeB->x));
	float dy = fabs(float(nodeA->y - nodeB->y));
	float hValue = sqrt( dx * dx + dy * dy);
	return hValue;
}

float BJMGame::BJMPathFinder::DIAGONAL( GridNode* nodeA, GridNode* nodeB )
{
	if(!nodeA || !nodeB) return 0;
	float dx = fabs(float(nodeA->x - nodeB->x));
	float dy = fabs(float(nodeA->y - nodeB->y));
	float hValue = dx > dy ? dx : dy;
	return hValue;
}

float BJMGame::BJMPathFinder::CARDINTCARD( GridNode* nodeA, GridNode* nodeB )
{
	if(!nodeA || !nodeB) return 0;
	float dx = fabs(float(nodeA->x - nodeB->x));
	float dy = fabs(float(nodeA->y - nodeB->y));
	float hValue = (dx < dy ? dx : dy)*sqrt(2) + fabs(dx - dy);
	return hValue;
}

}
