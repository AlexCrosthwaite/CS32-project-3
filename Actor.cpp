#include "Actor.h"
#include "StudentWorld.h"
#include "GraphObject.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

void Player::doSomething()
{
	int key;
	if (getWorld()->getKey(key))
	{
		//a key was pressed this tick
		Actor* ap;
		switch (key)
		{
		case KEY_PRESS_LEFT:
			//change which way the player faces
			setDirection(left);

			ap = getWorld()->getActor(getX() - 1, getY());

			if (!ap) //this means the spot is empty
			{
				moveTo(getX() - 1, getY());
			}

			else if (!(ap->blocksPlayer()))
			{
				Boulder* bp = dynamic_cast<Boulder*> (ap);

				if (bp == nullptr) // Here, we know the player can walk directly onto the other actor.
				{				   //We know if this check fails, then the player tried to walk onto a boulder
					moveTo(getX() - 1, getY());
				}
				else if (bp->push(left) == true) //check to see if we can move the boulder
				{
					moveTo(getX() - 1, getY());
				}
			}
			break;
		case KEY_PRESS_DOWN:
			//change which way the player faces
			setDirection(down);

			ap = getWorld()->getActor(getX(), getY() - 1);

			if (!ap) //this means the spot is empty
			{
				moveTo(getX(), getY() - 1);
			}

			else if (!(ap->blocksPlayer()))
			{
				Boulder* bp = dynamic_cast<Boulder*> (ap);

				if (bp == nullptr) // Here, we know the player can walk directly onto the other actor.
				{				   //We know if this check fails, then the player tried to walk onto a boulder
					moveTo(getX(), getY() - 1);
				}

				else if (bp->push(down) == true) //check to see if we can move the boulder
				{
					moveTo(getX(), getY() - 1);
				}
			}
			break;
		case KEY_PRESS_RIGHT:
			//change which way the player faces
			setDirection(right);

			ap = getWorld()->getActor(getX() + 1, getY());

			if (!ap) //this means the spot is empty
			{
				moveTo(getX() + 1, getY());
			}

			else if (!(ap->blocksPlayer()))
			{
				Boulder* bp = dynamic_cast<Boulder*> (ap);

				if (bp == nullptr) // Here, we know the player can walk directly onto the other actor.
				{				   //We know if this check fails, then the player tried to walk onto a boulder
					moveTo(getX() + 1, getY());
				}
				else if (bp->push(right) == true) //check to see if we can move the boulder
				{
					moveTo(getX() + 1, getY());
				}
			}
			break;
		case KEY_PRESS_UP:
			//change which way the play faces
			setDirection(up);

			ap = getWorld()->getActor(getX(), getY() + 1);

			if (!ap) //this means the spot is empty
			{
				moveTo(getX(), getY() + 1);
			}

			else if (!(ap->blocksPlayer()))
			{
				Boulder* bp = dynamic_cast<Boulder*> (ap);

				if (bp == nullptr) //Here, we know the player can walk directly onto the other actor.
				{				   //We know if this check fails, then the player tried to walk onto a boulder
					moveTo(getX(), getY() + 1);
				}

				else if (bp->push(up) == true) //check to see if we can move the boulder
				{
					moveTo(getX(), getY() + 1);
				}
			}
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

	Actor* ap = getWorld()->getActor(getX() + dx, getY() + dy);

	if (!ap) //this means the spot is empty
	{
		moveTo(getX() + dx, getY() + dy);
	}

	else if (!(ap->blocksPlayer()))
	{
		Boulder* bp = dynamic_cast<Boulder*> (ap);

		if (bp == nullptr) // Here, we know the player can walk directly onto the other actor.
		{				   //We know if this check fails, then the player tried to walk onto a boulder
			moveTo(getX() + dx, getY() + dy);
		}

		else if (bp->push(down) == true) //check to see if we can move the boulder
		{
			moveTo(getX() + dx, getY() + dy);
		}
	}
}

void Player::Hurt()
{
	setHitpoints(getHitpoints() - 2);

	if (getHitpoints() <= 0)
	{
		setDead();
		getWorld()->playSound(SOUND_PLAYER_DIE);
	}
	else
		getWorld()->playSound(SOUND_PLAYER_IMPACT);
}

void Shooter::shoot()
{
	getWorld()->ShootBullet(getX(), getY(), getDirection());
}

void Robot::resetTicks()
{
	int ticks = (28 - getWorld()->getLevel()) / 4;

	if (ticks < 3)
		m_ticksToWait = 3;
	else
		m_ticksToWait = ticks;
}

bool Robot::playerVisible()
{
	return true;
}

void SnarlBot::doSomething()
{
	//if (playerVisible())
}
bool Boulder::push(Direction dir)
{
	if (dir == up)
	{
		//find an Actor at the coordinates we are trying to push to
		Actor* ap = getWorld()->getActor(getX(), getY() + 1);

		//If we didnt find an actor, then the space is empty
		//and we can just move the boulder. Return true to notify the 
		//player that the boulder was pushed
		if (ap == nullptr)
		{
			moveTo(getX(), getY() + 1);
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
				moveTo(getX(), getY() + 1);
				return true;
			}
		}

		//return false to notify the player that the boulder could not be pushed
		return false;
	}
	else if (dir == right)
	{
		Actor* ap = getWorld()->getActor(getX() + 1, getY());

		if (ap == nullptr)
		{
			moveTo(getX() + 1, getY());
			return true;
		}
		else
		{
			Hole* hp = dynamic_cast<Hole*>(ap);
			if (hp)
			{
				moveTo(getX() + 1, getY());
				return true;
			}
		}
		return false;
	}
	else if (dir == down)
	{
		Actor* ap = getWorld()->getActor(getX(), getY() - 1);

		if (ap == nullptr)
		{
			moveTo(getX(), getY() - 1);
			return true;
		}
		else
		{
			Hole* hp = dynamic_cast<Hole*>(ap);
			if (hp)
			{
				moveTo(getX(), getY() - 1);
				return true;
			}
		}
		return false;
	}
	else // (dir == left)
	{
		Actor* ap = getWorld()->getActor(getX()  - 1, getY());

		if (ap == nullptr)
		{
			moveTo(getX() - 1, getY());
			return true;
		}
		else
		{
			Hole* hp = dynamic_cast<Hole*>(ap);
			if (hp)
			{
				moveTo(getX() - 1, getY());
				return true;
			}
		}
		return false;
	}
}

 
void Hole::doSomething()
{
	//look for a boulder on top of the hole
	Boulder* bp = getWorld()->findBoulder(getX(), getY());

	if (bp != nullptr)
	{
		//If we found a boulder, kill the hole and the boulder
		setDead();
		bp->setDead();
	}
}

bool Goodie::doSomethingGoodie()
{
	/*if (!isAlive())
		return false;
	else
	{*/
		//Check if the player is on top of the goodie
		if (getWorld()->player()->getX() == getX()
			&& getWorld()->player()->getY() == getY())
		{
			//If the player is ontop of the goodie, kill the goodie and play the correct sound
			setDead();
			getWorld()->playSound(SOUND_GOT_GOODIE);
			return true;
		}
		return false;
	//}
}


void Jewel::doSomething()
{
	if (doSomethingGoodie()) //The player got the goodie
	{
		getWorld()->increaseScore(50);
		getWorld()->getJewel();
	}
}

void ExtraLifeGoodie::doSomething()
{
	if (doSomethingGoodie()) //The player got the goodie
	{
		getWorld()->incLives();
		getWorld()->increaseScore(1000);
	}
}

void AmmoGoodie::doSomething()
{
	if (doSomethingGoodie()) //The player got the goodie
	{
		getWorld()->increaseScore(100);
		getWorld()->player()->addAmmo();
	}
}

void RestoreHealthGoodie::doSomething()
{
	if (doSomethingGoodie()) //The player got the goodie
	{
		getWorld()->increaseScore(500);
		getWorld()->player()->setHitpoints(20);
	}
}

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

//void Buller::checkCurrentCoordinates()
void Bullet::doSomething()
{
	if (!m_justSpawned)
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
		Actor* ap = getWorld()->FindNOTBullet(getX(), getY());

		if (ap != nullptr)
		{
			//Check if the bullet is on top of an actor that it can hurt
			//and act accordingly
			HealthActor* hap = dynamic_cast<HealthActor*>(ap);

			if (hap != nullptr)
			{
				hap->Hurt();
				setDead();
				return;
			}

			//check if the bullet is on top of an actor that it cannot hurt
			//and act accordingly
			Wall* wp = dynamic_cast<Wall*>(ap);

			if (wp != nullptr)
			{
				setDead();
				return;
			}

			//TODO: add checks for Actors that a bullet can be on top of
		}


		//At this point, we know the bullet should move past whatever actor it is on top of
		move(getDirection());
	}
	m_justSpawned = false;
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
	Actor* ap = getWorld()->FindNOTBullet(getX(), getY());


	if (ap != nullptr)
	{
		//Check again if the bullet is on top of an actor it can hurt
		HealthActor* hap = dynamic_cast<HealthActor*>(ap);

		if (hap != nullptr)
		{
			hap->Hurt();
			setDead();
			return;
		}

		//check if the bullet is on top of an actor it cant hurt
		Wall* wp = dynamic_cast<Wall*>(ap);

		if (wp != nullptr)
		{
			setDead();
			return;
		}
		//TODO: add checks for Actors that a bullet can be on top of
	}
}

void Exit::doSomething()
{
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
	else
	{
		if (!(getWorld()->jewelsLeft()))
		{
			getWorld()->playSound(SOUND_REVEAL_EXIT);
			m_isVisible = true;
			setVisible(true);
		}
	}
}
		