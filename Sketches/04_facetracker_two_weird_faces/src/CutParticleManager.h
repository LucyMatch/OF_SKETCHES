#pragma once

#include "ParticleManager.h"
#include "CutParticle.h"
#include "Cuts.h"

/*
	we generate a partcle manager PER landmark / facial feature... 

	keep an eye on performance -> as the way our landmark manager works is per instances they get created / destroyed

	@INVESTIGATE :
	may be better at some point to start with a static amount of managers ( one for left_eye, right_eye, mouth ) 
	and they just get updated in regards to if there are current instances witht hose features... 

	@TODO: 
	eventually we will want each manager to individual control of it's particles... 
	so the managers are what control uniquiness -> likely not PER manager
	but per FEATURE
	i.e. apply all x stuff to left eye
	
*/


class CutParticleManager : public ParticleManager {

public:
	CutParticleManager() {
		orig_location = glm::vec2(ofRandom(0, ofGetWidth()), ofRandom(0, ofGetHeight()));
		setLocation(orig_location);
		enabled = true;
		initGui();
	};

	CutParticleManager(glm::vec2 loc) {
		orig_location = loc;
		setLocation(loc);
		enabled = true;
		initGui();
	};

	CutParticleManager(BaseCut c) {
		cut = c;
		orig_location = c.getCenter();
		setLocation(orig_location);
		enabled = true;
		initGui();
	};

	

	virtual void update() {
		for (int i = 0; i < p.size(); i++) {
			p[i].update();

			if (enable_bounce)
				p[i].checkEdges(glm::vec4(0, 0, ofGetWidth(), ofGetHeight()), false);

			if (enable_kill && !enable_bounce)
				p[i].checkEdges(glm::vec4(0, 0, ofGetWidth(), ofGetHeight()), true);

			if (p[i].dead)
				p.erase(p.begin() + i);
		}
	}

	virtual void update(ofTexture _img) {
		frame = _img;
		if (enable_live_cut) {
			for (auto& _p : p)
				_p.updateFrame(&frame);
		}
	}

	virtual void update(BaseCut c) {
		cut = c;
		orig_location = c.getCenter();
		setLocation(orig_location);
	}

	virtual void draw() {
		if (enabled) {
			ofPushStyle();
			ofSetColor(c);
			for (auto& _p : p)
				_p.draw();
			ofPopStyle();
		}
	}

	virtual void spawn() {
		if (enabled && enable_limit && p.size() < limit) {
			CutParticle _p(&frame, curr_location);
			p.push_back(_p);
		}
	}

	virtual void spawn(glm::vec2 loc) {
		if (enabled && enable_limit && p.size() < limit) {
			CutParticle _p(&frame, loc);
			p.push_back(_p);
		}
	}

	virtual void applyVaryingGravity(float min, float max, int direction) {
		for (auto& _p : p) {
			glm::vec2 gravity;
			if (direction == 0)gravity = glm::vec2(ofRandom(min, max), 0);
			if (direction == 1)gravity = glm::vec2(0, ofRandom(min, max));
			if (direction == 2)gravity = glm::vec2(-ofRandom(min, max), 0);
			if (direction == 3)gravity = glm::vec2(0, -ofRandom(min, max));
			_p.applyforce(ofVec2f(gravity));
		}
	}

	void setLocation(glm::vec2 loc) {
		curr_location = loc;
	}

	void randomSpawn() {
		if(enabled)spawn(glm::vec2(ofRandom(0, ofGetWidth()), ofRandom(0, ofGetWidth())));
	}

	void drawDebug() {
		ofPushStyle();
		ofSetColor(db_c);
		for (auto& _p : p)
			_p.drawDebug();
		ofPopStyle();
	}

	void clear() {
		p.clear();
	}

	void initGui() {
		gui.setName("particle manager");
		gui.add(enable_limit.set("enable limit", true));
		gui.add(enable_live_cut.set("enable live frame updates", true));
		gui.add(limit.set("limit amt", 100, 0, 500));
		gui.add(enable_kill.set("enable kill on screen exit", true));
		gui.add(enable_bounce.set("enable bounce", false));
		gui.add(db_c.set("debug colour", ofColor(125, 0, 0, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));
		gui.add(c.set("colour", ofColor(255, 255, 255, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));
	}

	BaseCut getCut() { return cut; }

	vector<CutParticle> p;

	bool enabled;

	ofParameterGroup gui;
	ofParameter<bool> enable_limit, enable_kill, enable_bounce, enable_live_cut;
	ofParameter<int> limit;
	ofParameter<ofColor> db_c, c;

private:
	BaseCut cut;
	ofTexture frame;
	glm::vec2 orig_location, curr_location;
};