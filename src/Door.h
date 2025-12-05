#pragma once
#include "ofMain.h"
#include "ofxAssimpModelLoader.h"

class Door : public ofBaseApp
{
public:
	ofVec3f pos;
	float rot;
	bool isClosed, isLocked;
	ofMesh mesh;


	Door(ofVec3f _pos, float _rot, bool _isLocked);
	void unlock(bool* _key);
	void draw();
};