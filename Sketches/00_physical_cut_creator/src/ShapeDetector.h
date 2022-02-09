/*
	v 1 - Shape Detector
	Lucy Matchett - 2022

	Handles Blob detection via bg learning method
	Goal is to create a real time input for digital collages
	This class will detect "cut shapes" within a field
	so these can be used to mask digital textures etc...

	@TODO:
	* Blob Modifications / simplifications / smoothing etc

*/
#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxOpenCv.h"

class ShapeDetector {

public :

	ShapeDetector();

	void update( ofPixels p );
	void update( ofTexture tex );
	void draw();

	void drawVideo();
	void drawData();
	void drawDebug();
	void initGui();
	void sizeImgs(int w, int h);

	void setOutputDims(glm::vec2 d);
	void calcOutputDims();
	glm::vec2 getCurrentDims();

	void setFOV(bool state);
	void setFOV(int x, int y);
	void drawFOV();
	void drawLiveFOVConfig(int x, int y);



	ofxCvContourFinder 	contourFinder;
	ofxCvColorImage			colorImg;
	ofxCvGrayscaleImage 	grayImage;
	ofxCvGrayscaleImage 	grayBg;
	ofxCvGrayscaleImage 	grayDiff;

	ofParameterGroup gui;
	ofParameter<int> diff_thresh;
	ofParameter<int> cmin, cmax, considered;
	ofParameter<bool> choles, capprox, enable_FOV, enable_fullscreen;

	//key press vars
	bool learn_background = true, set_canvas = false;

	glm::vec2 output_dims, fov_output_dims;
	ofRectangle FOV = ofRectangle(0, 0, 100, 100);
	bool FOV_origin = false;

};