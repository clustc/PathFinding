/****************************************************************************
Author : Caili @Bojoy 2015
FileName: BJMMapLoader.h
Description: 
****************************************************************************/

#pragma once

#include <vector>
#include "cocos2d.h"

namespace BJMGame
{
	class BJMPath
	{
	public:
		BJMPath();
		~BJMPath();

		bool Init(std::vector<short> path);

		static BJMPath* Create(std::vector<short> path);

		bool	HasNextStep();
		short	GetNextStep();
		short	WatchNextStep();
		int		GetLeftStepNum();
		std::string	GetLeftStepStr();

	private:
		std::vector<short> m_path;
	};

}
