#include "Mob.h"

std::string type;
ofVec3f pos, rot;
float speed;
int state;
bool isAwareOfPlayer;
float lastTimeAwareOfPlayer;

// Player _player
Mob::Mob(int _type, ofVec3f _pos, ofVec3f _rot)
{
	type = _type;
	pos = _pos;
	rot = _rot;

	if (type == 0) speed = 0.002f;
	else speed = 0.001f;
	
	state = 0;
	isAwareOfPlayer = false;
	lastTimeAwareOfPlayer = NULL;
}

void Mob::update()
{

}

void Mob::draw()
{

}

void Mob::chase()
{

}

void Mob::beAttacked()
{

}