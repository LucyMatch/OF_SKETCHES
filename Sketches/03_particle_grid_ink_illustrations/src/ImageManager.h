#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class ImageManager {

public:

	ImageManager();
	void init();
	void assetsLoad(string path);
	void draw();
	void assetsDrawRandom(); //todo
	void assetsAnimate();  //todo

	ofFbo& getImageLayer();

	vector<ofTexture> images;
	ofFbo image_layer;

	ofParameterGroup image_ctrl;
	ofParameter<float> img_alpha;

};