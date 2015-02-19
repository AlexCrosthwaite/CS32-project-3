#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"
#include "GameWorld.h"
#include "Level.h"

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp


///////////////
//   ACTOR   //
///////////////
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



//////////////////////
//   HEALTH ACTOR   //
//////////////////////
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



////////////////
//   PLAYER   //
////////////////
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

	void addAmmo()
	{
		m_ammunition += 20;
	}

	bool blocksPlayer()
	{
		return false;
	}

private:
	int m_ammunition;

};


/////////////////
//   BOULDER   //
/////////////////
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


	bool blocksPlayer()
	{
		return false;
	}

private:


};


//////////////
//   WALL   //
//////////////
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


//////////////
//   HOLE   //
//////////////
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


////////////////
//   GOODIE   //
////////////////
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

	virtual bool blocksPlayer()
	{
		return false;
	}

	virtual bool doSomethingGoodie(); //This funtion will be used by all derived goodies

private:

};

///////////////
//   JEWEL   //
///////////////
class Jewel : public Goodie
{
public:
	Jewel(int startX, int startY, StudentWorld* studentworld)
		: Goodie(IID_JEWEL, startX, startY, studentworld)
	{
	}

	void doSomething();

private:
};

////////////////////
//   EXTRA LIFE   //
////////////////////
class ExtraLifeGoodie : public Goodie
{
public:
	ExtraLifeGoodie(int startX, int startY, StudentWorld* studentworld)
		: Goodie(IID_EXTRA_LIFE, startX, startY, studentworld)
	{
	}

	void doSomething();

private:
};


//////////////
//   AMMO   //
//////////////
class AmmoGoodie : public Goodie
{
public:
	AmmoGoodie(int startX, int startY, StudentWorld* studentworld)
		: Goodie(IID_AMMO, startX, startY, studentworld)
	{
	}

	void doSomething();

private:
};


////////////////////////
//   RESTORE HEALTH   //
////////////////////////
class RestoreHealthGoodie : public Goodie
{
public:
	RestoreHealthGoodie(int startX, int startY, StudentWorld* studentworld)
		: Goodie(IID_RESTORE_HEALTH, startX, startY, studentworld)
	{
	}

	void doSomething();

private:
};




#endif // ACTOR_H_
