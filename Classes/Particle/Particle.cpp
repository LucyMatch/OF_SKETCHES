#include "Particle.h"

float Particle::seekLimit = 10.0;
float Particle::speedLimit = 5.0;
float Particle::massBase = 10.0;
ofColor Particle::color = ofColor(0, 0, 0, 100);
int Particle::r = 1;

Particle::Particle() {

    dead = false;
    mass = massBase / r;
    velocity.set(0, 0);
    acceleration.set(0.0, 0.00);
    location = ofVec2f(0,0);
    home_location = location;

    //limit the magnitude of a vector
    velocity.limit(speedLimit);

}

Particle::Particle(ofVec2f _location) {
    dead = false;
    mass = massBase / r;
    velocity.set(0, 0);
    acceleration.set(0.0, 0.00);
    location = _location;
    home_location = location;

    //limit the magnitude of a vector
    velocity.limit(speedLimit);
}

void Particle::update() {


    //limit the magnitude of a vector
    velocity.limit(speedLimit);

    velocity = velocity + acceleration;
    location = location + velocity;

    //reset acceleration each frame
    acceleration = acceleration * 0;

}

void Particle::draw() {
    if (!dead) {
        ofPushStyle();
        ofSetColor(color);
        ofFill();
        ofDrawRectangle(location.x, location.y, r, r);
       ofPopStyle();
    }
}

void Particle::applyforce(ofVec2f force) {

    ofVec2f f = force;

    force = force / mass;

    acceleration += force;

}

void Particle::seek(ofVec2f target) {

    ofVec2f desiredV;
    desiredV = target - location;
    
    desiredV.normalize();
    desiredV *= speedLimit;

    ofVec2f steer = desiredV - velocity;
    steer.limit(seekLimit);
    applyforce(steer);

}

void Particle::checkEdges(glm::vec4 dims) {
    if (location.x < dims.x || location.x >(dims.x + dims.z)) {
        kill();
    }
    if (location.y < dims.y || location.y >(dims.y + dims.w)) {
        kill();
    }
}

void Particle::kill() {
    dead = true;
}

void Particle::setHome(ofVec2f loc) {
    home_location = loc;
}