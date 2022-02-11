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
void VideoHandler::setup(string _path, videoModes _mode) {

	mode = _mode;
	path = _path;
	vector<ofVideoDevice> cams;
	
	switch (mode) {
		case VIDEO_LOCAL :
			
			loadVideo();

			break;
		case VIDEO_WEBCAM :
			web_cam.close();
			web_cam.setVerbose(true);
			web_cam.setDeviceID(curr_feed);
			cams = web_cam.listDevices();
			feed_count = cams.size();
			web_cam.setup(dims.x, dims.y, true);
			break;
		case VIDEO_IP :
			ip_info = ofx::Video::IpVideoGrabberSettings::fromFile(_path);

			ofLogNotice("ofApp::setup()") << "Loaded " << ip_info.size() << " locations.";

			feed_count = ip_info.size();

			auto c = std::make_shared<ofx::Video::IPVideoGrabber>();
			auto& settings = getCam();
			c->setCameraName(settings.getName());
			c->setUsername(settings.getUsername());
			std::cout << settings.getUsername() << std::endl;
			c->setPassword(settings.getPassword());
			std::cout << settings.getPassword() << std::endl;
			c->setURI(settings.getURL());
			c->setPixelFormat(OF_PIXELS_RGBA);	//not hooked up to anything.. but i think why we cant set by pixels
			c->connect();

			ip_cam.reset();
			ip_cam = c;

			break;
	}

}

//--------------------------------------------------------------
void VideoHandler::loadVideo() {
	dir.listDir(path);
	dir.sort();
	feed_count = (int)dir.size();

	local_cam.stop();
	local_cam.close();
	local_cam.load(dir.getPath(curr_feed));
	local_cam.setVolume(0);
	local_cam.play();

	cout << feed_count << endl;
	cout << path << endl;
}

//--------------------------------------------------------------
void VideoHandler::update() {

	bool updated = false;

	switch (mode) {
	case VIDEO_LOCAL:
		local_cam.update();
		updated = local_cam.isFrameNew();
		break;
	case VIDEO_WEBCAM:
		web_cam.update();
		updated = web_cam.isFrameNew();
		break;
	case VIDEO_IP:
		ip_cam->update();
		updated = ip_cam->isFrameNew();
		break;
	}

	if (updated) {
			if (enable_resizing) {
				ofPushStyle();
				ofPushMatrix();
				output.begin();
				ofSetColor(bg_c);
				ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
				if (enable_mirror) {
					ofTranslate(ofGetWidth(), 0);
					ofRotateYDeg(180);
				}
				if (mode == 0)local_cam.draw(coords.x, coords.y, o_dims.x, o_dims.y);
				if (mode == 1)web_cam.draw(coords.x, coords.y, o_dims.x, o_dims.y);
				if (mode == 2)ip_cam->draw(coords.x, coords.y, o_dims.x, o_dims.y);
				output.end();
				ofPopMatrix();
				ofPopStyle();
			}
			else {
				if (mode == 0)frame.setFromPixels(local_cam.getPixels());
				if (mode == 1)frame.setFromPixels(web_cam.getPixels());
				if (mode == 2)frame.setFromPixels(ip_cam->getPixels());
				if (enable_mirror)frame.mirror(false, true);
				frame.update();
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
	else {
		glm::vec2 p(0,0), o = dims;
		frame.draw(p.x, p.y, o.x, o.y);
	}

	if (enable_videoinfo)
		drawCamInfo(ip_cam);
}

//--------------------------------------------------------------
void VideoHandler::nxtFeed() {
	curr_feed = (curr_feed + 1) % feed_count;
	cout << "curr feed = " << curr_feed << " - "<< getVideoTitle() << endl;
	setup(path, mode);
}

//--------------------------------------------------------------
void VideoHandler::prevFeed() {
	curr_feed = curr_feed -1 % feed_count;
	if (curr_feed < 0)curr_feed = feed_count -1;
	cout << "curr feed = " << curr_feed << " - " << getVideoTitle() << endl;
	setup(path, mode);
}

//--------------------------------------------------------------
ofx::Video::IpVideoGrabberSettings& VideoHandler::getCam() {
	return ip_info[curr_feed];
}

//--------------------------------------------------------------
void VideoHandler::setDims(glm::vec2 _dims) {
	dims = _dims;
	setup(path, mode);
}

//--------------------------------------------------------------
void VideoHandler::setOutputDims(glm::vec2 _dims) {
	if(_dims.x > _dims.y)
		o_dims = glm::vec2(_dims.y * (dims.x / dims.y), _dims.y);
	else
		o_dims = glm::vec2(_dims.x, _dims.x * ( dims.y / dims.x ));
	coords = getOutputCoords();
	output.allocate(o_dims.x, o_dims.y, GL_RGBA);
}

//--------------------------------------------------------------
void VideoHandler::setMode(videoModes _mode) {
	mode = _mode;
	setup(path, mode);
}

//--------------------------------------------------------------
void VideoHandler::setMode(videoModes _mode, string _path) {
	mode = _mode;
	path = _path;
	setup(path, mode);
}

//--------------------------------------------------------------
void VideoHandler::setDirectory(string _path) {
	path = _path;
	setup(path, mode);
}

//--------------------------------------------------------------
void VideoHandler::setActive( bool status) {
	active = status;
}

//--------------------------------------------------------------
glm::vec2& VideoHandler::getDims() {
	return dims;
}

//--------------------------------------------------------------
glm::vec2& VideoHandler::getODims() {
	return o_dims;
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
ofPixels VideoHandler::getFramePixels() {
	if (enable_resizing) {
		ofPixels pix;
		output.readToPixels(pix);
		return pix;
	}
	return frame.getPixels();
}

//--------------------------------------------------------------
ofPixels VideoHandler::getOriginalPixels() {
	switch (mode) {
	case VIDEO_LOCAL:
		return local_cam.getPixels();
		break;
	case VIDEO_WEBCAM:
		return web_cam.getPixels();
		break;
	case VIDEO_IP:
		return ip_cam->getPixels();
		break;
	}
}

//--------------------------------------------------------------
ofFbo& VideoHandler::getOutputFBO() {
	return output;
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
	switch (mode) {
	case VIDEO_LOCAL:
		return dir.getPath(curr_feed);
		break;
	default:
		return "live video feed";
		break;
	}
}

//--------------------------------------------------------------
bool VideoHandler::isFrameNew() {
	switch (mode) {
	case VIDEO_LOCAL:
		return local_cam.isFrameNew();
		break;
	case VIDEO_WEBCAM:
		return web_cam.isFrameNew();
		break;	
	case VIDEO_IP:
		return ip_cam->isFrameNew();
		break;
	default:
		return true;
		break;
	}
}

//--------------------------------------------------------------
bool VideoHandler::isActive() {
	return active;
}

//--------------------------------------------------------------
void VideoHandler::drawCamInfo(std::shared_ptr<ofx::Video::IPVideoGrabber>& g) {
	float kbps = g->getBitRate() / 1000.0f; // kilobits / second, not kibibits / second
	float fps = g->getFrameRate();

	std::stringstream ss;

	// ofToString formatting available in 0072+
	ss << "          NAME: " << g->getCameraName() << std::endl;
	ss << "          HOST: " << g->getHost() << std::endl;
	ss << "           FPS: " << ofToString(fps, 2/*,13,' '*/) << std::endl;
	ss << "          Kb/S: " << ofToString(kbps, 2/*,13,' '*/) << std::endl;
	ss << " #Bytes Recv'd: " << ofToString(g->getNumBytesReceived(), 0/*,10,' '*/) << std::endl;
	ss << "#Frames Recv'd: " << ofToString(g->getNumFramesReceived(), 0/*,10,' '*/) << std::endl;
	ss << "Auto Reconnect: " << (g->getAutoReconnect() ? "YES" : "NO") << std::endl;
	ss << " Needs Connect: " << (g->getNeedsReconnect() ? "YES" : "NO") << std::endl;
	ss << "Time Till Next: " << g->getTimeTillNextAutoRetry() << " ms" << std::endl;
	ss << "Num Reconnects: " << ofToString(g->getReconnectCount()) << std::endl;
	ss << "Max Reconnects: " << ofToString(g->getMaxReconnects()) << std::endl;
	ss << "  Connect Fail: " << (g->hasConnectionFailed() ? "YES" : "NO");

	ofSetColor(255);
	ofDrawBitmapString(ss.str(), 10, 10 + 12);
}

//--------------------------------------------------------------
void VideoHandler::initGui() {

	gui.setName("video controls");
	gui.add(c.set("video colour", ofColor(255, 255, 255, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));
	gui.add(bg_c.set("video bg colour", ofColor(255, 255, 255, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));
	gui.add(enable_video_bg.set("enable video bg", false));
	gui.add(enable_resizing.set("enable resizing", false));
	gui.add(enable_mirror.set("enable mirror", false));
	gui.add(enable_videoinfo.set("enable info", false));

}

