#pragma once

#include "ofMain.h"
#include "Particle.h"

class Forces {

public:

	Forces();

	void init();
	void display();
	void update();
	void update(ofVec2f loc);
	void update(ofVec2f loc, float _r);

	/// GETTERS
	bool inRange(Particle const& p);
	virtual ofVec2f force(Particle const& p);

	ofVec2f location;
	float radius = 20;
	//static float mass, G, C, minDis, maxDis, rmod;
	//gonna update this so itsone gui panel 
	//and we can update the settings per instance

	ofParameterGroup force_ctrl;
	ofParameter<float> mass, G, C, min_dis, max_dis, rmod;
	ofParameter<ofColor> display_color;

private:

};

#pragma mark REPEL

class Repel : public Forces {

public:

	Repel();

	virtual ofVec2f force(Particle const& p);
	//static float mass, G, C, minDis, maxDis, rmod;
	//float radius = 20;

private:

};

#pragma mark FRICTION

class Friction : public Forces {

public:

	Friction();

	virtual ofVec2f force(Particle const& p);
	//static float mass, G, C, minDis, maxDis, rmod;
	//float radius = 20;

private:

};

#pragma mark DRAG

class Drag : public Forces {

public:

	Drag();

	virtual ofVec2f force(Particle const& p);
	//static float mass, G, C, minDis, maxDis, rmod;
	//float radius = 20;

private:

};

#pragma mark ATTRACT

class Attract : public Forces {

public:

	Attract();

	virtual ofVec2f force(Particle const& p);
	//static float mass, G, C, minDis, maxDis, rmod;
	//float radius = 20;

private:

};