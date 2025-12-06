#pragma once
#include "ofMain.h"

// A sector is a walkable area of the map defined by four limits.
class Sector : public ofBaseApp
{
public:
	float maxX, maxZ, minX, minZ;
	Sector(float _maxX, float _maxZ, float _minX, float _minZ);
};