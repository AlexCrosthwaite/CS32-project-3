#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"
#include "GameWorld.h"
#include "Level.h"

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Goodie;

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

	virtual bool blocksRobot()
	{
		return blocksPlayer();
	}

	virtual bool blocksBullet()
	{
		return false;
	}

	virtual bool blocksVision()
	{
		return true;
	}

	virtual bool isDamageable()
	{
		return false;
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

	void Hurt()
	{
		m_hitpoints -= 2;
		if (m_hitpoints <= 0)
		{
			setDead();
			die();		//Perform the extra actions that must be done when a specific 
		}				//Health actor dies.
		else
		{
			getHit();	//Perform the actions that must be done when a specific Health
		}				//Actor gets hit.
	}	

	virtual void die() = 0;

	virtual void getHit() = 0;

	void setHitpoints(int hitpoints)
	{
		m_hitpoints = hitpoints;
	}

	int getHitpoints()
	{
		return m_hitpoints;
	}

	virtual bool isDamageable()
	{
		return true;
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

	virtual void die();

	virtual void getHit();

	void addAmmo()
	{
		m_ammunition += 20;
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
		resetTicks();
	}

	int ticksToWait()
	{
		return m_ticksToWait;
	}

	void wait()
	{
		m_ticksToWait--;
	}

	void resetTicks();

	void getHit();

	bool playerVisible();

private:
	int m_ticksToWait;

};


//////////////////
//   SNARLBOT   //
//////////////////
class SnarlBot : public Robot
{
public:
	SnarlBot(int startX, int startY, StudentWorld* studentWorld, Direction dir)
		: Robot(IID_SNARLBOT, startX, startY, studentWorld, dir)
	{
		setHitpoints(10);
	}

	virtual void doSomething();

	void reverseDirection(Direction dir);

	void move(Direction dir);

	virtual void die();

private:

};


///////////////////
//   KLEPTOBOT   //
///////////////////
class KleptoBot : public Robot
{
public:
	KleptoBot(int imageID, int startX, int startY, StudentWorld* studentWorld)
	: Robot(imageID, startX, startY, studentWorld, right)
	{
		setHitpoints(5);
		m_goodieType = Level::empty;
		newDistance();
	}

	void doSomething();

	bool holdingGoodie()
	{
		return m_goodieType != Level::empty;
	}

	virtual void die();

	void pickUp(Goodie* goodie);

	void newDistance()
	{
		m_distanceBeforeTurning = rand() % 5 + 1;
	}

	void newDirection();

	Level::MazeEntry heldGoodie()
	{
		return m_goodieType;
	}

private:
	int m_distanceBeforeTurning;
	Level::MazeEntry m_goodieType;
};


/////////////////////////
//   ANGRY KLEPTOBOT   //
/////////////////////////
class AngryKleptoBot : public KleptoBot
{
public:
	AngryKleptoBot(int imageID, int startX, int startY, StudentWorld* studentWorld)
		: KleptoBot(imageID, startX, startY, studentWorld)
	{
		setHitpoints(8);
	}

	void doSomething();

	virtual void die();
	
private:

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

	virtual void die()
	{
		//do nothing
	}

	virtual void getHit()
	{
		//do nothing
	}

	bool blocksPlayer()
	{
		return false;
	}

	virtual bool blocksRobot()
	{
		return true;
	}

private:

};


/////////////////
//   FACTORY   //
/////////////////
class Factory : public Actor
{
public:
	Factory(int startX, int startY, StudentWorld* studentWorld, bool makesAngry)
		: Actor(IID_ROBOT_FACTORY, startX, startY, studentWorld)
	{
		m_makesAngry = makesAngry;
	}

	void doSomething();

	bool blocksBullet()
	{
		return true;
	}

	int countKleptoBots();

private:
	bool m_makesAngry;
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

	bool blocksBullet()
	{
		return true;
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
	
	virtual void doSomething();

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


	bool blocksPlayer()
	{
		return false;
	}

	virtual bool isStealable()
	{
		return true;
	}

	bool blocksVision()
	{
		return false;
	}

	virtual void rewardPlayer() = 0;

	void doSomething();

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

	virtual void rewardPlayer();

	virtual bool isStealable()
	{
		return false;
	}
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

	virtual void rewardPlayer();

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

	virtual void rewardPlayer();

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

	virtual void rewardPlayer();

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
	}

	void doSomething();

	void move(Direction dir);
	
	bool blocksVision()
	{
		return false;
	}

	bool blocksPlayer()
	{
		return false;
	}

private:
	
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

	void makeVisible()
	{
		setVisible(true);
		m_isVisible = true;
	}

	bool visible()
	{
		return m_isVisible;
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
