#include "VideoHandler.h"

//--------------------------------------------------------------
VideoHandler::VideoHandler(glm::vec2 _dims) {
	dims = _dims;
	initGui();
}

//--------------------------------------------------------------
void VideoHandler::setup() {

#ifdef _WEBCAM
	cam.close();
	cam.setVerbose(true);
	cam.setDeviceID(curr_feed);

	vector<ofVideoDevice> cams = cam.listDevices();
	//cout << "listing cams myself" << endl;
	//cout << cams.size() << endl;
	//for (auto i : cams)
	//	cout << i.deviceName << endl;

	feed_count = cams.size();
		
	cam.setup(dims.x, dims.y, true);

#else
	//@TODO:
	// 	   //make a class from ip cam sketch
	// 	   //so we can load a bunch and cycle through which we are "pulling"
	//	cam.setCameraName("handy cammy");
	//	cam.setURI("http://192.168.1.137/axis-cgi/mjpg/video.cgi");
	//cam.setCameraName("Spy Cam");
	//cam.setURI("http://192.168.1.161/mjpg/video.mjpg?streamprofile=sixteen_nine_03");
	//cam.setUsername("ofAdmin");
	//cam.setPassword("openframeworks");
	//cam.connect();
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
	cam.draw(0, 0);

}

//--------------------------------------------------------------
void VideoHandler::nxtFeed() {
	curr_feed = ++curr_feed % feed_count;
	cout << "curr fedd = " << curr_feed << endl;
	setup();
	
}

//--------------------------------------------------------------
void VideoHandler::prevFeed() {
	curr_feed = --curr_feed % feed_count;
	cout << "curr fedd = " << curr_feed << endl;
	setup();
}


//--------------------------------------------------------------
void VideoHandler::setDims(glm::vec2 _dims) {
	dims = _dims;
	setup();
}

//--------------------------------------------------------------
glm::vec2& VideoHandler::getDims() {
	return dims;
}

//--------------------------------------------------------------
ofTexture* VideoHandler::getFrameTex() {
	return &frame.getTexture();
}

//--------------------------------------------------------------
ofImage& VideoHandler::getFrameImg() {
	return frame;
}

//--------------------------------------------------------------
void VideoHandler::initGui() {

	gui.setName("video controls");
	gui.add(c.set("video colour", ofColor(255, 255, 255, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));
	gui.add(bg_c.set("video bg colour", ofColor(255, 255, 255, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));
	gui.add(enable_video_bg.set("enable video bg", false));

}
