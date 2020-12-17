#pragma once


#include "ofMain.h"
#include "Particle.h"

class ParticleManager {

public: 

	//--------------------------------------------------------------
	ParticleManager() {
		draw_dims = { 0, ofGetWidth(), 0, ofGetHeight() };
	};

	//--------------------------------------------------------------
	void setup(glm::vec4 dims) {
		draw_dims = dims;
	};

	//--------------------------------------------------------------
	void simpleSpawn() {

		//start with a default force
		ofVec2f gravity(-10, 0);

		ofVec2f _loc;
		_loc.set(draw_dims[1], ofRandom(draw_dims[2], draw_dims[3]));

		Particle _p(_loc, 5);

		_p.applyforce(gravity);

		p.push_back(_p);
	};

	//--------------------------------------------------------------
	void multiSpawn( int amount ) {
		for (int i = 0; i < amount; i++) {
			ofVec2f _loc;
			_loc.set(draw_dims[1], ofRandom(draw_dims[2], draw_dims[3]));
			std::cout << _loc << std::endl;
			Particle _p(_loc, 1);
			p.push_back(_p);
		}

	};

	//--------------------------------------------------------------
	void update() {

		for (int i = 0; i < p.size(); i++) {

			p[i].update();

			p[i].checkEdges(ofVec2f(draw_dims[0], draw_dims[1]));

			//erase dead
			if (p[i].dead) {
				p.erase(p.begin() + i);
			}
		}
	};

	//--------------------------------------------------------------
	void applyforce(ofVec2f f) {
		for (int i = 0; i < p.size(); i++) {
			p[i].applyforce(f);
		}
	};

	//--------------------------------------------------------------
	void applyVaryingGravity(float min, float max) {
		for (int i = 0; i < p.size(); i++) {
			ofVec2f gravity(ofRandom(min, max), 0);
			p[i].applyforce(gravity);
		}
	};

	// seek locations are at x = 0, y = particle origin track 
	// the aim of this is to steer particle back toward hero
	//--------------------------------------------------------------
	void applySeek() {
		for (int i = 0; i < p.size(); i++) {
			//create target
			ofVec2f t(0, p[i].y_origin);
			p[i].seek( t );
		}
	};

	//--------------------------------------------------------------
	void draw() {
		for (int i = 0; i < p.size(); i++) {
		    p[i].draw();
		}	
	};



	//vars
	//--------------------------------------------------------------
	vector<Particle> p;
	glm::vec4 draw_dims;


private: 

};