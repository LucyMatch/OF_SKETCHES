#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxFaceTracker2.h"
#include "VideoHandler.h"
#include "PolyCuts.h"

class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();

    void initGui();
    void drawGui(ofEventArgs& args);
    void framerate();
    void drawDebug();

    void keyPressed(int key);
    void mouseMoved(int x, int y);

    VideoHandler video;

    ofxFaceTracker2 tracker;

    //testing singular cut
    PolyCuts left_eye, right_eye, mouth;

    ofxPanel gui;
    ofParameter<ofColor> bg_c;
    ofParameter<bool> enable_debug, enable_orig;


};
