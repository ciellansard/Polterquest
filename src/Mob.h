#pragma once
#include "ofMain.h"
#include "ofxAssimpModelLoader.h"

class Mob : public ofBaseApp
{
public: 
	int type;
	ofVec3f pos;
	float rot, health, defaultSpeed, currentSpeed;
	int state, flipFactor;
	std::string texPath;
	ofTexture tex;
	ofMesh mesh;
	bool isAwareOfPlayer;
	float lastTimeAwareOfPlayer;
	ofEasyCam* player;
	//Sector sector;

	Mob(int _type, int _loot, ofVec3f _pos, float _rot, ofEasyCam *_player);
	//~Mob();
	void update();
	void draw();
	void chase();
	void beAttacked();
};