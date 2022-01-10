#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxFaceTracker2.h"
#include "VideoHandler.h"
#include "PolyCuts.h"
#include "LandmarkCutManager.h"
#include "CutParticleManager.h"

class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();

    void checkPmen();
    void updatePmen();

    void initGui();
    void drawGui(ofEventArgs& args);
    void framerate();
    void drawDebug();
    void drawInfo();

    void intervalExport();

    void keyPressed(int key);
    void mouseMoved(int x, int y);

    ofFbo main_draw, cuts_draw;

    VideoHandler video;

    ofxFaceTracker2 tracker;

    LandmarkCutManager cut_man;

    vector<CutParticleManager> p_men;

    bool show_info, enable_interval_export;
    unsigned long time, time_elapsed;

    ofxPanel gui;

    ofParameter<ofColor> bg_c, cuts_draw_color, cuts_color;
    ofParameter<bool> enable_debug, enable_orig, enable_trails;
    ofParameter<float> time_interval, cuts_draw_alpha;

};
