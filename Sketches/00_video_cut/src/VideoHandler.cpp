#include "VideoHandler.h"

//--------------------------------------------------------------
void VideoHandler::setup() {

	initGui();

	//cam setup
	dims.x = 640;
	dims.y = 480;

#ifdef _WEBCAM
	cam.setup(dims.x, dims.y);
#else
	//@TODO:
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
void VideoHandler::update() {

	cam.update();
	if (cam.isFrameNew())
		frame.setFromPixels(cam.getPixels());

}

//--------------------------------------------------------------
void VideoHandler::draw() {

	//background ctrl
	if (enable_video_bg) {
		ofSetColor(bg_c);
		ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
	}

	ofSetColor(c);
	cam.draw(0, 0, ofGetWidth(), ofGetHeight());


}

//--------------------------------------------------------------
void VideoHandler::drawDebug() {

}

ofTexture& VideoHandler::getFrameTex() {
	return frame.getTexture();
}

ofImage& VideoHandler::getFrameImg() {
	return frame;
}

//--------------------------------------------------------------
void VideoHandler::initGui() {

	gui.setup("main controls");
	gui.add(c.set("video colour", ofColor(255, 255, 255, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));
	gui.add(bg_c.set("video bg colour", ofColor(255, 255, 255, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));
	gui.add(enable_video_bg.set("enable video bg", false);

}
