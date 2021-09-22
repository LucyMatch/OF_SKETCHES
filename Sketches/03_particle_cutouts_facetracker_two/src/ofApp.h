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

    void initGui();
    void drawGui(ofEventArgs& args);
    void updatePMenGui( int amt );
    void framerate();
    void drawDebug();
    void drawInfo();
    void intervalExport();
    void checkPmen();

    void keyPressed(int key);
    void mouseMoved(int x, int y);

    ofFbo p_draw, main_draw;

    VideoHandler video;

    ofxFaceTracker2 tracker;

    LandmarkCutManager cut_man;
     
    vector<CutParticleManager> p_men;

    bool show_info, enable_interval_export;
    unsigned long time, time_elapsed;

    ofxPanel gui;

    //@TODO: update these / tidy - this is all from previous sketches
    //      just getting baseline func in....
    ofxPanel p_man_gui;

    ofParameterGroup particleGui, repelGui, attractGui;
    ofParameter<ofColor> bg_c, pman_c;
    ofParameter<bool> enable_debug, enable_orig, enable_auto_spawn, enable_varying_gravity, enable_trails, enable_plain_draw;
    ofParameter<int> v_gravity_direction, particle_fbo_alpha;
    ofParameter<float> v_gravity_min, v_gravity_max;
    ofParameter<float> time_interval;

};
