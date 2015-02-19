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
			//bullet stuff
			break;
		case KEY_PRESS_ESCAPE:
			getWorld()->decLives();
			break;

		}
	}
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
			
			
			
			
			/*(getWorld()->whatTypeIsThis(ap) == Level::empty
			|| getWorld()->whatTypeIsThis(ap) == Level::hole)
		{
			moveTo(getX(), getY() + 1);
			return true;
		}
		return false;*/
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
	//If the hole is dead, clearly we dont want to do anything
	if (!isAlive())
		return;

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
	if (!isAlive())
		return false;
	else
	{
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
	}
}


void Jewel::doSomething()
{
	if (doSomethingGoodie()) //The player got the goodie
	{
		getWorld()->increaseScore(50);
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