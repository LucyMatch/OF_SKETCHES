#include "Forces.h"

float Forces::G = 1.0;
float Forces::C = 1.0;
float Forces::mass = 1;
float Forces::minDis = 5;
float Forces::maxDis = 100;
float Forces::rmod = 2;

Forces::Forces() {}

void Forces::display() {
	ofPushStyle();
	ofFill();
	ofSetColor(255, 0, 0, 255);
	ofDrawCircle(location, radius);	
	ofPopStyle();
}
void Forces::update() {

}
void Forces::update(ofVec2f loc) {
	location = loc;
}
void Forces::update(ofVec2f loc, float _r) {
	location = loc;
	radius = _r;
}
bool Forces::inRange(Particle* p) {
	float dist = location.distance(p->location);
	if (dist < (radius * rmod)) {return true;}
	else {return false;}
}
ofVec2f Forces::force(Particle* p) {
	ofVec2f f(0.0, 0.0);
	return f;
}

#pragma mark REPEL

float Repel::G = 5.0;
float Repel::C = 1.0;
float Repel::mass = 50;
float Repel::minDis = 5;
float Repel::maxDis = 100;
float Repel::rmod = 2;

Repel::Repel() {}

ofVec2f Repel::force(Particle* p) {
	ofVec2f dir = location - p->location;
	float d = dir.length();
	dir.normalize();
	d = ofClamp(d, minDis, maxDis);
	//power version / not G & M
	//float force =  -1 * power / (d*d);
	float force = (-1 * G * mass * p->mass) / (d * d);
	dir *= force;
	return dir;
}

#pragma mark FRICTION

float Friction::G = 1.0;
float Friction::C = 1.0;
float Friction::mass = 1;
float Friction::minDis = 5;
float Friction::maxDis = 100;
float Friction::rmod = 2;

Friction::Friction() {}

ofVec2f Friction::force(Particle* p) {
	ofVec2f f;
	float normal = 1;
	float fMag = C * normal;
	f = p->velocity;
	f *= -1; // friction has a direction opp to p
	f.normalize();
	f *= fMag;
	return f;
}

#pragma mark DRAG

float Drag::G = 1.0;
float Drag::C = 1.0;
float Drag::mass = 1;
float Drag::minDis = 5;
float Drag::maxDis = 100;
float Drag::rmod = 2;

Drag::Drag() {}

ofVec2f Drag::force(Particle* p) {
	float speed = p->velocity.length();
	float dragMagnitude = C * speed * speed;

	ofVec2f drag = p->velocity;
	drag *= -1;
	drag.normalize();
	drag *= dragMagnitude;
	return drag;
}

#pragma mark ATTRACT

float Attract::G = 1.0;
float Attract::C = 1.0;
float Attract::mass = 1;
float Attract::minDis = 5;
float Attract::maxDis = 100;
float Attract::rmod = 2;

Attract::Attract() {}

ofVec2f Attract::force(Particle* p) {
	ofVec2f force;
	force = location - p->location;
	float distance = location.distance(p->location);
	distance = ofClamp(distance, minDis, maxDis);
	force.normalize();
	float strength = (G * mass * p->mass) / (distance * distance);
	force *= strength;
	return force;
}