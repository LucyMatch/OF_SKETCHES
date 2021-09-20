#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxFaceTracker2.h"
#include "VideoHandler.h"
#include "PolyCuts.h"
#include "LandmarkCutManager.h"

class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();

    void initGui();
    void drawGui(ofEventArgs& args);
    void framerate();
    void drawDebug();
    void drawInfo();
    void intervalExport();

    void keyPressed(int key);
    void mouseMoved(int x, int y);

    ofFbo main_draw;

    VideoHandler video;

    ofxFaceTracker2 tracker;

    LandmarkCutManager cut_man;

    //int cut_count = 0;
    bool show_info, enable_interval_export;
    unsigned long time, time_elapsed;

    ofxPanel gui;

    //@TODO: update these / tidy - this is all from previous sketches
    //      just getting baseline func in....
    ofParameter<ofColor> bg_c, cuts_c;
    ofParameter<bool> enable_debug, enable_orig;
    ofParameter<float> time_interval;

};
