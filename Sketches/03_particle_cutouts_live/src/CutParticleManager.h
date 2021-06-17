#pragma once

#include "ParticleManager.h"
#include "CutParticle.h"

//A PARTICLE MAN PER CUT?
// -- going this way for now
// may need to rethink if we want lots of particles 
// there may be a more efficient way

//OR JUST PARTICLE MAN WITH A P FOR EVERY CUT... 
//this would work if we only wanted one particle per cut - but that is not the case

//@TODO:
/*
	- you could add dims back in to have an inner square where particles are? rather than entire screen?

*/


class CutParticleManager : public ParticleManager {

public:
	CutParticleManager() { 
		orig_location = ofVec2f(ofRandom(0, ofGetWidth()), ofRandom(0, ofGetHeight()));
		setLocation(orig_location);
	};

	CutParticleManager( ofVec2f loc) { 
		orig_location = loc;
		setLocation(loc);
	};

	virtual void update() {
		for (int i = 0; i < p.size(); i++) {
			p[i].update();

			if (enable_bounce)
				p[i].checkEdges(glm::vec4(0, 0, ofGetWidth(), ofGetHeight()), false);

			if (enable_kill)
				p[i].checkEdges(glm::vec4(0, 0, ofGetWidth(), ofGetHeight()), true);

			if (p[i].dead)
				p.erase(p.begin() + i);
		}
	}

	virtual void update(ofTexture* _img) {
		frame = _img;
		//do we want to do this seperate from above?
		for(auto& _p : p)
			_p.updateFrame(frame);
	}

	virtual void draw() {
		for (auto& _p : p)
			_p.draw();
	}

	virtual void spawn() {
		spawn(curr_location);
	}

	virtual void spawn(ofVec2f loc) {
		CutParticle _p(loc, frame);
		p.push_back(_p);
	}
	
	virtual void applyVaryingGravity(float min, float max, int direction) {
		for (auto &_p : p) {
			ofVec2f gravity;
			if (direction == 0)gravity.set(ofRandom(min, max), 0);
			if (direction == 1)gravity.set(0, ofRandom(min, max));
			if (direction == 2)gravity.set(-ofRandom(min, max), 0);
			if (direction == 3)gravity.set(0, -ofRandom(min, max));
			_p.applyforce(gravity);
		}
	}

	void setLocation(ofVec2f loc) {
		curr_location = loc;
	}

	void randomSpawn() {
		spawn(ofVec2f(ofRandom(0, ofGetWidth()), ofRandom(0, ofGetWidth())));
	}

	void drawDebug(){}

	void clear() {
		p.clear();
	}

	void initGui() {
		gui.setName("particle manager");
		gui.add(enable_limit.set("enable limit", true));
		gui.add(limit.set("limit amt", 100, 0, 500));
		gui.add(enable_kill.set("enable kill on screen exit"));
		gui.add(enable_bounce.set("enable bounce", false));
	}


	vector<CutParticle> p;
	ofTexture* frame;
	ofVec2f orig_location, curr_location;

	//@todo:
	//gotta be static - as we are doing one per cut
	//can we see if there is a dynamic way to do gui? would be great to have diff ctrls for each
	ofParameterGroup gui;
	ofParameter<bool> enable_limit, enable_kill, enable_bounce;
	ofParameter<int> limit;

private:

};