#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

//--------------------------------------------------------------
void ofApp::setup(){

	initGUI();

	ofSetFrameRate(60);
	ofSetVerticalSync(false);
	ofEnableAlphaBlending();

	cam.setDeviceID(0);
	cam.setup(cam_w, cam_h);
	cam_fbo.allocate(cam_w, cam_h, GL_RGBA);
	
	//add to gui
	//@TODO: we need a listener to "reset" when these vals are updated in gui
	contourFinder.setMinArea(10);
	contourFinder.setMaxAreaRadius(150);

	curr_mode = INPUT;


}

//--------------------------------------------------------------
void ofApp::update(){

	cam.update();
	cam_fbo.begin();
	//ofPushStyle();
	//@TODO: ratio + more control etc here
		cam.draw(0,0, cam_w, cam_h);
	//ofPopStyle();
	cam_fbo.end();

	if (curr_mode == INPUT || debug) updateActivePixel();

	//cv color finder
	if (curr_mode == OUTPUT) {
		//we can either have a contour finder for each palette colour.... 
		//or one contour finder + an vector to save them to - thats what get drawn
		//we can either have a vector of : ofPoints, ofPolys or Rect bounding boxes
		for (auto & c : palette) {
			contourFinder.setTargetColor(c, trackHueSat ? TRACK_COLOR_HS : TRACK_COLOR_RGB);
			contourFinder.setThreshold(threshold);
			contourFinder.findContours(cam);
			//contours = contourFinder.getPolylines();
		}

	}

}

//--------------------------------------------------------------
void ofApp::draw(){

	ofPushStyle();
	ofSetColor(bg);
	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
	ofPopStyle();

	cam_fbo.draw(0, 0,cam_w, cam_h);

	if (curr_mode == OUTPUT) {
		ofPushStyle();
			ofSetLineWidth(3);
			contourFinder.draw();
		ofPopStyle();
	}

	if (curr_mode == INPUT)drawViewFinder();

	drawPalette();

	if (debug)drawDebug();

}

//--------------------------------------------------------------
void ofApp::drawDebug() {
	drawViewFinder();
}

//--------------------------------------------------------------
void ofApp::updateActivePixel() {
	
		ofPixels pixels;
		cam_fbo.readToPixels(pixels);
		if (inCamBounds(mouseX, mouseY)) {
			active_pixel = pixels.getColor(mouseX, mouseY);
		}
}

//--------------------------------------------------------------
void ofApp::drawViewFinder() {

	float pad = 20, outline = 6, x = 50, y = 50, r = 200;

	ofPushStyle();
	ofSetColor(255, 255, 255, 255);
	ofDrawRectangle((x - outline), (y - outline), r + (outline * 2), r + (outline * 2));
	ofSetColor(active_pixel);
	ofDrawRectangle(x, y, r, r);
	ofPopStyle();
	
}

//--------------------------------------------------------------
void ofApp::drawPalette() {

	float pad = 20, outline = 6, x = 50, y = 300, r = 20;

	for (auto i = 0; i < palette.size(); i++) {
		ofPushStyle();
			ofSetColor(255, 255, 255, 255);
			ofDrawRectangle((x - outline), (y +((r+pad)*i)) - outline, r + (outline*2), r + (outline*2));
			ofSetColor(palette[i]);
			ofDrawRectangle(x, (y +((r+pad)*i)), r, r);
		ofPopStyle();
	}
}

//--------------------------------------------------------------
void ofApp::initGUI() {
	gui.setup("P R I M A R Y");
	gui.add(bg.set("background", ofColor(104, 166, 218, 80), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));
	
	gui_cv.setup("C V");
	gui_cv.add(threshold.set("threshold", 100, 0, 255));
	gui_cv.add(trackHueSat.set("Track Hue Sat", false));
}

//--------------------------------------------------------------
void ofApp::drawGUI(ofEventArgs& args) {
	gui.draw();
	gui_cv.draw();
}

//--------------------------------------------------------------
bool ofApp::inCamBounds(int x, int y) {
	if (x <= cam_w && y <= cam_h)return true;
	return false;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	switch (key) {
	case 'c' :
		//clear saved pallette
		palette.clear();
		break;
	case 's' : 
		//switch mode
		if (curr_mode == INPUT) {
			curr_mode = OUTPUT;
		}else{ curr_mode = INPUT; }
		break;
	case 'd':
		//toggle debug
		debug = !debug;
		break;
	default :
		break;
	}

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	if (curr_mode == INPUT && inCamBounds(x, y)) 
			palette.push_back(active_pixel);	
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
