#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "IPVideoGrabber.h"
#include "ofxGui.h"

#define _WEBCAM 		//uncomment to use IP cam

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);

#ifdef _WEBCAM
		ofVideoGrabber cam;
#else
		ofx::Video::IPVideoGrabber cam;
#endif
	
		ofVec2f cam_dims;
		ofImage	vid;

		ofxPanel gui;
		bool gui_hide;
		ofParameter<ofColor> bg, vid_c;
		ofParameter<bool> draw_bg,draw_vid;
};

