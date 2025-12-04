#pragma once
#include "ofApp.h"

enum sectorType
{
	room,
	hall
};

// A sector is a walkable area of the map defined by four corner coordinates.
class Sector
{
public:
	ofVec2f* getCorners();

protected:
	ofVec2f corners[4];
};