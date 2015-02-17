#include "StudentWorld.h"
#include "Level.h"
#include "GameConstants.h"
#include <string>
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
		cout << "Could not find file\n";
	else if (result == Level::load_fail_bad_format)
		cout << "Level was improperly formatted\n";
	else if (result == Level::load_success)
	{
		for (int i = 0; i < VIEW_HEIGHT; i++)
		{
			for (int j = 0; j < VIEW_WIDTH; j++)
			{
				Level::MazeEntry item = lev.getContentsOf(j, i);
				if (item == Level::wall || item == Level::player)
					m_maze[j][i] = item;
				else if (item == Level::boulder || item == Level::hole)
					m_maze[j][i] = item;
				else
					m_maze[j][i] = Level::empty;
			
			}
		}
	}

	return Level::load_success;
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
	if (loadAlevel() == -1)
		exit(1); //level loaded incorrectly or had bad format

	//our maze data member now contains the data for the level
	for (int i = 0; i < VIEW_HEIGHT; i++)
	{
		for (int j = 0; j < VIEW_WIDTH; j++)
		{
			Level::MazeEntry me = m_maze[j][i];
			switch (me)
			{
			case Level::empty:	//do nothing
				break;
			case Level::wall:
				m_ActorList.push_back(new Wall(j, i, this));
				break;
			case Level::boulder:
				m_ActorList.push_back(new Boulder(j, i, this));
				break;
			case Level::hole:
				m_ActorList.push_back(new Hole(j, i, this));
				break;
			case Level::player:
				m_player = new Player(j, i, this);
				break;
			}
		}
	}
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
				return GWSTATUS_PLAYER_DIED;

		}
	}

	decBonus();

	//check if the player died during this tick
	if (getLives() < livesBefore)
		return GWSTATUS_PLAYER_DIED;



	// This code is here merely to allow the game to build, run, and terminate after hitting enter a few times 
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	delete m_player;
	std::list<Actor*>::iterator ap = m_ActorList.begin();
	while (ap != m_ActorList.end())
	{
		delete *ap;
		ap = m_ActorList.erase(ap);
	}
}


Level::MazeEntry StudentWorld::whatTypeIsThis(Actor* ap)
{
	if (ap == nullptr)
		return Level::empty;
	Wall* wp = dynamic_cast<Wall*>(ap);
	if (wp != nullptr)
		return Level::wall;
	Boulder* bp = dynamic_cast<Boulder*>(ap);
	if (bp != nullptr)
		return Level::boulder;
	Hole* hp = dynamic_cast<Hole*>(ap);
	if (hp != nullptr)
		return Level::hole;;

	//ADD MORE HERE AS YOU ADD ACTORS

}
// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
