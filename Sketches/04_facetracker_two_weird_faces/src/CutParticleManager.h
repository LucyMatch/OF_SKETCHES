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

		if (enable_auto_spawn) {
			if (enable_random_spawn)
				randomSpawn();
			else
				spawn();
		}

		if (enable_varying_gravity)applyVaryingGravity(v_gravity_min, v_gravity_max, v_gravity_direction);

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

	virtual void drawFBO() {
		if (enabled) {
			ofPushStyle();
			fbo.begin();
				ofSetColor(bg, bg_alpha);
				ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
				ofSetColor(c);
				for (auto& _p : p)
					_p.draw();
			fbo.end();
			ofPopStyle();
		}
	}

	virtual void spawn() {
		if (enabled && enable_limit && p.size() < limit) {
			ofTexture* tex;
			if (enable_image_set && img_set_valid) {
				tex = &(*img_set)[img_set_counter];
				nextImg();
			}
			else
				tex = &frame;
			CutParticle _p(tex, curr_location);
			p.push_back(_p);
		}
	}

	virtual void spawn(glm::vec2 loc) {
		if (enabled && enable_limit && p.size() < limit) {
			ofTexture* tex;
			if (enable_image_set && img_set_valid) {
				tex = &(*img_set)[img_set_counter];
				nextImg();
			}
			else
				tex = &frame;
			CutParticle _p(tex, loc);
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

	void setImageSet( vector<ofTexture>* imgs) {
		img_set = imgs;
		img_set_counter = 0;
		img_set_valid = true;
	}

	void nextImg() {
		img_set_counter = ++img_set_counter % ((*img_set).size());
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
		gui.add(bg.set("bg", ofColor(255, 255, 255, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));
		gui.add(bg_alpha.set("bg_alpha", 0, 0, 255));
		gui.add(c.set("colour", ofColor(255, 255, 255, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));
		gui.add(db_c.set("debug colour", ofColor(125, 0, 0, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));
		gui.add(enable_limit.set("enable limit", true));
		gui.add(enable_live_cut.set("enable live frame updates", true));
		gui.add(limit.set("limit amt", 100, 0, 500));
		gui.add(enable_kill.set("enable kill on screen exit", true));
		gui.add(enable_bounce.set("enable bounce", false));
		gui.add(enable_auto_spawn.set("enable auto spawning", false));
		gui.add(enable_random_spawn.set("enable random spawning", false));
		gui.add(enable_varying_gravity.set("enable varying gravity", false));
		gui.add(v_gravity_direction.set("gravity direction", 0, 0, 3));
		gui.add(v_gravity_min.set("gravity min", 0.5, 0.5, 25));
		gui.add(v_gravity_max.set("gravity max", 10, 0.5, 25));
		gui.add(enable_image_set.set("enable_image set particles", false));
		gui.add(enable_tex_cutting.set("enable particle texture cutting", true));
	}

	BaseCut getCut() { return cut; }
	ofFbo* getFBO() { return &fbo; }

	/// 
	/// 
	/// 

	vector<CutParticle> p;

	bool enabled;

	ofParameterGroup gui;
	ofParameter<bool> enable_limit, enable_kill, enable_bounce, enable_live_cut, enable_auto_spawn, enable_random_spawn, enable_varying_gravity, enable_image_set, enable_tex_cutting;
	ofParameter<int> limit, bg_alpha;
	ofParameter<ofColor> db_c, c, bg;
	ofParameter<int> v_gravity_direction;
	ofParameter<float> v_gravity_min, v_gravity_max;

private:
	BaseCut cut;
	ofFbo fbo;
	ofTexture frame;
	vector<ofTexture>* img_set;	//frames can be disabled + cut particles can be displayed using a set of images stored here
	int img_set_counter = 0;
	bool img_set_valid = false;
	glm::vec2 orig_location, curr_location;
};