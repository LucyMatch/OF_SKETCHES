/*
	v 2 - Video Handler
	Lucy Matchett - 2022

	Handles Video
	Local + live feeds

	@TODO:
	* Ip Cameras
	* Resolution options
	* Scaling options
	* ip : cams + cycling through all the options and loading json file
		--> this code is somehwere in sketch folder
		-->create an ipcam class that this class pulls in if ip is enabled
	* other format / codec options + support
	* Play controls - loop speed etc...
	* Add proper deconstructor

*/
#pragma once

#include "ofMain.h"
#include "ofxGui.h"
//@TODO : //#include "IPVideoGrabber.h"



class VideoHandler {

	public :

		enum videoModes {
			/// \Local Video from Directory
			VIDEO_LOCAL = 0,
			/// \Live Feed From Connected Webcams
			VIDEO_WEBCAM = 1,
			/// \Live Feed From Connected IPCams (@TODO)
			VIDEO_IP = 2
		};

		VideoHandler(glm::vec2 _dims = glm::vec2(640,480));

		void setup(string _path = "videos", videoModes _mode = VIDEO_WEBCAM );
		void loadVideo();
		void update();
		void draw();

		void initGui();
		void nxtFeed();
		void prevFeed();

		void setOutputDims(glm::vec2 _dims);
		void setDims(glm::vec2 _dims);
		void setMode(videoModes _mode);
		void setMode(videoModes _mode, string _path);
		void setDirectory(string _path);
		void setActive(bool status);

		glm::vec2& getDims();
		glm::vec2& getOutputCoords();
		ofTexture* getFrameTex();
		ofImage& getFrameImg();
		string getVideoTitle();
		bool isFrameNew(); 
		bool isActive();

		ofParameterGroup gui;
		ofParameter<ofColor> c, bg_c;
		ofParameter<bool> enable_video_bg, enable_resizing, enable_mirror;

	private :

		bool active = true;

		ofVideoPlayer local_cam;
		ofVideoGrabber web_cam;
		//ofx::Video::IPVideoGrabber ip_cam;

		//input
		videoModes mode;
		int curr_feed = 0, feed_count;

		//local input
		string path;
		ofDirectory dir;

		//output
		glm::vec2 dims, o_dims, coords;

		ofImage frame, output_frame;
		ofFbo output;


};