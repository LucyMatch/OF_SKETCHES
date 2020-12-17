#pragma once

#include "ofMain.h"
#include "Particle.h"

class Forces {

public:

	Forces();

	void display();
	void update();
	void update(ofVec2f loc);
	void update(ofVec2f loc, float _r);

	/// GETTERS
	bool inRange(Particle* p);
	virtual ofVec2f force(Particle* p);

	ofVec2f location;
	float radius;
	static float mass, G, C, minDis, maxDis, rmod;

private:

};

#pragma mark REPEL

class Repel : public Forces {

public:

	Repel();

	virtual ofVec2f force(Particle* p);
	static float mass, G, C, minDis, maxDis, rmod;

private:

};

#pragma mark FRICTION

class Friction : public Forces {

public:

	Friction();

	virtual ofVec2f force(Particle* p);
	static float mass, G, C, minDis, maxDis, rmod;

private:

};

#pragma mark DRAG

class Drag : public Forces {

public:

	Drag();

	virtual ofVec2f force(Particle* p);
	static float mass, G, C, minDis, maxDis, rmod;

private:

};

#pragma mark ATTRACT

class Attract : public Forces {

public:

	Attract();

	virtual ofVec2f force(Particle* p);
	static float mass, G, C, minDis, maxDis, rmod;

private:

};