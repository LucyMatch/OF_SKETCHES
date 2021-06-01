#pragma once

#include "ofMain.h"
#include "ofxGui.h"
//@TODO : //#include "IPVideoGrabber.h"

//#define _LOCAL			//@TODO: local video trigger & set up
#define _WEBCAM				//comment for ip cam ( to do )

//@TODO:
//			btns for looping through different ip cameras & webcams
//			stuff for local videos & looping through them
//			maybe eventually stuff regarding resolution / number of pixels
//			this scaling will become important!!
//			ip : cams + cycling through all the options and loading json file
//			--> this code is somehwere in sketch folder add to this class
//mimap?

class VideoHandler : public ofBaseApp {

	public :

		void setup();
		void update();
		void draw();

		void drawDebug();
		void initGui();

		//void updateDims();
		ofTexture& getFrameTex();
		ofImage& getFrameImg();

	#ifdef _WEBCAM
		ofVideoGrabber cam;
	#else 
		//todo
		ofx::Video::IPVideoGrabber cam;
	#endif

		glm::vec2 dims;
		ofImage frame;

		ofParameterGroup gui;
		ofParameter<ofColor> c, bg_c;
		ofParameter<bool> enable_video_bg;
};