#pragma once
#include "Particle.h"

class Block : public Particle {

public:

	Block() {
		dead = false;
		mass = massBase / r;
		velocity.set(0, 0);
		acceleration.set(0.0, 0.00);
		location = ofVec2f(0, 0);
		home_location = location;

		//limit the magnitude of a vector
		velocity.limit(speedLimit);
	};

	Block(ofVec2f  _location, int _r) {
		dead = false;
		r = _r;
		mass = massBase / r;
		velocity.set(0, 0);
		acceleration.set(0.0, 0.00);
		location = _location;
		home_location = location;

		//limit the magnitude of a vector
		velocity.limit(speedLimit);
	}

	static float seekLimit, speedLimit, massBase;
	static ofColor color;
	static int r;

private: 

};

float Block::seekLimit = 10.0;
float Block::speedLimit = 5.0;
float Block::massBase = 10.0;
ofColor Block::color = ofColor(217, 217, 0, 100);
int Block::r = 5;