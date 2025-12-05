#include "Mob.h"

enum states
{
	calm,
	hurt,
	angry,
	suck
};

// Player _player
Mob::Mob(int _type, ofVec3f _pos, float _rot)
{
	type = _type;
	pos = _pos;
	rot = _rot;

	if (type == 0)
	{
		health = 5.0f;
		defaultSpeed = 0.002f;
	}
	else
	{
		health = 20.0f;
		defaultSpeed = 0.001f;
	}
	
	currentSpeed = defaultSpeed;

	state = 0;
	texPath = "images/mobs/mob" + ofToString(type) + ofToString(state) + ".png";
	ofLoadImage(tex, texPath);

	ofxAssimpModelLoader loader;
	loader.loadModel("models/mob" + ofToString(type) + ".obj");
	mesh = loader.getMesh(0);

	flipFactor = 1;

	isAwareOfPlayer = false;
	lastTimeAwareOfPlayer = NULL;
}

void Mob::update()
{
	switch (state)
	{
		case calm:
		{
			currentSpeed = defaultSpeed;
			rot += 0.1f;
			break;
		}
		case hurt:
		{
			currentSpeed = 0;
			break;
		}
		case angry:
		{
			currentSpeed = defaultSpeed * 3.0f;
			// rotate to face player
			break;
		}
		case suck:
		{
			currentSpeed = 0;
			break;
		}
		default: break;
	}

	// Get texture for current state
	if (texPath != "images/mobs/mob" + ofToString(type) + ofToString(state) + ".png") texPath = "images/mobs/mob" + ofToString(type) + ofToString(state) + ".png";
	ofLoadImage(tex, texPath);

	// Movement
	pos.y = 0.1f * (sin(ofGetElapsedTimeMillis() * currentSpeed) + 1.0f);
	pos.x += cos(rot) * currentSpeed;
	pos.z += sin(rot) * currentSpeed;

	((sin(ofGetElapsedTimeMillis() * currentSpeed) == 0)) ? flipFactor = 3 : flipFactor = 1;
}

void Mob::draw()
{
	ofPushMatrix();
	{
		ofTranslate(pos.x, pos.y, pos.z);
		ofRotateYDeg(rot);
		ofScale(1, flipFactor, 1);
		tex.bind();
		{
			mesh.draw();
		}
		tex.unbind();
	}
	ofPopMatrix();
	
}

void Mob::chase()
{

}

void Mob::beAttacked()
{

}