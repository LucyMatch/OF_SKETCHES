#pragma once

#include "ofMain.h"
#include "Videohandler.h"
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
		void drawInfo();

		void keyPressed(int key);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void windowResized(int w, int h);

		glm::vec2 canvas_dims;

		VideoHandler video;
		ShapeDetector shape;

		//gui + controlls
		//---------------
		
		ofxPanel gui;
		ofParameter<ofColor> bg_c;
		ofParameter<bool> enable_debug = false, 
			enable_info = false, 
			enable_bg_video = false, 
			enable_shape_data = false;

};
