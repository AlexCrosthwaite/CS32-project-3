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

				if (bp == nullptr) // Here, we know the player can walk directly onto the other actor.
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
		Actor* ap = getWorld()->getActor(getX(), getY() + 1);

		if (getWorld()->whatTypeIsThis(ap) == Level::empty
			|| getWorld()->whatTypeIsThis(ap) == Level::hole)
		{
			moveTo(getX(), getY() + 1);
			return true;
		}
		return false;
	}
	else if (dir == right)
	{
		Actor* ap = getWorld()->getActor(getX() + 1, getY());

		if (getWorld()->whatTypeIsThis(ap) == Level::empty
			|| getWorld()->whatTypeIsThis(ap) == Level::hole)
		{
			moveTo(getX() + 1, getY());
			return true;
		}
		return false;
	}
	else if (dir == down)
	{
		Actor* ap = getWorld()->getActor(getX(), getY() - 1);

		if (getWorld()->whatTypeIsThis(ap) == Level::empty
			|| getWorld()->whatTypeIsThis(ap) == Level::hole)
		{
			moveTo(getX(), getY() - 1);
			return true;
		}
		return false;
	}
	else // (dir == left)
	{
		Actor* ap = getWorld()->getActor(getX()  - 1, getY());

		if (getWorld()->whatTypeIsThis(ap) == Level::empty
			|| getWorld()->whatTypeIsThis(ap) == Level::hole)
		{
			moveTo(getX() - 1, getY());
			return true;
		}
		return false;
	}
}
void Hole::doSomething()
{
	if (!isAlive())
		return;
	{

	}
}