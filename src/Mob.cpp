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
		defaultSpeed = 0.0025f;
	}
	else
	{
		health = 6.0f;
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

			// Stop mob from clipping into player
			float distToPlayer = (pow(player->getPosition().x - pos.x, 2) + pow(player->getPosition().z - pos.z, 2));
			if (distToPlayer <= 5.0f)
			{
				currentSpeed = 0;
			}
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
}

void Mob::beAttacked()
{

}