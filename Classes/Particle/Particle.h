#pragma once

#include "ofMain.h"

class Particle {

public: 

    Particle();
    Particle(ofVec2f _location);
    virtual void update();
    virtual void draw();
    void setHome(ofVec2f loc);
    void applyforce(ofVec2f force);
    void seek(ofVec2f target);
    void checkEdges(glm::vec4 dims);
    void kill();

    ofVec2f location;
    ofVec2f velocity;
    ofVec2f acceleration;
    ofVec2f home_location;

    bool dead;
    float mass;

    static float seekLimit, speedLimit, massBase;
    static ofColor color;
    static int r;

private: 

    

};