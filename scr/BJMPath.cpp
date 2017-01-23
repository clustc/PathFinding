#include "BJMPath.h"
#include "features/game/2darpg/map/BJMMapManager.h"

namespace BJMGame
{


BJMPath::BJMPath()
{
}

BJMPath::~BJMPath()
{
	if(!m_path.empty())
	{
		m_path.clear();
	}
}

bool BJMPath::Init( std::vector<short> path )
{
	m_path = path;
	return true;
}

BJMGame::BJMPath* BJMPath::Create( std::vector<short> path_vec )
{
	BJMPath* path = new BJMPath;
	if(!path->Init(path_vec))
	{
		delete path;
		path = NULL;
	}
	return path;
}

bool BJMPath::HasNextStep()
{
	return !m_path.empty();
}

short BJMPath::GetNextStep()
{
	if(HasNextStep())
	{
		short nextPos = m_path.back();
		m_path.pop_back();
		return nextPos;
	}
	else
	{
		n_warning("No next step!");
		return 0;
	}
}

short BJMPath::WatchNextStep()
{
	if(HasNextStep())
	{
		short nextPos = m_path.back();
		return nextPos;
	}
	else
	{
		n_warning("No next step!");
		return 0;
	}
}

int BJMPath::GetLeftStepNum()
{
	return m_path.size();
}

std::string BJMPath::GetLeftStepStr()
{
	if(!m_path.empty())
	{
		std::string result = "";
		std::vector<short>::iterator i; 
		for (i = m_path.begin(); i != m_path.end(); ++i)  
		{
			result = result + StringUtils::toString(*i);
		}
		return result;
	}
	return "";
}

}
