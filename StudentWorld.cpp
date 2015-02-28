#include "StudentWorld.h"
#include "Level.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <list>
#include <sstream>
#include <iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

int StudentWorld::loadAlevel()
{
	//turn the int level into a string of form "level###.dat"
	
	ostringstream oss;
	unsigned int level = getLevel();

	oss.fill('0');
	oss << "level" << setw(2) << level << ".dat";
	string levelFile = oss.str();

	Level lev = Level(assetDirectory());

	Level::LoadResult result = lev.loadLevel(levelFile);

	if (result == Level::load_fail_file_not_found)
		return result;
	else if (result == Level::load_fail_bad_format)
		return result;
	else if (result == Level::load_success)
	{
		for (int i = 0; i < VIEW_WIDTH; i++)
		{
			for (int j = 0; j < VIEW_HEIGHT; j++)
			{
				Level::MazeEntry item = lev.getContentsOf(i, j);
				switch (item)
				{
				case Level::player:
					m_player = new Player(i, j, this);
					break;
				case Level::wall:
					m_ActorList.push_back(new Wall(i, j, this));
					break;
				case Level::boulder:
					m_ActorList.push_back(new Boulder(i, j, this));
					break;
				case Level::hole:
					m_ActorList.push_back(new Hole(i, j, this));
					break;
				case Level::jewel:
					m_ActorList.push_back(new Jewel(i, j, this));
					m_jewels++;
					break;
				case Level::extra_life:
					m_ActorList.push_back(new ExtraLifeGoodie(i, j, this));
					break;
				case Level::ammo:
					m_ActorList.push_back(new AmmoGoodie(i, j, this));
					break;
				case Level::restore_health:
					m_ActorList.push_back(new RestoreHealthGoodie(i, j, this));
					break;
				case Level::exit:
					m_ActorList.push_back(new Exit(i, j, this));
					break;
				case Level::vert_snarlbot:
					m_ActorList.push_back(new SnarlBot(i, j, this, GraphObject::down));
					break;
				case Level::horiz_snarlbot:
					m_ActorList.push_back(new SnarlBot(i, j, this, GraphObject::right));
					break;
				case Level::angry_kleptobot_factory:
					m_ActorList.push_back(new Factory(i, j, this, true));
					break;
				case Level::kleptobot_factory:
					m_ActorList.push_back(new Factory(i, j, this, false));
					break;
				}
			}
		}
		return result;
	}
}

void StudentWorld::setDisplayText()
{
	//get the data required for the display text
	unsigned int score = getScore();
	unsigned int level = getLevel();
	unsigned int nLives = getLives();
	int bonus = getBonus();
	int hitPoints = player()->getHitpoints();
	int ammo = player()->getAmmunition();
	int percentage = hitPoints * 5;

	//create the display text
	ostringstream oss;

	oss << "Score: ";
	oss.fill('0');
	oss << setw(7) << score << "  "
		<< "Level: "
		<< setw(2) << level << "  "
		<< "Lives: ";
	oss.fill(' ');
	oss << nLives << "  "
		<< "Health: "
		<< setw(3) << percentage << "%  "
		<< "Ammo: " << ammo << "  "
		<< "Bonus: "
		<< setw(4) << bonus;

	string displayText = oss.str();

	setGameStatText(displayText);
}

int StudentWorld::init()
{
	//Check to see if we are trying to initialize level 100, in which case the player won
	if (getLevel() == 100)
		return GWSTATUS_PLAYER_WON;

	m_jewels = 0;

	int result = loadAlevel(); //load the next level and store the result  of the load

	if (result == Level::load_fail_bad_format)
		return GWSTATUS_LEVEL_ERROR;
	if (result == Level::load_fail_file_not_found)
		return GWSTATUS_PLAYER_WON;		//if we couldnt find the next level, then the player won

	m_levelComplete = false;
	m_bonus = 1000;
	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	//update the display text
	setDisplayText();

	//Ask the player to do something
	unsigned int livesBefore = getLives();
	player()->doSomething();

	//check if the player completed the level

	for (auto ap : m_ActorList)
	{
		if (ap->isAlive())
		{
			ap->doSomething();

			if (getLives() < livesBefore)
			{
				return GWSTATUS_PLAYER_DIED;
			}

			if (m_levelComplete)
			{
				increaseScore(getBonus());
				return GWSTATUS_FINISHED_LEVEL;
			}

		}
	}

	removeDeadActors();

	decBonus();

	//check if the player died during this tick
	if (getLives() < livesBefore)
		return GWSTATUS_PLAYER_DIED;

	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	//delete all the actor pointers in the actor list
	delete m_player;
	auto ap = m_ActorList.begin();
	while (ap != m_ActorList.end())
	{
		delete *ap;
		ap = m_ActorList.erase(ap);
	}
}

void StudentWorld::removeDeadActors()
{
	for (auto ap = m_ActorList.begin(); ap != m_ActorList.end();)
	{
		if (!(*ap)->isAlive())//If an actor is not alive, then we want to remove it
		{
			delete (*ap);
			ap = m_ActorList.erase(ap);
		}
		else
			ap++;
	}
}

void StudentWorld::ShootBullet(int x, int y, GraphObject::Direction dir)
{
	//The actor that shoots the bullet will pass its current Coordinates and direction
	//We will deal with which coordinate the bullet will spawn on based on these parameters
	switch (dir)
	{
	case GraphObject::up:
		m_ActorList.push_back(new Bullet(x, y + 1, this, dir));
		break;
	case GraphObject::right:
		m_ActorList.push_back(new Bullet(x + 1, y, this, dir));
		break;
	case GraphObject::down:
		m_ActorList.push_back(new Bullet(x, y - 1, this, dir));
		break;
	case GraphObject::left:
		m_ActorList.push_back(new Bullet(x - 1, y, this, dir));
		break;
	}
}

void StudentWorld::spawnKleptoBot(int x, int y, bool isAngry)
{
	if (isAngry)
	{
		m_ActorList.push_back(new AngryKleptoBot(IID_ANGRY_KLEPTOBOT, x, y, this));
	}
	else
		m_ActorList.push_back(new KleptoBot(IID_KLEPTOBOT, x, y, this));
	playSound(SOUND_ROBOT_BORN);
}

Actor* StudentWorld::FindNOTBullet(int x, int y)
{
	for (auto ap : m_ActorList)
	{
		if (ap->getX() == x && ap->getY() == y)
		{
			Bullet* bp = dynamic_cast<Bullet*>(ap);
			if (bp != nullptr)
				continue;
			else
				return ap;
		}
	}
	return nullptr;
}

Actor* StudentWorld::findObstruction(int x, int y)
{
	for (auto ap : m_ActorList)
	{
		if (ap->getX() == x && ap->getY() == y)
		{
			if (ap->isAlive() && ap->blocksRobot())
			{
				return ap;
			}
		}
	}
	return nullptr;
}

Actor* StudentWorld::getActor(int x, int y)
{
	for (auto ap : m_ActorList)
	{
		if (ap->getX() == x && ap->getY() == y)
		{
			if (ap->isAlive())
			{
				return ap;
			}
		}
	}
	return nullptr;
}

bool StudentWorld::foundKlepto(int x, int y)
{
	for (auto ap : m_ActorList)
	{
		if (ap->getX() == x && ap->getY() == y)
		{
			if (ap->isAlive())
			{
				KleptoBot* kbp = dynamic_cast<KleptoBot*>(ap);
				if (kbp != nullptr)
				{
					return true;
				}
			}
		}
	}
	return false;
}

Boulder* StudentWorld::findBoulder(int x, int y)
{
	for (auto ap : m_ActorList)
	{
		if (ap->getX() == x && ap->getY() == y)
		{
			if (ap->isAlive())
			{
				Boulder* bp = dynamic_cast<Boulder*>(ap);

				if (bp != nullptr)
				{
					return bp;
				}
			}
		}
	}
	return nullptr;
}

Actor* StudentWorld::findDamageable(int x, int y)
{
	for (auto ap : m_ActorList)
	{
		if (ap->isDamageable() && ap->getX() == x && ap->getY() == y)
		{
			if (ap->isAlive())
			{
				return ap;
			}
		}
	}

	return findObstruction(x, y);
}

void StudentWorld::dropGoodie(Level::MazeEntry goodie, int x, int y)
{
	switch (goodie)
	{
	case Level::extra_life:
		m_ActorList.push_back(new ExtraLifeGoodie(x, y, this));
		break;
	case Level::ammo:
		m_ActorList.push_back(new AmmoGoodie(x, y, this));
		break;
	case Level::jewel:
		m_ActorList.push_back(new Jewel(x, y, this));
		break;
	case Level::restore_health:
		m_ActorList.push_back(new RestoreHealthGoodie(x, y, this));
		break;
	}

}
// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
