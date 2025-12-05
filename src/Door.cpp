#include "Door.h"

Door::Door(ofVec3f _pos, float _rot, bool _isLocked)
{
	pos = _pos;
	rot = _rot;
	isClosed = true;
	isLocked = _isLocked;

	ofxAssimpModelLoader loader;
	loader.loadModel("models/door" + ofToString(isLocked) + ".obj");
	mesh = loader.getMesh(0);
	ofMesh mesh;
}

void Door::unlock(bool* _key)
{
	_key = false; // Player uses up their key and no longer has one.
	//delete this;
}

void Door::draw()
{
	ofPushMatrix();
	{
		ofTranslate(pos.x, pos.y, pos.z);
		ofRotateYDeg(rot);
		mesh.draw();
	}
	ofPopMatrix();
}