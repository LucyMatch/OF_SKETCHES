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

    void keyPressed(int key);
    void mouseMoved(int x, int y);

    VideoHandler video;

    ofxFaceTracker2 tracker;

    //testing singular cut
    //PolyCuts left_eye, right_eye, mouth;
    LandmarkCutManager cut_man;

    ofxPanel gui;
    ofParameter<ofColor> bg_c;
    ofParameter<bool> enable_debug, enable_orig;
    ofParameter<int> blend_mode;

    std::map<int, ofBlendMode> blends{
	    {0, OF_BLENDMODE_DISABLED },
	    {1, OF_BLENDMODE_ALPHA },
	    {2, OF_BLENDMODE_ADD },
	    {3, OF_BLENDMODE_SUBTRACT },
	    {4, OF_BLENDMODE_MULTIPLY },
	    {5, OF_BLENDMODE_SCREEN },
    };
};
