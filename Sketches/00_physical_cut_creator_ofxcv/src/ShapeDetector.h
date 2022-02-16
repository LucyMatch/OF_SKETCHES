/*
	v 2 - Shape Detector
	Lucy Matchett - 2022

	this version uses ofxcv
	must be with of 10.

	Handles Blob detection via bg learning method
	Goal is to create a real time input for digital collages
	This class will detect "cut shapes" within a field
	so these can be used to mask digital textures etc...
	
	@TODO: tracking handling + pointers so we can associate blobs with "cuts"

*/
#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxCv.h"

class ShapeDetector {

public :

	ShapeDetector();

	void update( ofPixels p );
	void update( ofTexture tex );

	void drawPaths();
	void drawShapes();
	void drawInput();
	void drawData();
	void drawDebug();

	void initGui();
	void sizeImgs(int w, int h);

	void setFullscreenDims(glm::vec2 d);
	void calcFullscreenDims();
	glm::vec2 getCurrentDims();

	void setFOV(bool state);
	void setFOV(int x, int y);
	void drawFOV();
	void drawLiveFOVConfig(int x, int y);

	void setPalette(vector<ofColor> p);

	//callbacks / listeners
	void updateContourMode(bool& enable_color_track);

	//cv objs
	ofxCv::RunningBackground background;
	ofImage bg_thresh_result;
	ofxCv::ContourFinder finder;
	ofColor target_colour = ofColor(255,255,255);

	//canvas etc
	ofTexture output_frame;
	glm::vec2 current_frame_dims;
	glm::vec2 fullscreen_dims;
	ofRectangle FOV = ofRectangle(0, 0, 100, 100);
	bool FOV_origin = false;

	//colours for fun drawing
	vector<ofColor> palette;

	//GUI
	ofParameterGroup gui;
	ofParameter<int> cthreshold, cmin, cmax;
	ofParameter<bool> choles, cinvert;
	ofParameter<bool> enable_bg_learning, enable_colour_track, colour_track_mode;
	ofParameter<float> bg_threshold, bg_learning_time;
	ofParameter<bool> enable_FOV, enable_fullscreen, enable_manual_scale;
	ofParameter<float> manual_scale, outline_width;

	//key press vars
	bool learn_background = true, set_canvas = false;

};