#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxFaceTracker2.h"
#include "LocalMediaManager.h"
#include "VideoHandler.h"
#include "PolyCuts.h"
#include "LandmarkCutManager.h"
#include "CutParticleManager.h"

class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();

    void updateCutElements();

    void checkPmen();
    void updatePmen( int index,  int i, int j );

    void initGui();
    void updatePMenGui(int amt);
    void drawGui(ofEventArgs& args);
    void framerate();
    void drawDebug();
    void drawInfo();

    void intervalExport();

    void keyPressed(int key);
    void mouseMoved(int x, int y);

    glm::vec2 canvas_dims;

    ofFbo main_draw, cuts_draw, p_draw;

    LocalMediaManager media_man;
    Feed* bg_feed = NULL;

    VideoHandler video;

    ofxFaceTracker2 tracker;

    LandmarkCutManager cut_man;

    vector<CutParticleManager> p_men;

    bool show_info, enable_interval_export;
    unsigned long time, time_elapsed;

    ofxPanel gui;
    ofxPanel p_man_gui;
    ofParameterGroup particleGui;

    ofParameter<ofColor> bg_c, cuts_draw_color, cuts_color, p_color;
    ofParameter<bool> enable_debug, enable_orig, enable_cuts_draw, enable_p_draw, enable_trails;
    ofParameter<float> time_interval, cuts_draw_alpha, p_draw_alpha;

    ofParameterGroup feed_gui;
    ofParameter<ofColor> bg_feed_c;
    ofParameter<bool> enable_bg_feed, enable_bg_resize;
};
