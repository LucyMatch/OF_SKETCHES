/*VideoHandler v1*/
#pragma once

#include "ofMain.h"
#include "ofxGui.h"
//@TODO : //#include "IPVideoGrabber.h"

#define _LOCAL 0	//@TODO: local video trigger & set up
#define _WEBCAM	1   //comment for ip cam ( todo )

//@TODO:
//			btns for looping through different ip cameras & webcams
//			stuff for local videos & looping through them
// 
//			maybe eventually stuff regarding resolution / number of pixels
//			this scaling will become important!
// 
//			ip : cams + cycling through all the options and loading json file
//			--> this code is somehwere in sketch folder 
//			-->create an ipcam class that this class pulls in if ip is enabled
// 
//			additionally allow for stills in the system
// 
//mimap?

class VideoHandler {

	public :

		VideoHandler(glm::vec2 _dims = glm::vec2(640,480));

		void setup(string path = "videos");
		void update();
		void draw();

		void initGui();
		void setOutputDims(glm::vec2 _dims);
		void setDims(glm::vec2 _dims);
		void nxtFeed();
		void prevFeed();

		glm::vec2& getDims();
		glm::vec2& getOutputCoords();
		ofTexture* getFrameTex();
		ofImage& getFrameImg();
		string getVideoTitle();

	#if _LOCAL > 0
		ofVideoPlayer cam;
	#elif _WEBCAM > 0
		ofVideoGrabber cam;
	#else 
		//todo
		ofx::Video::IPVideoGrabber cam;
	#endif

		int curr_feed = 0, feed_count;
		ofDirectory dir;

		glm::vec2 dims, o_dims, coords;
		ofImage frame, output_frame;

		ofFbo output;

		ofParameterGroup gui;
		ofParameter<ofColor> c, bg_c;
		ofParameter<bool> enable_video_bg, enable_resizing;
};