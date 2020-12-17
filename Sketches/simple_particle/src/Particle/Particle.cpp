#include "Particle.h"

Particle::Particle(ofVec2f _location, int _r) {

    //all this stuff should be on GUI..

    speedLimit = 5;
    seekLimit = 10;
    mass = 10.0 / _r;
    velocity.set(0, 0);
    acceleration.set(0.0, 0.00);
    location = _location;

    //limit the magnitude of a vector
    velocity.limit(speedLimit);
    r = _r;
    dead = false;
    
    y_origin = location.y;

    color = ofColor(255, 255, 255, 255);

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

void Particle::checkEdges(ofVec2f x_dims) {

    //this will just bounce them for now... 
    //obvi not what we want... 
    //if (location.x < 0) {
    //    location.x = 0;
    //    velocity.x *= -1;
    //}
    //else if (location.x > ofGetWidth()) {
    //    location.x = ofGetWidth();
    //    velocity.x *= -1;
    //}

    if (location.x < x_dims.x) {
        dead = true;
    }


}