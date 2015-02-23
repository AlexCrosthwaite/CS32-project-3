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

	virtual void doSomething() = 0;

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

	virtual bool blocksVision()
	{
		return true;
	}

	void dirToDelta(Direction dir, int& dx, int& dy)
	{
		switch (dir)
		{
		case up:
			dx = 0;
			dy = 1;
			break;
		case right:
			dx = 1;
			dy = 0;
			break;
		case down:
			dx = 0;
			dy = -1;
			break;
		case left:
			dx = -1;
			dy = 0;
			break;
		}
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

	virtual void Hurt()
	{
		m_hitpoints -= 2;
		if (m_hitpoints <= 0)
		{
			setDead();
		}
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


/////////////////
//   SHOOTER   //
/////////////////

class Shooter : public HealthActor
{
public: 
	Shooter(int imageID, int startX, int startY, StudentWorld* studentWorld, Direction dir)
		: HealthActor(imageID, startX, startY, studentWorld, dir)
	{
	}

	void shoot();

private:
	
};


////////////////
//   PLAYER   //
////////////////
class Player : public Shooter
{
public:
	Player(int startX, int startY, StudentWorld* studentWorld)
		: Shooter(IID_PLAYER, startX, startY, studentWorld, right)
	{	
		setHitpoints(20);
		m_ammunition = 20;
	}

	void doSomething();

	int getAmmunition()
	{
		return m_ammunition;
	}

	void Hurt();

	void addAmmo()
	{
		m_ammunition += 20;
	}

	bool blocksPlayer()
	{
		return false;
	}

	void move(Direction dir);

private:
	int m_ammunition;

};


///////////////
//   ROBOT   //
///////////////
class Robot : public Shooter
{
public:
	Robot(int imageID, int startX, int startY, StudentWorld* studentworld, Direction dir)
		: Shooter(imageID, startX, startY, studentworld, dir)
	{
		setHitpoints(10);
		resetTicks();
	}

	int ticksToWait()
	{
		return m_ticksToWait;
	}

	void resetTicks();
	
	bool playerVisible();

private:
	int m_ticksToWait;

};


//////////////////
//   SNARLBOT   //
//////////////////
class SnarlBot :public Shooter
{
public:
	SnarlBot(int startX, int startY, StudentWorld* studentWorld, Direction dir)
		: Shooter(IID_SNARLBOT, startX, startY, studentWorld, dir)
	{
		setHitpoints(10);
	}

	void doSomething();

	void Hurt() {}

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

	bool blocksVision()
	{
		return false;
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

	bool blocksVision()
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


////////////////
//   BULLET   //
////////////////
class Bullet : public Actor
{
public:
	Bullet(int startX, int startY, StudentWorld* studentWorld, Direction dir)
	: Actor(IID_BULLET, startX, startY, studentWorld, dir)
	{
		m_justSpawned = true;
	}

	void doSomething();

	void move(Direction dir);
	
	bool blocksVision()
	{
		return false;
	}
private:
	bool m_justSpawned;
};

//////////////
//   EXIT   //
//////////////
class Exit : public Actor
{
public:
	Exit(int startX, int startY, StudentWorld* studentWorld)
	: Actor(IID_EXIT, startX, startY, studentWorld)
	{
		setVisible(false);
		m_isVisible = false;
	}

	void doSomething();

	bool blocksPlayer()
	{
		return false;
	}

	bool blocksVision()
	{
		return false;
	}

private:
	bool m_isVisible;

};


#endif // ACTOR_H_
