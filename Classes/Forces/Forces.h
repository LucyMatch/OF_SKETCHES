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

	//for dealing with diff shaped forces
	ofRectangle r;
	float triangleArea(glm::vec2& a, glm::vec2& b, glm::vec2& c);
	bool isInsideTriangle(glm::vec2& p, glm::vec2& a, glm::vec2& b, glm::vec2& c);

	ofParameterGroup force_ctrl;
	ofParameter<float> mass, G, C, min_dis, max_dis, rmod;
	ofParameter<ofColor> display_color;
	ofParameter<int> shape_option;

private:

};

#pragma mark REPEL

class Repel : public Forces {

public:

	Repel();

	virtual ofVec2f force(Particle const& p);

private:

};

#pragma mark FRICTION

class Friction : public Forces {

public:

	Friction();

	virtual ofVec2f force(Particle const& p);

private:

};

#pragma mark DRAG

class Drag : public Forces {

public:

	Drag();

	virtual ofVec2f force(Particle const& p);

private:

};

#pragma mark ATTRACT

class Attract : public Forces {

public:

	Attract();

	virtual ofVec2f force(Particle const& p);

private:

};