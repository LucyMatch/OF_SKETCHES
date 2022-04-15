/*
	v 2 - Video Handler
	Lucy Matchett - 2022

	Handles Video
	Local + live feeds

	@TODO:
	* Resolution / scaling need a clear approach! 
	*	will have to retro fit older sketches but worth it
	* other format / codec options + support
	* Play controls - loop speed etc...
	* Add proper deconstructor

*/
#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "IPVideoGrabber.h"

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
		void loadDirectory();
		void update();
		void draw();

		void initGui();
		int nxtFeed();
		int prevFeed();

		int setFeed( int index, bool reload = true);
		void setOutputDims(glm::vec2 _dims);
		void setDims(glm::vec2 _dims);
		void setMode(videoModes _mode, bool reload = true);
		void setMode(videoModes _mode, string _path, bool reload = true);
		void setDirectory(string _path, bool reload = true);
		void setActive(bool status);
		void setVolume(int);
		

		//specific for ip cam
		void drawCamInfo(std::shared_ptr<ofx::Video::IPVideoGrabber>& g);
		ofx::Video::IpVideoGrabberSettings& getCam();


		glm::vec2& getDims();
		glm::vec2& getODims();
		glm::vec2& getOutputCoords();
		ofTexture* getFrameTex();
		ofImage& getFrameImg();
		ofPixels getFramePixels();
		ofPixels getOriginalPixels();
		ofFbo& getOutputFBO();
		string getVideoTitle();
		bool isFrameNew(); 
		bool isActive();
		bool isFrameAllocated();

		ofParameterGroup gui;
		ofParameter<ofColor> c, bg_c;
		ofParameter<bool> enable_video_bg, enable_resizing, enable_mirror, enable_videoinfo;

		//should be private but being lazy
		ofVideoPlayer local_cam;
		ofVideoGrabber web_cam;
		std::shared_ptr<ofx::Video::IPVideoGrabber> ip_cam;
		std::vector<ofx::Video::IpVideoGrabberSettings> ip_info;

	private :

		bool active = false;

		//input
		videoModes mode;
		int curr_feed = 0, feed_count = NULL;

		//local input
		string path;
		ofDirectory dir;

		//output
		glm::vec2 dims, o_dims, coords;

		ofImage frame, output_frame;
		ofFbo output;


};