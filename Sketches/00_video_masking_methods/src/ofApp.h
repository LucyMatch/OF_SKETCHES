#pragma once

#include "ofMain.h"
#include "ofxGui.h"

enum MASK_METHOD {
	/// \Masking w/ a black + white matte video
	///	\Plays both videos and attempts to sync matte to original video
	MASK_PLAY = 0,
	/// \Masking w/ a black + white matte video
	/// \pauses both videos + progresses them frame by frame on update
	MASK_FRAME = 1,
	/// \Uses Green Screen Swizzle - TODO / TEST
	GREEN_SCREEN = 2
	//@TODO: additional methods - w/ cv
};
const int MASK_METHOD_ITEMS = 3;

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void drawDebug();

		void loadVideos();
		void setVideos();
		void closeVideos();
		void nextVideo();

		void nextMethod();

		void initGui();
		void drawGui(ofEventArgs& args);
		void drawInfo();
		void framerate();

		void keyPressed(int key);
		void mousePressed(int x, int y, int button);
		void windowResized(int w, int h);

		MASK_METHOD method;

		ofVideoPlayer video, matte;
		int curr_src = 0;

		ofFbo composite;
		ofTexture mask;

		ofxPanel gui;
		ofParameter<ofColor> bg_c;
		ofParameter<bool> 
			enable_debug = true,
			enable_info = false;
		ofParameter<int> composite_alpha;

};
