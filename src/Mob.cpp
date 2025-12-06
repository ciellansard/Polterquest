#include "Mob.h"

enum states
{
	calm,
	hurt,
	angry,
	suck
};

// Player _player
Mob::Mob(int _type, int _loot, ofVec3f _pos, float _rot, ofEasyCam *_player)
{
	type = _type;
	pos = _pos;
	rot = _rot;
	player = _player;

	if (type == 0)
	{
		health = 2.0f;
		defaultSpeed = 0.005f;
	}
	else
	{
		health = 10.0f;
		defaultSpeed = 0.002f;
	}
	
	currentSpeed = defaultSpeed;

	state = angry;
	texPath = "images/mobs/mob" + ofToString(type) + ofToString(state) + ".png";
	ofLoadImage(tex, texPath);

	ofxAssimpModelLoader loader;
	loader.loadModel("models/mob" + ofToString(type) + ".obj");
	mesh = loader.getMesh(0);

	flipFactor = 1;

	isAwareOfPlayer = false;
	lastTimeAwareOfPlayer = NULL;
}
/*
Mob::~Mob()
{
	delete player;
}
*/
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
			chase();
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

	// Bob up and down. 0.1f is the amplitude, currentSpeed/5.0f controls the frequency, and +1 ensures that ghost won't clip
	// through the floor at the bottom of the cycle
	pos.y = 0.1f * (sin(ofGetElapsedTimeMillis() * currentSpeed / 5.0f) + 1.0f);

	//((sin(ofGetElapsedTimeMillis() * currentSpeed) == 0)) ? flipFactor = 3 : flipFactor = 1;
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
	// Mob approaches player, moving a maximum of currentSpeed units per execution.
	float deltaX = pos.x - player->getPosition().x;
	float deltaZ = pos.z - player->getPosition().z;
	float theta = atan2f(deltaZ, deltaX);
	float newX = cos(theta) * currentSpeed;
	float newZ = sin(theta) * currentSpeed;

	pos.set(pos.x - newX, pos.y, pos.z - newZ);
	printf("(%f, %f, %f)\n", pos.x, pos.y, pos.z);
}

void Mob::beAttacked()
{

}