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
	Actor(int imageID, int startX, int startY, StudentWorld* studentWorld, 
		Level::MazeEntry entry, Direction startDirection = none)
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

	Level::MazeEntry entry()
	{
		return m_me;
	}

private:
	StudentWorld* m_world;
	bool m_alive;
	Level::MazeEntry m_me;
};


class HealthActor : public Actor
{
public:
	HealthActor(int imageID, int startX, int startY, StudentWorld* studentWorld, 
		Level::MazeEntry entry, Direction startDirection = none)
	: Actor(imageID, startX, startY, studentWorld, entry, startDirection)
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
		: HealthActor(IID_PLAYER, startX, startY, studentWorld, Level::player, right)
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

private:
	int m_ammunition;

};

class Boulder : public HealthActor
{
public:
	Boulder(int startX, int startY, StudentWorld* studentWorld)
		: HealthActor(IID_BOULDER, startX, startY, studentWorld, Level::boulder)
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

private:


};
class Wall : public Actor
{
public:
	Wall(int startX, int startY, StudentWorld* studentWorld)
		: Actor(IID_WALL, startX, startY, studentWorld, Level::wall)
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
		: Actor(IID_HOLE, startX, startY, studentWorld, Level::hole)
	{
	}
	
	void doSomething();
private:

};


#endif // ACTOR_H_
