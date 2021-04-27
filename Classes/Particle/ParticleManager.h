#pragma once


#include "ofMain.h"
#include "Particle.h"

class ParticleManager {

public: 

	//--------------------------------------------------------------
	ParticleManager() {
		draw_dims = { 0, 0, ofGetWidth(), ofGetHeight() };
		fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
	};

	//--------------------------------------------------------------
	void setup(glm::vec4 dims) {
		draw_dims = dims;
	};

	void spawn(ofVec2f loc) {
		Particle _p(loc);
		p.push_back(_p);
	};

	//--------------------------------------------------------------
	virtual void simpleSpawn() {

		if (enable_limit && p.size() < limit) {
			//start with a default force
			ofVec2f gravity(ofRandom(-1, 1), ofRandom(-1, 1));

			ofVec2f _loc;
			_loc.set(ofRandom(draw_dims.x, (draw_dims.x + draw_dims.z)), ofRandom(draw_dims.y, (draw_dims.y + draw_dims.w)));

			Particle _p(_loc);

			_p.applyforce(gravity);

			p.push_back(_p);
		}
	};

	//--------------------------------------------------------------
	void multiSpawn( int amount ) {
		if (enable_limit && p.size() < limit) {
			for (int i = 0; i < amount; i++) {
				ofVec2f _loc;
				_loc.set(ofRandom(draw_dims.x, (draw_dims.x + draw_dims.z)), ofRandom(draw_dims.y, (draw_dims.y + draw_dims.w)));
				Particle _p(_loc);
				p.push_back(_p);
			}
		}

	};

	//--------------------------------------------------------------
	virtual void update() {

		for (int i = 0; i < p.size(); i++) {

			p[i].update();

			p[i].checkEdges(draw_dims, true);

			//erase dead
			if (p[i].dead) {
				p.erase(p.begin() + i);
			}
		}
	};

	//--------------------------------------------------------------
	virtual void applyforce(ofVec2f f) {
		for (int i = 0; i < p.size(); i++) {
			p[i].applyforce(f);
		}
	};

	//--------------------------------------------------------------
	virtual void applyVaryingGravity(float min, float max) {
		for (int i = 0; i < p.size(); i++) {
			ofVec2f gravity(ofRandom(min, max), 0);
			p[i].applyforce(gravity);
		}
	};

	//applies to home lcoation by default
	//--------------------------------------------------------------
	void applySeek() {
		for (int i = 0; i < p.size(); i++) {
			p[i].seek(p[i].home_location);
		}
	};

	//--------------------------------------------------------------
	void applySeek( ofVec2f loc) {
		for (int i = 0; i < p.size(); i++) {
			p[i].seek(loc);
		}
	};

	//--------------------------------------------------------------
	virtual void draw() {

		ofEnableBlendMode(blends[b_mode_selector]);

		for (int i = 0; i < p.size(); i++) {
		    p[i].draw();
		}	
	};

	//--------------------------------------------------------------
	void drawFbo(int bg_alpha) {
		fbo.begin();

			ofPushStyle();
			ofSetColor(255, 255, 255, bg_alpha);
			ofDrawRectangle(0,0,ofGetWidth(),ofGetHeight());
			ofPopStyle();

			draw();

		fbo.end();
	};

	ofFbo& getFbo() {
		return fbo;
	}

	//--------------------------------------------------------------
	void drawDebug() {
		ofPushStyle();
		ofSetColor(255, 0, 0, 100);
		ofDrawRectangle(draw_dims[0], draw_dims[1], draw_dims[2], draw_dims[3]);
		ofPopStyle();
	};


	//vars
	//--------------------------------------------------------------
	vector<Particle> p;
	glm::vec4 draw_dims;
	bool enable_limit = true;
	int limit = 100;
	int b_mode_selector = 1;
	std::map<int, ofBlendMode> blends = Particle::blends;
	ofFbo fbo;


private: 

};