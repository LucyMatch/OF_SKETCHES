#include "VideoHandler.h"

//--------------------------------------------------------------
VideoHandler::VideoHandler(glm::vec2 _dims) {
	dims = _dims;
	o_dims = _dims;
	coords = glm::vec2(0, 0);
	setOutputDims(_dims);
	initGui();
}

//--------------------------------------------------------------
void VideoHandler::setup(string path) {

#if _LOCAL > 0

	dir.listDir(path);
	dir.sort();

	feed_count = (int)dir.size();

	cam.load(dir.getPath(curr_feed));
	cam.setVolume(0);
	cam.play();

#elif _WEBCAM > 0
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

	//cout << cam.isLoaded() << endl;

	cam.update();
	if (cam.isFrameNew()) {
		if (enable_resizing) {
			output.begin();
				ofSetColor(bg_c);
				ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
				cam.draw(coords.x, coords.y, o_dims.x, o_dims.y);
			output.end();
		}
		else {
			frame.setFromPixels(cam.getPixels());
		}
	}

}

//--------------------------------------------------------------
void VideoHandler::draw() {

	//background ctrl
	if (enable_video_bg) {
		ofSetColor(bg_c);
		ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
	}

	ofSetColor(c);
	if (enable_resizing) {output.draw(0,0);}
	else {cam.draw(0,0);}

}

//--------------------------------------------------------------
void VideoHandler::nxtFeed() {
	curr_feed = ++curr_feed % feed_count;
	cout << "curr feed = " << curr_feed << endl;
	setup();
}

//--------------------------------------------------------------
void VideoHandler::prevFeed() {
	curr_feed = --curr_feed % feed_count;
	if (curr_feed < 0)curr_feed = feed_count -1;
	cout << "curr feed = " << curr_feed << endl;
	setup();
}


//--------------------------------------------------------------
void VideoHandler::setDims(glm::vec2 _dims) {
	dims = _dims;
	setup();
}

//--------------------------------------------------------------
void VideoHandler::setOutputDims(glm::vec2 _dims) {
	o_dims = glm::vec2(_dims.y * (dims.x / dims.y), _dims.y);
	coords = getOutputCoords();
	output.allocate(o_dims.x, o_dims.y, GL_RGBA);
}

//--------------------------------------------------------------
glm::vec2& VideoHandler::getDims() {
	return dims;
}

//--------------------------------------------------------------
glm::vec2& VideoHandler::getOutputCoords() {
	return glm::vec2((ofGetWidth() - o_dims.x) / 2, (ofGetHeight() - o_dims.y) / 2);
}

//--------------------------------------------------------------
ofTexture* VideoHandler::getFrameTex() {
	if ( enable_resizing )return &output.getTexture();
	return &frame.getTexture();
}

//--------------------------------------------------------------
ofImage& VideoHandler::getFrameImg() {
	if (enable_resizing) {
		ofPixels pix;
		output.readToPixels( pix );
		output_frame.setFromPixels( pix );
		return output_frame;
	}
	return frame;
}

//--------------------------------------------------------------
string VideoHandler::getVideoTitle() {
#if _LOCAL > 0
	return dir.getPath(curr_feed);
#else 
	return "live video feed";
#endif
}



//--------------------------------------------------------------
void VideoHandler::initGui() {

	gui.setName("video controls");
	gui.add(c.set("video colour", ofColor(255, 255, 255, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));
	gui.add(bg_c.set("video bg colour", ofColor(255, 255, 255, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));
	gui.add(enable_video_bg.set("enable video bg", false));
	gui.add(enable_resizing.set("enable resizing", true));

}

