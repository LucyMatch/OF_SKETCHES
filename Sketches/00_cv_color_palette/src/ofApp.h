#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxCv.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void drawDebug();

		void updateActivePixel();
		bool inCamBounds(int x, int y);

		void drawViewFinder();
		void drawPalette();

		void initGUI();
		void drawGUI(ofEventArgs& args);

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		//CAM
		ofVideoGrabber cam;
		ofFbo cam_fbo;
		int cam_w = 1280, cam_h = 720;
			//opencv
		ofxCv::ContourFinder contourFinder;
		//std::vector<ofPolyline>& contours;


		//UI
		vector<ofColor> palette;
		ofColor active_pixel;

		//States
		enum Mode {INPUT, OUTPUT} curr_mode;
		bool debug = false;

		//GUI PRIMARY
		ofxPanel gui;
		ofParameter<ofColor> bg;

		ofxPanel gui_cv;
		ofParameter<float> threshold;
		ofParameter<bool> trackHueSat;
		
};


//@TODO:
//add frame per second count info box
//