/*
	Local Media Manager Example
	Lucy Matchett - 2022

	Simple usage of Local Media Manager
	+ environment for testing + adding functions

*/

#pragma once

#include "ofMain.h"
#include "LocalMediaManager.h"
#include "ofxGui.h"
#include "VideoHandler.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void initGui();
		void drawGui(ofEventArgs& args);
		void framerate();
		void drawDebug();
		void drawInfo();

		void keyPressed(int key);
		void mouseDragged(int x, int y, int button);
		void windowResized(int w, int h);

		glm::vec2 canvas_dims;

		LocalMediaManager media_man;
		VideoHandler video;

		Feed* local_default_video_feed = NULL;
		Feed* local_default_video_feed_2 = NULL;

		Feed* local_default_image_feed = NULL;
		Feed* local_image_collection = NULL;

		vector<Feed*> local_image_feeds;


		ofxPanel gui;
		ofParameter<ofColor> bg_c, c;
		ofParameter<bool> enable_debug = false, enable_info = false;
		
};
