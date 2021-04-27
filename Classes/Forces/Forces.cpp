#include "Forces.h"
#include <cmath>

//creating ofparams for these - disabling static for now - 
//i want to control per instance.. not per class
//float Forces::G = 1.0;
//float Forces::C = 1.0;
//float Forces::mass = 1;
//float Forces::min_dis = 5;
//float Forces::max_dis = 100;
//float Forces::rmod = 2;

Forces::Forces() { init(); }

void Forces::init() {
	force_ctrl.add(G.set("G", 1.0, 0.0, 25.0));
	force_ctrl.add(C.set("C", 1.0, 0.0, 25.0));
	force_ctrl.add(mass.set("mass", 1.0, 0.0, 50.0));
	force_ctrl.add(min_dis.set("min distance", 5.0, 0.0, 500.0));
	force_ctrl.add(max_dis.set("max distance", 100.0, 0.0, 1000.0));
	force_ctrl.add(rmod.set("radius modifer", 2.0, -10.0, 250));
	force_ctrl.add(display_color.set("display color", ofColor(255,255,255,255), ofColor(0,0,0,0), ofColor(255,255,255,255)));
	force_ctrl.add(shape_option.set("force shape", 0, 0, 2));
}
void Forces::display() {
	glm::vec2 a, b, c;
	ofPushStyle();
	ofFill();
	ofSetColor(display_color);
	switch (shape_option) {
	case 0:
		ofDrawCircle(location, radius * rmod);
		break;
	case 1:
		r.setFromCenter(location, radius * rmod, radius * rmod);
		ofDrawRectangle(r);
		break;
	case 2:
		a = glm::vec2(location.x, location.y + (sqrt(3) / 3) * (radius * rmod));
		b = glm::vec2(location.x - (radius * rmod)/2, location.y - (sqrt(3) / 6) * (radius * rmod));
		c = glm::vec2(location.x + (radius * rmod) / 2, location.y - (sqrt(3) / 6) * (radius * rmod));
		ofDrawTriangle( a, b, c );
		break;
	default:
		break;
	}
	
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
bool Forces::inRange(Particle const& p) {
	switch (shape_option) {
	case 0:
		//circle
		if (location.distance(p.location) < (radius * rmod)) { return true; }
		else { return false; }
		break;
	case 1:
		//square
		r.setFromCenter(location, radius * rmod, radius * rmod);
		cout << "square mode - is inside? " << r.inside(p.location) << endl;
		return r.inside(p.location);
		break;
	case 2:
		//	triangle?



		break;
	default:
		break;
	}

}
ofVec2f Forces::force(Particle const& p) {
	ofVec2f f(0.0, 0.0);
	return f;
}
float Forces::triangleArea(glm::vec2& a, glm::vec2& b, glm::vec2& c) {
	return abs((a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y)) / 2.0);
}
bool Forces::isInsideTriangle(glm::vec2& p, glm::vec2& a, glm::vec2& b, glm::vec2& c) {
	float A = triangleArea(a,b,c);
	float A1 = triangleArea(p,b,c);
	float A2 = triangleArea(a,p,c);
	float A3 = triangleArea(a,b,p);
	return (A == A1 + A2 + A3);
}

#pragma mark REPEL

//float Repel::G = 10.0;
//float Repel::C = 1.0;
//float Repel::mass = 50;
//float Repel::min_dis = 5;
//float Repel::max_dis = 100;
//float Repel::rmod = 2;

Repel::Repel() {}

ofVec2f Repel::force(Particle const& p) {
	ofVec2f dir = location - p.location;
	float d = dir.length();
	dir.normalize();
	d = ofClamp(d, min_dis, max_dis);
	//power version / not G & M
	//float force =  -1 * power / (d*d);
	float force = (-1 * G * mass * p.mass) / (d * d);
	dir *= force;
	return dir;
}

#pragma mark FRICTION

//float Friction::G = 1.0;
//float Friction::C = 1.0;
//float Friction::mass = 1;
//float Friction::min_dis = 5;
//float Friction::max_dis = 100;
//float Friction::rmod = 2;

Friction::Friction() {}

ofVec2f Friction::force(Particle const& p) {
	ofVec2f f;
	float normal = 1;
	float fMag = C * normal;
	f = p.velocity;
	f *= -1; // friction has a direction opp to p
	f.normalize();
	f *= fMag;
	return f;
}

#pragma mark DRAG

//float Drag::G = 1.0;
//float Drag::C = 1.0;
//float Drag::mass = 1;
//float Drag::min_dis = 5;
//float Drag::max_dis = 100;
//float Drag::rmod = 2;

Drag::Drag() {}

ofVec2f Drag::force(Particle const& p) {
	float speed = p.velocity.length();
	float dragMagnitude = C * speed * speed;

	ofVec2f drag = p.velocity;
	drag *= -1;
	drag.normalize();
	drag *= dragMagnitude;
	return drag;
}

#pragma mark ATTRACT

//float Attract::G = 1.0;
//float Attract::C = 1.0;
//float Attract::mass = 1;
//float Attract::min_dis = 5;
//float Attract::max_dis = 100;
//float Attract::rmod = 2;

Attract::Attract() {}

ofVec2f Attract::force(Particle const& p) {
	ofVec2f force;
	force = location - p.location;
	float distance = location.distance(p.location);
	distance = ofClamp(distance, min_dis, max_dis);
	force.normalize();
	float strength = (G * mass * p.mass) / (distance * distance);
	force *= strength;
	return force;
}