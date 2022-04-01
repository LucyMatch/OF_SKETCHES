#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxCv.h"
#include "CutFollower.h"
#include "ColourPalette.h"
#include "VideoHandler.h"
#include "ShapeDetector.h"


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void initGui();	//called by main
		void drawGui(ofEventArgs& args);
		void framerate();
		void drawDebug();
		void drawPalette();
		void drawInfo();

		void keyPressed(int key);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void windowResized(int w, int h);

		VideoHandler video_input;	//primary video input source that we track contours on
		ShapeDetector shape_detector;

		ofxCv::RectTrackerFollower<CutFollower> tracker;

		ColourPalette palettes;
		int bg_c_index = 0;

		ofxPanel gui;
		ofParameter<ofColor> bg_c;
		ofParameter<bool> enable_debug = false,
			enable_info = false,
			enable_shape_data = false,
			enable_bg_video = false,
			enable_palette_preview = false;
		ofParameter<int> curr_palette;
		
};
