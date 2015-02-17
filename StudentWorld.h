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
		std::list<Actor*>::iterator ap = m_ActorList.begin();
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

	Level::MazeEntry ActorAt(int x, int y)
	{
		return m_maze[x][y];
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

	int loadAlevel();
	void setDisplayText();

private:
	Level::MazeEntry m_maze[VIEW_WIDTH][VIEW_HEIGHT];
	std::list<Actor*> m_ActorList;
	Player* m_player;
	unsigned int m_bonus;
	
};

#endif // STUDENTWORLD_H_
