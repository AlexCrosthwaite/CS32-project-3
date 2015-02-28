#include "Actor.h"
#include "StudentWorld.h"
#include "GraphObject.h"
#include <cstdlib>
#include <vector>

using namespace std;

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp


/////////////////////////////////
//   PLAYER  IMPLEMENTATIONS   //
/////////////////////////////////
void Player::doSomething()
{
	int key;
	if (getWorld()->getKey(key))
	{
		//a key was pressed this tick
		switch (key)
		{
		case KEY_PRESS_LEFT:
			move(left);
			break;
		case KEY_PRESS_DOWN:
			move(down);
			break;
		case KEY_PRESS_RIGHT:
			move(right);
			break;
		case KEY_PRESS_UP:
			move(up);
			break;
		case KEY_PRESS_SPACE:
			if (m_ammunition > 0)
			{
				shoot();
				getWorld()->playSound(SOUND_PLAYER_FIRE);
				m_ammunition--;
			}
			break;
		case KEY_PRESS_ESCAPE:
			getWorld()->decLives();
			break;
		}
	}
}

void Player::move(Direction dir)
{
	int dx;
	int dy;

	dirToDelta(dir, dx, dy);

	setDirection(dir);

	Actor* ap = getWorld()->findObstruction(getX() + dx, getY() + dy);

	if (ap == nullptr) //this means the spot is empty
	{
		moveTo(getX() + dx, getY() + dy);
	}

	else if (!(ap->blocksPlayer()))		//The only obstruction that does not always block a player
	{									//is a boulder
		Boulder* bp = dynamic_cast<Boulder*>(ap);

		if (bp->push(dir) == true) //check to see if we can move the boulder
		{
			moveTo(getX() + dx, getY() + dy);
		}
	}
}

void Player::die()
{
	getWorld()->decLives();
	getWorld()->playSound(SOUND_PLAYER_DIE);
}

void Player::getHit()
{
	getWorld()->playSound(SOUND_PLAYER_IMPACT);
}


/////////////////////////////////
//   SHOOTER IMPLEMENTATIONS   //
/////////////////////////////////
void Shooter::shoot()
{
	getWorld()->ShootBullet(getX(), getY(), getDirection());
}


///////////////////////////////
//   ROBOT IMPLEMENTATIONS   //
///////////////////////////////
void Robot::resetTicks()
{
	int ticks = (28 - getWorld()->getLevel()) / 4;

	if (ticks < 3)
		m_ticksToWait = 3;
	else
		m_ticksToWait = ticks;
}

void Robot::getHit()
{
	getWorld()->playSound(SOUND_ROBOT_IMPACT);
}

bool Robot::playerVisible()
{
	int dx;
	int dy;
	dirToDelta(getDirection(), dx, dy);
	for (int i = 1;; i++)
	{
		if (getWorld()->player()->getX() == getX() + i*dx && getWorld()->player()->getY() == getY() + i*dy)
			return true;
		Actor* ap = getWorld()->findObstruction(getX() + i*dx, getY() + i*dy);
		if (ap == nullptr)
			continue;
		else if (ap->blocksVision())
			return false;
	}
}


//////////////////////////////////
//   SNARLBOT IMPLEMENTATIONS   //
//////////////////////////////////
void SnarlBot::doSomething()
{
	if (!isAlive())		//if the robot is dead, we dont want to do something
		return;
	if (ticksToWait() > 1)		//If the snarlbot is not allowed to move this tick
	{							// then we dont want to do anything
		wait();		
		return;
	}
	else
	{
		resetTicks();			//the snarlbot performed an action, so reset to tick counter
		if (playerVisible())	
		{						//If the snarlbot can see the player, then we want to shoot
			shoot();
			getWorld()->playSound(SOUND_ENEMY_FIRE);
		}
		else
		{
			move(getDirection());	//Otherwise, the snarlbot should move
		}
	}
}

void SnarlBot::move(Direction dir)
{
	int dx;
	int dy;

	dirToDelta(dir, dx, dy);

	Actor* ap = getWorld()->findObstruction(getX() + dx, getY() + dy);

	
	if (ap != nullptr) //anything that blocks a player blocks a robot as well
	{
		reverseDirection(dir); //If the snarlbot cant move, reverse its direction
	}
	else
	{		//if the snarlbot is not blocked, then it can (and should) move
		moveTo(getX() + dx, getY() + dy);
	}
}

void SnarlBot::reverseDirection(Direction dir)
{
	switch (dir)
	{
	case up:
		setDirection(down);
		break;
	case right:
		setDirection(left);
		break;
	case down:
		setDirection(up);
		break;
	case left:
		setDirection(right);
		break;
	}
}

void SnarlBot::die()
{
	getWorld()->increaseScore(100);
	getWorld()->playSound(SOUND_ROBOT_DIE);
}


///////////////////////////////////
//   KLEPTOBOT IMPLEMENTATIONS   //
///////////////////////////////////
void KleptoBot::doSomething()
{
	if (!isAlive())
		return;
	if (ticksToWait() > 1)
	{
		wait();
		return;
	}
	//At this point, the KleptoBot should do something.
	//So, we should reset its tick count

	resetTicks();

	if (!holdingGoodie())
	{
		Actor* ap = getWorld()->FindNOTBullet(getX(), getY());
		if (ap != nullptr)
		{
			Goodie* gp = dynamic_cast<Goodie*>(ap);

			if (gp != nullptr && gp->isStealable())
			{
				int chanceToPickup = rand() % 10;
				if (chanceToPickup == 0) //just choose one of the 10 options for when a goodie should be picked up
				{
					pickUp(gp);
					getWorld()->playSound(SOUND_ROBOT_MUNCH);
					return;		//If the KleptoBot picked up the goodie, then it should do nothign else during this tick
				}
			}
		}
	}
	
	if (m_distanceBeforeTurning > 0)
	{
		int dx;
		int dy;

 		dirToDelta(getDirection(), dx, dy);

		if (getWorld()->player()->getX() == getX() + dx 
			&& getWorld()->player()->getY() == getY() + dy)
		{
			newDistance();
			newDirection();
			return;
		}

		Actor* ap = getWorld()->findObstruction(getX() + dx, getY() + dy);


		if (ap != nullptr) //Here, the robot is blocked
		{ 
			newDistance();
			newDirection();
			return;
		}
		else
		{		//if the snarlbot is not blocked, then it can (and should) move
			moveTo(getX() + dx, getY() + dy);
			m_distanceBeforeTurning--;
			return;
		}
	}

	//Here, the KleptoBot must choose a new direction to face, since it is supposed to turn
	
	//Also, we must set distanceBeforeTurning again
	newDistance();
	newDirection();
}

void KleptoBot::newDirection()
{
	Direction da[4] = { up, right, down, left };

	int directionsLeft = 4;

	Direction firstChosen;

	bool started = false;

	
	while (directionsLeft > 0)
	{
		int i = rand() % directionsLeft; //Pick a direction
		
		Direction dir = da[i]; 
		if (!started)
		{
			firstChosen = dir;
			started = true;
		}
		
		int dx;
		int dy;

		dirToDelta(dir, dx, dy);


		if (getWorld()->player()->getX() == getX() + dx
			&& getWorld()->player()->getY() == getY() + dy)
		{
			da[i] = da[directionsLeft - 1];
			directionsLeft--;
			continue;
		}
		Actor* ap = getWorld()->findObstruction(getX() + dx, getY() + dy);

		if (ap == nullptr)
		{
			setDirection(dir);
			moveTo(getX() + dx, getY() + dy);
			m_distanceBeforeTurning--;
			return;
		}
		else
		{
			da[i] = da[directionsLeft - 1];
			directionsLeft--;
		}
	}

	//At this point, we know the snarlbot found obstructions in all four directions
	//So, set direction to our first chosen direction

	setDirection(firstChosen);
}

void KleptoBot::die()
{
	getWorld()->increaseScore(10);
	getWorld()->playSound(SOUND_ROBOT_DIE);
	if (holdingGoodie())
	{
		getWorld()->dropGoodie(m_goodieType, getX(), getY());
	}
}

void KleptoBot::pickUp(Goodie* goodie)
{
	AmmoGoodie* ag = dynamic_cast<AmmoGoodie*>(goodie);
	if (ag != nullptr)
	{
		m_goodieType = Level::ammo;
		goodie->setDead();
		return;
	}
	RestoreHealthGoodie* rhg = dynamic_cast<RestoreHealthGoodie*>(goodie);
	if (rhg != nullptr)
	{
		m_goodieType = Level::restore_health;
		goodie->setDead();
		return;
	}
	ExtraLifeGoodie* elg = dynamic_cast<ExtraLifeGoodie*>(goodie);
	if (elg != nullptr)
	{
		m_goodieType = Level::extra_life;
		goodie->setDead();
		return;
	}
}


/////////////////////////////////////////
//   ANGRY KLEPTOBOT IMPLEMENTATIONS   //
/////////////////////////////////////////
void AngryKleptoBot::doSomething()
{
	if (!isAlive())
		return;

	if (ticksToWait() > 1)
	{
		wait();
		return;
	}

	if (playerVisible())
	{
		getWorld()->ShootBullet(getX(), getY(), getDirection());
		getWorld()->playSound(SOUND_ENEMY_FIRE);
		resetTicks();
		return;
	}

	//At this point, an angry kleptoBot should act the same as a normal kleptobot

	KleptoBot::doSomething();
}

void AngryKleptoBot::die()
{
	getWorld()->increaseScore(20);
	getWorld()->playSound(SOUND_ROBOT_DIE);
	if (holdingGoodie())
	{
		getWorld()->dropGoodie(heldGoodie(), getX(), getY());
	}
}


/////////////////////////////////
//   FACTORY IMPLEMENTATIONS   //
/////////////////////////////////
void Factory::doSomething()
{
	if (getWorld()->foundKlepto(getX(), getY()))
	{
		return;			//If the factory has a kleptobot ontop of it, it cant do anything
	}

	if (countKleptoBots() < 3)
	{
		int chance = rand() % 50; //The factory has a 1 in 50 chance of spawning a kleptobot

		if (chance == 0)	//We will 0 to be our signal to spawn a KleptoBot
		{
			getWorld()->spawnKleptoBot(getX(), getY(), m_makesAngry);
		}
	}
}

int Factory::countKleptoBots()
{
	int x = getX();
	int y = getY();

	int count = 0;

	for (int i = -3; i < 4; i++)
	{
		if (x + i < 0 || x + i >= VIEW_WIDTH)		//dont check out of bounds if we dont need to
			continue;
		for (int j = -3; j < 4; j++)
		{
			if (y + j < 0 || y + j >= VIEW_HEIGHT)
				continue;			//dont check out of bounds if we dont need to
			if (getWorld()->foundKlepto(x + i, y + j))
				count++;
		}
	}
	return count;
}


/////////////////////////////////
//   BOULDER IMPLEMENTATIONS   //
/////////////////////////////////
bool Boulder::push(Direction dir)
{
	int dx;
	int dy;

	dirToDelta(dir, dx, dy);

	//find an Actor at the coordinates we are trying to push to
	Actor* ap = getWorld()->getActor(getX() + dx, getY() + dy);

	//If we didnt find an actor, then the space is empty
	//and we can just move the boulder. Return true to notify the 
	//player that the boulder was pushed
	if (ap == nullptr)
	{
		moveTo(getX() + dx, getY() + dy);
		return true;
	}
	else
	{
		//if we found an actor on the coordinates, check if it is a hole
		Hole* hp = dynamic_cast<Hole*>(ap);
		if (hp)
		{
			//if the actor at the push coordinates is a hole, then we are allowed
			//to push the boulder on top of the hole. Again, return true to notify the player
			moveTo(getX() + dx, getY() + dy);
			return true;
		}
	}

	//return false to notify the player that the boulder could not be pushed
	return false;
}


//////////////////////////////
//   HOLE IMPLEMENTATIONS   //
//////////////////////////////
void Hole::doSomething()
{
	if (!isAlive())
		return;
	//look for a boulder on top of the hole
	Actor* bp = getWorld()->findBoulder(getX(), getY());

	if (bp != nullptr)
	{
		//If we found a boulder, kill the hole and the boulder
		setDead();
		bp->setDead();
	}
}


////////////////////////////////
//   GOODIE IMPLEMENTATIONS   //
////////////////////////////////
void Goodie::doSomething()
{
	//Check if the player is on top of the goodie
	if (getWorld()->player()->getX() == getX()
		&& getWorld()->player()->getY() == getY())
	{
		//If the player is ontop of the goodie, kill the goodie and play the correct sound
		setDead();
		getWorld()->playSound(SOUND_GOT_GOODIE);
		rewardPlayer(); //give the player the appropriate rewards for collecting a goodie
	}
}

void Jewel::rewardPlayer()
{
	getWorld()->increaseScore(50);
	getWorld()->getJewel();
}

void ExtraLifeGoodie::rewardPlayer()
{
	getWorld()->incLives();
	getWorld()->increaseScore(1000);
}

void AmmoGoodie::rewardPlayer()
{
	getWorld()->increaseScore(100);
	getWorld()->player()->addAmmo();
}

void RestoreHealthGoodie::rewardPlayer()
{
	getWorld()->increaseScore(500);
	getWorld()->player()->setHitpoints(20);
}


////////////////////////////////
//   BULLET IMPLEMENTATIONS   //
////////////////////////////////
void Bullet::move(Direction dir)
{
	switch (dir)
	{
	case up:
		moveTo(getX(), getY() + 1);
		break;
	case right:
		moveTo(getX() + 1, getY());
		break;
	case down:
		moveTo(getX(), getY() - 1);
		break;
	case left:
		moveTo(getX() - 1, getY());
		break;
	}
}

void Bullet::doSomething()
{
	if (true)
	{
		if (!isAlive())  //if the bullet is dead, we dont want to do anything
			return;

		if ((getWorld()->player()->getX() == getX()			//Check if the bullet is on top
			&& getWorld()->player()->getY() == getY()))		//of the player.
		{
			getWorld()->player()->Hurt();
			setDead();
			return;
		}

		//Find an actor at the bullet's coordinates
		Actor* ap = getWorld()->findDamageable(getX(), getY());

		if (ap != nullptr)
		{
			if (ap->blocksBullet())
			{
				setDead();
				return;
			}
			HealthActor* hap = dynamic_cast<HealthActor*>(ap);

			if (hap != nullptr)
			{
				hap->Hurt();
				setDead();
				return;
			}
		}
	}
	

	//At this point, we know the bullet should move past whatever actor it is on top of
	move(getDirection());

	//Repeat the same algorithm as above to test whether the bullet should stay alive
	//at its new coordinates

	if ((getWorld()->player()->getX() == getX()			//Check if the bullet is on top
		&& getWorld()->player()->getY() == getY()))		//of the player.
	{
		getWorld()->player()->Hurt();
		setDead();
		return;
	}

	//Find an actor at the bullet's coordinates
	Actor *ap = getWorld()->findObstruction(getX(), getY());

	if (ap != nullptr)
	{
		if (ap->blocksBullet())
		{
			setDead();
			return;
		}
		//Check again if the bullet is on top of an actor it can hurt
		HealthActor* hap = dynamic_cast<HealthActor*>(ap);

		if (hap != nullptr)
		{
			hap->Hurt();
			setDead();
			return;
		}
	}
}


//////////////////////////////
//   EXIT IMPLEMENTATIONS   //
//////////////////////////////
void Exit::doSomething()
{
	if (!isAlive())
		return;
	if (m_isVisible)
	{
		if (getWorld()->player()->getX() == getX() && getWorld()->player()->getY() == getY())
		{
			setDead();
			getWorld()->increaseScore(2000);
			getWorld()->playSound(SOUND_FINISHED_LEVEL);
			getWorld()->completeLevel();
		}
	}
	else if (getWorld()->jewelsLeft() == false)
	{
		getWorld()->playSound(SOUND_REVEAL_EXIT);
		m_isVisible = true;
		setVisible(true);
	}
}
		