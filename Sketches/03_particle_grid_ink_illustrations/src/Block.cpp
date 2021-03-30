#include "Block.h"

//@TODO: Add to GUI
//behavior
ofParameter<float> Block::seek_limit = 50.0;
ofParameter<float> Block::speed_limit = 2.0;
ofParameter<float> Block::mass_base = 11.0;
ofParameter<bool> Block::arrive_on = true;
ofParameter<bool> Block::seek_on = true;
ofParameter<bool> Block::trail = true;
ofParameter<int> Block::arrive_cap = 100;

//asethetic
ofParameter<ofColor> Block::pcolor = ofColor(0, 0, 0, 100);
ofParameter<ofColor> Block::tcolor = ofColor(0,0,0,100);
ofParameter<float> Block::trail_wgt = 5.0;
ofParameter<int> Block::history_length = 3;
ofParameter<int> Block::r = 5;

Block::Block() {
		dead = false;
		mass = mass_base / r;
		velocity.set(0, 0);
		acceleration.set(0.0, 0.00);
		location = ofVec2f(0, 0);
		home_location = location;

		//limit the magnitude of a vector
		velocity.limit(speed_limit);
};

Block::Block(ofVec2f  _location) {
		dead = false;
		mass = mass_base / r;
		velocity.set(0, 0);
		acceleration.set(0.0, 0.00);
		location = _location;
		home_location = location;

		//limit the magnitude of a vector
		velocity.limit(speed_limit);
}

void Block::draw() {
	if (!dead) {

		if (trail) {
			ofPushStyle();
			ofSetColor(tcolor);
			ofSetLineWidth(trail_wgt);
			for (int i = 1; i < history.size(); i++) {
				ofDrawLine(history[i - 1].x, history[i - 1].y, history[i].x, history[i].y);
			}
			ofPopStyle();
		}

		ofPushStyle();
		ofSetColor(pcolor);
		ofFill();
		ofDrawRectangle((location.x - (r/2)), (location.y - r/2), r, r);
		ofPopStyle();

	}
}

void Block::update() {

	//limit the magnitude of a vector
	velocity.limit(speed_limit);

	velocity = velocity + acceleration;
	location = location + velocity;

	//reset acceleration each frame
	acceleration = acceleration * 0;

	//for trail / history
	if (trail) {
		history.push_back(location);
		int diff = history.size() - history_length;
		if (history.size() > history_length) {
			for (int i = 0; i < diff; i++) {
				history.erase(history.begin() + i);
			}
			//for trails that originate from home pt
			//for (int i = 1; i < diff; i++) {
			//	history.erase(history.begin() + i);
			//}
		}
	}
}

/// Particle seeking provided pt
void Block::seek(ofVec2f& target) {

	ofVec2f desiredV;
	desiredV = target - location;

	desiredV.normalize();
	desiredV *= speed_limit;

	ofVec2f steer = desiredV - velocity;
	steer.limit(seek_limit);
	applyforce(steer);

}

/// Particle behavior if within arrive cap of seek target
/// the below alters the speedlimit ( mapped to base limit ) the closer the particle gets to seek pt
void Block::arrive(ofVec2f& target) {

	ofVec2f desiredV = target - location;

	float d = desiredV.length();

	if (d < arrive_cap) {
		float m = ofMap(d, 0, arrive_cap, 0, speed_limit);
		desiredV *= m;
	}
	else {
		desiredV *= speed_limit;
	}

	ofVec2f steer = desiredV - velocity;
	steer.limit(seek_limit);
	applyforce(steer);

}

