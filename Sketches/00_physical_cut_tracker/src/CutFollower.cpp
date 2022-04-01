#include "CutFollower.h"

using namespace ofxCv;
using namespace cv;

const float dyingTime = 1;

//--------------------------------------------------------------
void CutFollower::setup(const cv::Rect& track) {
	//random colour - if we can access should set a function to set colour 
	color.set(255,255,255);
	cur = toOf(track).getCenter();
	smooth = cur;
}
//--------------------------------------------------------------
void CutFollower::update(const cv::Rect& track) {
	cur = toOf(track).getCenter();
	smooth.interpolate(cur, .5);
	history.addVertex(smooth);
}
//--------------------------------------------------------------
void CutFollower::update(ofPolyline poly) {
	shape = poly;
}
//--------------------------------------------------------------
void CutFollower::setColor(ofColor c) {
	color = c;
	color_set = true;
}
//--------------------------------------------------------------
void CutFollower::kill() {
	float curTime = ofGetElapsedTimef();
	if (startedDying == 0) {
		startedDying = curTime;
	}
	else if (curTime - startedDying > dyingTime) {
		dead = true;
	}
}
//--------------------------------------------------------------
void CutFollower::draw() {
	ofPushStyle();
	ofSetColor(color);
	shape.draw();
	ofPopStyle();
}
//--------------------------------------------------------------
void CutFollower::drawDebug() {
	ofPushStyle();
	if (startedDying) {
		ofSetColor(ofColor::red);
	}
	else {
		ofSetColor(color);
	}
	ofDrawRectangle(cur, 20, 20);
	history.draw();
	shape.draw();
	ofSetColor(255);
	ofDrawBitmapString(ofToString(label), cur);
	ofPopStyle();
}
//--------------------------------------------------------------
bool CutFollower::isColorSet() {
	return color_set;
}