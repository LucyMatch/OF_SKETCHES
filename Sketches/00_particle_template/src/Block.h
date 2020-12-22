#pragma once
#include "Particle.h"

class Block : public Particle {

public:

	Block();
	Block(ofVec2f  _location);
	virtual void draw();
	virtual void update();
	static float seekLimit, speedLimit, massBase;
	static ofColor color;
	static int r;

private: 

};
