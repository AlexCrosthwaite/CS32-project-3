#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"
#include "GameWorld.h"
#include "Level.h"

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor : public GraphObject
{
public:
	Actor(int imageID, int startX, int startY, 
		StudentWorld* studentWorld, Direction startDirection = none)
		: GraphObject(imageID, startX, startY, startDirection)
	{
		setVisible(true);
		m_world = studentWorld;
		m_alive = true;
	}

	virtual ~Actor()
	{
	}

	virtual void doSomething()
	{
	}

	StudentWorld* getWorld()
	{
		return m_world;
	}

	bool isAlive()
	{
		return m_alive;
	}

	void setDead()
	{
		m_alive = false;
	}

	virtual bool blocksPlayer()
	{
		return true;
	}

private:
	StudentWorld* m_world;
	bool m_alive;
};


class HealthActor : public Actor
{
public:
	HealthActor(int imageID, int startX, int startY, 
		StudentWorld* studentWorld, Direction startDirection = none)
	: Actor(imageID, startX, startY, studentWorld, startDirection)
	{
	}
	
	virtual ~HealthActor()
	{
	}

	virtual void getAttacked()
	{

	}
	void setHitpoints(int hitpoints)
	{
		m_hitpoints = hitpoints;
	}
	int getHitpoints()
	{
		return m_hitpoints;
	}

private:
	int m_hitpoints;
};

class Player : public HealthActor
{
public:
	Player(int startX, int startY, StudentWorld* studentWorld)
		: HealthActor(IID_PLAYER, startX, startY, studentWorld, right)
	{	
		setHitpoints(20);
		m_ammunition = 20;
	}

	void doSomething();

	void getAttacked()
	{

	}

	int getAmmunition()
	{
		return m_ammunition;
	}

	bool blocksPlayer()
	{
		return false;
	}

private:
	int m_ammunition;

};

class Boulder : public HealthActor
{
public:
	Boulder(int startX, int startY, StudentWorld* studentWorld)
		: HealthActor(IID_BOULDER, startX, startY, studentWorld)
	{
		setHitpoints(10);
	}

	void getAttacked()
	{
	}

	bool push(Direction dir);

	void doSomething()
	{
		//do nothing
	}

	bool blocksPlayer()
	{
		return false;
	}

private:


};
class Wall : public Actor
{
public:
	Wall(int startX, int startY, StudentWorld* studentWorld)
		: Actor(IID_WALL, startX, startY, studentWorld)
	{
	}

	void doSomething()
	{
		//do nothing
	}

private:

};

class Hole : public Actor
{
public:
	Hole(int startX, int startY, StudentWorld* studentWorld)
		: Actor(IID_HOLE, startX, startY, studentWorld)
	{
	}
	
	void doSomething();

	bool blocksPlayer()
	{
		return true;
	}
private:

};

class Goodie : public Actor
{
public:
	Goodie(int imageID, int startX, int startY, StudentWorld* studentworld)
		: Actor(imageID, startX, startY, studentworld)
	{
	}

	~Goodie()
	{
	}

	bool doSomethingGoodie();

private:

};




#endif // ACTOR_H_
