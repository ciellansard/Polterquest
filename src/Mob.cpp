#include "Mob.h"

enum states
{
	calm,
	hurt,
	angry,
	suck
};

// Player _player
Mob::Mob(int _type, ofVec3f _pos, float _rot, ofEasyCam *_player)
{
	type = _type;
	pos = _pos;
	rot = _rot;
	player = _player;

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
			rot = atan((player->getPosition().z - pos.z) / (player->getPosition().x - pos.x));
			//printf("%f\n", rot);
			break;
		}
		case suck:
		{
			currentSpeed = 0;
			break;
		}
		default: break;
	}

	//printf("%f, %f, %f\n", player->getPosition().x, player->getPosition().y, player->getPosition().z);

	// Get texture for current state
	if (texPath != "images/mobs/mob" + ofToString(type) + ofToString(state) + ".png") texPath = "images/mobs/mob" + ofToString(type) + ofToString(state) + ".png";
	ofLoadImage(tex, texPath);

	// Movement
	pos.y = 0.1f * (sin(ofGetElapsedTimeMillis() * -currentSpeed) + 1.0f);
	pos.x += cos(rot) * -currentSpeed;
	pos.z += sin(rot) * -currentSpeed;

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

}

void Mob::beAttacked()
{

}