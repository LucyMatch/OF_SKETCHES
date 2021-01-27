#include "Particle.h"

//behavior
float Particle::seekLimit = 10.0;
float Particle::speedLimit = 5.0;
float Particle::massBase = 10.0;
bool Particle::arrive_on = true;
bool Particle::seek_on = true;
bool Particle::trail = true;
int Particle::arrive_cap = 100;

//asethetic
ofColor Particle::pcolor = ofColor(0, 0, 0, 100);
ofColor Particle::tcolor = ofColor(0, 0, 0, 100);
float Particle::trail_wgt = 1.0;
int Particle::history_length = 10;
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

Particle::Particle(ofVec2f &_location) {
    dead = false;
    mass = massBase / r;
    velocity.set(0, 0);
    acceleration.set(0.0, 0.00);
    location = _location;
    home_location = location;
    //limit the magnitude of a vector
    velocity.limit(speedLimit);
}

Particle::~Particle() {}

void Particle::update() {
    velocity = velocity + acceleration;
    //limit the magnitude of a vector
    velocity.limit(speedLimit);
    location = location + velocity;
    //reset acceleration each frame
    acceleration = acceleration * 0;

    //for trail / history
    //for trail / history
    history.push_back(location);
    int diff = history.size() - history_length;
    if (history.size() > history_length) {
        for (int i = 0; i < diff; i++) {
            history.erase(history.begin() + i);
        }
    }
}

void Particle::draw() {
    if (!dead) {

        if (trail) {
            ofPushStyle();
            ofSetColor(tcolor);
            ofSetLineWidth(trail_wgt);
            for (int i = 1; i < history.size(); i++) 
                ofDrawLine(history[i - 1].x, history[i - 1].y, history[i].x, history[i].y);
            ofPopStyle();
        }

       ofPushStyle();
        ofSetColor(pcolor);
        ofFill();
        ofDrawRectangle(location.x, location.y, r, r);
       ofPopStyle();

    }
}

void Particle::applyforce(ofVec2f &force) {
    ofVec2f f = force;
    force = force / mass;
    acceleration += force;
}

/// Particle seeking provided pt
void Particle::seek(ofVec2f &target) {

    ofVec2f desiredV;
    desiredV = target - location;
    
    desiredV.normalize();
    desiredV *= speedLimit;

    ofVec2f steer = desiredV - velocity;
    steer.limit(seekLimit);
    applyforce(steer);

}

/// Particle behavior if within arrive cap of seek target
/// the below alters the speedlimit ( mapped to base limit ) the closer the particle gets to seek pt
void Particle::arrive(ofVec2f& target) {

    ofVec2f desiredV = target - location;

    float d = desiredV.length();

    if (d < arrive_cap) {
        float m = ofMap(d, 0, arrive_cap, 0, speedLimit);
        desiredV *= m;
    }else {
        desiredV *= speedLimit;
    }

    ofVec2f steer = desiredV - velocity;
    steer.limit(seekLimit);
    applyforce(steer);

}

void Particle::setHome(ofVec2f& loc) {
    home_location = loc;
}

void Particle::kill() {
    dead = true;
}

void Particle::checkEdges(glm::vec4 dims, bool _kill) {
    bool _x = false, _y = false;
    if (location.x < dims.x || location.x >(dims.x + dims.z)) _x = true;
    if (location.y < dims.y || location.y >(dims.y + dims.w)) _y = true;
    if (_x || _y) {
        if (_kill)kill();
        else bounce(_x, _y);
    }
}


void Particle::bounce(bool _x, bool _y) {
    if (_x)velocity.x *= -1;
    if (_y)velocity.y *= -1;
}

