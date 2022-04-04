#pragma once

#include "ofMain.h"
#include "ofxCv.h"



class CutFollower : public ofxCv::RectFollower {
protected:
	ofColor color;
	bool color_set = false;
	ofVec3f cur, smooth;
	float startedDying;
	ofPolyline history;	
	ofPolyline shape;
public:
	//--------------------------------------------------------------
	CutFollower()
		:startedDying(0) {
	}
	//--------------------------------------------------------------
	void setup(const cv::Rect& track);
	void update(const cv::Rect& track);
	void update(ofPolyline poly);
	void setColor(ofColor c);
	bool isColorSet();
	void kill();
	void draw();
	void drawDebug();
	ofPolyline getShape();
	ofPolyline getHistory();
};