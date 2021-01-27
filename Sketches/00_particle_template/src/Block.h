#pragma once
#include "Particle.h"

class Block : public Particle {

public:

	Block();
	Block(ofVec2f  _location);
	virtual void draw();
	virtual void update();
	static float seekLimit, speedLimit, massBase, trail_wgt;
	static ofColor pcolor, tcolor;
	static int r, arrive_cap, history_length;
	static bool trail, arrive_on, seek_on;

private: 

};
