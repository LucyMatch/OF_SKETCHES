#pragma once
#include "Particle.h"

class Block : public Particle {

public:

	Block();
	Block(ofVec2f  _location);
	virtual void draw();
	virtual void update();
	virtual void seek(ofVec2f& target);
	virtual void arrive(ofVec2f& target);

	static ofParameter<float> speed_limit, mass_base, trail_wgt, seek_limit;
	static ofParameter<ofColor> pcolor, tcolor;
	static ofParameter<int> r, arrive_cap, history_length;
	static ofParameter<bool> trail, arrive_on, seek_on;

private: 

};
