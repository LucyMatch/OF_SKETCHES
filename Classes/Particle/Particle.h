#pragma once

#include "ofMain.h"

class Particle {

public: 

    Particle(ofVec2f _location, int _r);
    void update();
    void draw();
    void applyforce(ofVec2f force);
    void seek(ofVec2f target);
    void checkEdges(ofVec2f dims);

    ofVec2f location;
    ofVec2f velocity;
    ofVec2f acceleration;

    float y_origin;
    float seekLimit;
    int r;
    float speedLimit;
    float mass;
    bool dead;

    ofColor color;

private: 

};