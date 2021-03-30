#include "Block.h"

//@TODO: Add to GUI
//behavior
float Block::seekLimit = 3.0;
float Block::speedLimit = 2.0;
float Block::massBase = 11.0;
bool Block::arrive_on = false;
bool Block::seek_on = true;
bool Block::trail = true;
int Block::arrive_cap = 100;

//asethetic
ofColor Block::pcolor = ofColor(0, 0, 0, 100);
ofColor Block::tcolor = ofColor(0,0,0,100);
float Block::trail_wgt = 5.0;
int Block::history_length = 3;
int Block::r = 5;

Block::Block() {
		dead = false;
		mass = massBase / r;
		velocity.set(0, 0);
		acceleration.set(0.0, 0.00);
		location = ofVec2f(0, 0);
		home_location = location;

		//limit the magnitude of a vector
		velocity.limit(speedLimit);
};

Block::Block(ofVec2f  _location) {
		dead = false;
		mass = massBase / r;
		velocity.set(0, 0);
		acceleration.set(0.0, 0.00);
		location = _location;
		home_location = location;

		//limit the magnitude of a vector
		velocity.limit(speedLimit);
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
	velocity.limit(speedLimit);

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

