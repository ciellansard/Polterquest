#pragma once
#include "ofApp.h"

class Mob : public ofBaseApp
{
public: 
	int type;
	ofVec3f pos, rot;
	float speed;
	int state;
	bool isAwareOfPlayer;
	float lastTimeAwareOfPlayer;

	// Player _player
	Mob(int _type, ofVec3f _pos, ofVec3f _rot);
	void update();
	void draw();
	void chase();
	void beAttacked();
};