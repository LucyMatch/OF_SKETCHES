#pragma once

#include "ofMain.h"

class Particle {

public: 

    Particle();
    Particle(ofVec2f &_location);
    ~Particle();

    virtual void update();
    virtual void draw();

    void setHome(ofVec2f &loc);
    void applyforce(ofVec2f &force);
    void kill();

    virtual void seek(ofVec2f &target);
    virtual void arrive(ofVec2f& target);

    void checkEdges(glm::vec4 dims, bool _kill);
    void bounce(bool _x, bool _y);

    ofVec2f location, velocity, acceleration, home_location;
    vector<ofVec2f> history;

    bool dead;
    float mass;

    static float seekLimit, speedLimit, massBase, trail_wgt;
    static ofColor pcolor, tcolor;
    static int r, arrive_cap, history_length;
    static bool trail, arrive_on, seek_on;

private: 

    

};