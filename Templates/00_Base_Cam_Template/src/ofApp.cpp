#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

//--------------------------------------------------------------
void ofApp::setup(){

	ofSetVerticalSync(true);
	ofBackground(255, 255, 255);
	ofEnableAlphaBlending();
	ofSetBackgroundAuto(false);
	
		//gui
	gui.setup("main controls");
	gui.add(bg.set("background", ofColor(255,255,255,255),ofColor(0,0,0,0),ofColor(255,255,255,255)));
	gui.add(vid_c.set("video", ofColor(255,255,255,255),ofColor(0,0,0,0),ofColor(255,255,255,255)));
	gui.add(draw_bg.set("draw bg", true));
	gui.add(draw_vid.set("draw vid", true));
	
	gui_hide = false;
	
		//cam setup
	cam_dims.x = 640;
	cam_dims.y = 480;
	
#ifdef _WEBCAM
	cam.setup(cam_dims.x, cam_dims.y);
#else
		//	cam.setCameraName("handy cammy");
		//	cam.setURI("http://192.168.1.137/axis-cgi/mjpg/video.cgi");
	cam.setCameraName("Spy Cam");
	cam.setURI("http://192.168.1.161/mjpg/video.mjpg?streamprofile=sixteen_nine_03");
	cam.setUsername("ofAdmin");
	cam.setPassword("openframeworks");
	cam.connect();
#endif
	
	
}

//--------------------------------------------------------------
void ofApp::update(){

	cam.update();
	if(cam.isFrameNew()){
		vid.setFromPixels(cam.getPixels());
	}
	
}

//--------------------------------------------------------------
void ofApp::draw(){
	
		//background ctrl
	if(draw_bg){
		ofSetColor(bg);
		ofDrawRectangle(0,0,ofGetWidth(),ofGetHeight());
	}
	
		//drawing cam feeds
	if(draw_vid){
		ofSetColor(vid_c);
		cam.draw(0,0,ofGetWidth(),ofGetHeight());
	}
	
		//drawing cam feeds
	if(!gui_hide){
		gui.draw();
	}

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	
	switch (key) {
		case 'h':
			gui_hide = !gui_hide;
			break;
		case '1':
			gui.saveToFile("settings_1");
			break;
		case '2':
			gui.saveToFile("settings_2");
			break;
		case 'q':
			gui.loadFromFile("settings_1");
			break;
		case 'w':
			gui.loadFromFile("settings_2");
			break;
	}
	
}


