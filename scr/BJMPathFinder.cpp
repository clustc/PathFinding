#include "BJMPathFinder.h"
#include "features/game/2darpg/map/BJMMapManager.h"

namespace BJMGame
{

bool checkBinaryHeapValid(GridNodeVec& vec)
{
	//判断第一个是否为最小值
	if(vec.empty()) return true;

	float min_f = vec[0]->f;
	for(unsigned i = 1; i < vec.size(); ++i)
	{
		if(vec[i]->f < min_f)
		{
			return false;
		}
	}
	return true;
}

bool cmp(GridNode* nodeA, GridNode* nodeB)
{
	return nodeA->f < nodeB->f;
}

void percolate_up(GridNodeVec& vec, int index)
{
	if(index <= 0) return;
	int parentIndex;
	if(index%2 == 0)
	{
		parentIndex = index/2 -1;
	}
	else
	{
		parentIndex = (index - 1) /2;
	}

	if(cmp(vec[index], vec[parentIndex]))
	{
		//当前节点权值高于父节点权值（f越小权值越高）,则交换
		GridNode* pTemp = vec[index];
		vec[index] = vec[parentIndex];
		vec[parentIndex] = pTemp;

		//递归
		percolate_up(vec, parentIndex);
	}
}

void percolate_down(GridNodeVec& vec, int index)
{
	int lfIndex, rtIndex, minIndex;
	lfIndex = index*2 + 1;
	rtIndex = lfIndex + 1;
	if(rtIndex >= (int)vec.size())
	{
		if(lfIndex >= (int)vec.size())
		{
			return;
		}
		else
		{
			minIndex = lfIndex;
		}
	}
	else
	{
		if(cmp(vec[lfIndex], vec[rtIndex]))
		{
			minIndex = lfIndex;
		}
		else
		{
			minIndex = rtIndex;
		}
	}

	if(cmp(vec[minIndex], vec[index]))
	{
		GridNode* pTemp = vec[index];
		vec[index] = vec[minIndex];
		vec[minIndex] = pTemp;

		percolate_down(vec, minIndex);
	}
}

void push(GridNodeVec& vec,GridNode* item)
{
	vec.push_back(item);
	percolate_up(vec, vec.size()-1);
}

GridNode* pop(GridNodeVec& vec)
{
	if(vec.size() < 1) return NULL;
	GridNode* root = vec[0];
	vec[0] = vec[vec.size()-1];
	vec.pop_back();
	if(vec.size() > 1)
	{
		percolate_down(vec, 0);
	}
	return root;
}

void heapify(GridNodeVec& vec, GridNode* item)
{
	int index = -1;
	for(int i = 0; i<(int)vec.size(); ++i)
	{
		if(vec[i] == item)
		{
			index = i;
			break;
		}
	}
	if(index != -1)
	{
		percolate_down(vec, index);
		percolate_up(vec, index);
	}
}


BJMPathFinder::BJMPathFinder(void)
	: m_GridWidth(0)
	,m_GridHeight(0)
	,m_grid(NULL)
{
}


BJMPathFinder::~BJMPathFinder(void)
{
	Clear();
}

BJMPath* BJMPathFinder::PrintPath( GridNode* endNode )
{
	std::vector<short> path;
	GridNode* node = endNode;
	while (node->pParent)
	{
		GridNode* parent = node->pParent;
		int dx = node->x - parent->x;
		int dy = node->y - parent->y;

		path.push_back(GetDir(dx, dy));

		node = parent;
	}
	return BJMPath::Create(path);
}

void BJMPathFinder::Reset()
{
	if(m_grid)
	{
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
}

void BJMPathFinder::Clear()
{
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

void BJMGame::BJMPathFinder::FindNeighbours(GridNodeVec& output, GridNode* node)
{
	if(!output.empty())
	{
		output.clear();
	}
	
	int Indexes[MAX_MOVE_DIRECTIONS] = {0,2,4,6,1,3,5,7};

	for(int i = 0; i<MAX_MOVE_DIRECTIONS; i++)
	{
		int index = Indexes[i];
		int x = node->x + BJMGame::MOVE_DIRECTION_OFFSETS[index][0];
		int y = node->y + BJMGame::MOVE_DIRECTION_OFFSETS[index][1];

		GridNode* pNode = GetNodeAt(x,y);
		if(pNode && WalkAble(x,y))
		{
			output.push_back(pNode);
		}

	}
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
		m_grid[y][x] = new GridNode(x,y);
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
