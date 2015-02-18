#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include "Actor.h"
#include <list>
#include <string>


// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
	 : GameWorld(assetDir)
	{
	}

	~StudentWorld()
	{
		delete m_player;
		auto ap = m_ActorList.begin();
		while (ap != m_ActorList.end())
		{
			delete *ap;
			ap = m_ActorList.erase(ap);
		}
	}
	virtual int init();
	virtual int move();
	virtual void cleanUp();

	Player* player()
	{
		return m_player;
	}
	
	Actor* getActor(int x, int y)
	{
		for (auto ap : m_ActorList)
		{
			if (ap->getX() == x && ap->getY() == y)
				return ap;
		}
		return nullptr;
	}

	Boulder* findBoulder(int x, int y)
	{
		for (auto ap : m_ActorList)
		{
			if (ap->getX() == x && ap->getY() == y)
			{
				Boulder* bp = dynamic_cast<Boulder*>(ap);
				if (bp != nullptr)
				{
					return bp;
				}
			}
		}
		return nullptr;
	}

	/*void setEmpty(int x, int y)
	{
		m_maze[x][y] = Level::empty;
	}

	void setFilled(int x, int y, Level::MazeEntry me)
	{
		m_maze[x][y] = me;
	}*/

	unsigned int getBonus()
	{
		return m_bonus;
	}

	void decBonus()
	{
		if (m_bonus > 0)
		m_bonus--;
	}

	Level::MazeEntry whatTypeIsThis(Actor* ap);

	void removeDeadActors();

	int loadAlevel();
	void setDisplayText();

private:
	std::list<Actor*> m_ActorList;
	Player* m_player;
	unsigned int m_bonus;
	
};

#endif // STUDENTWORLD_H_
