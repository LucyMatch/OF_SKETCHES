#include "Block.h"

//@TODO: Add to GUI
float Block::seekLimit = 3.0;
float Block::speedLimit = 2.0;
float Block::massBase = 11.0;
ofColor Block::color = ofColor(0);
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
		ofPushStyle();
		ofSetColor(color);
		ofFill();
		ofDrawRectangle(location.x, location.y, r, r);
		ofPopStyle();
}

void Block::update() {


	//limit the magnitude of a vector
	velocity.limit(speedLimit);

	velocity = velocity + acceleration;
	location = location + velocity;

	//reset acceleration each frame
	acceleration = acceleration * 0;

}

