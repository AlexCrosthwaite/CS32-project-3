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
		m_levelComplete = false;
		m_jewels = 0;
	}


	
	~StudentWorld()
	{
		cleanUp();
	}

	virtual int init();
	virtual int move();
	virtual void cleanUp();

	Player* player()
	{
		return m_player;
	}
	
	//Use this function to find an actor with specific x and y coordinates
	Actor* getActor(int x, int y);
	

	//Use this function when you want to find a specific type, not just a generic actor
	Boulder* findBoulder(int x, int y);


	unsigned int getBonus()
	{
		return m_bonus;
	}

	//Decrement the bonus during a tick
	void decBonus()
	{
		if (m_bonus > 0)
		m_bonus--;
	}

	void completeLevel()
	{
		m_levelComplete = true;
	}

	void getJewel()
	{
		m_jewels--;
	}

	bool jewelsLeft()
	{
		return m_jewels > 0;
	}

	//Level::MazeEntry whatTypeIsThis(Actor* ap);

	void removeDeadActors();

	//loads the current level data into our ActorList
	int loadAlevel();

	//sets the display text at the top of the screen 
	void setDisplayText();

	//add a bullet into the level
	void ShootBullet(int x, int y, GraphObject::Direction dir);

	Actor* FindNOTBullet(int x, int y);

private:
	std::list<Actor*> m_ActorList;
	Player* m_player;
	unsigned int m_bonus;
	bool m_levelComplete;
	int m_jewels;
	
};

#endif // STUDENTWORLD_H_
