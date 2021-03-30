#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofBackground(0, 0, 0);

	ip_info = ofx::Video::IpVideoGrabberSettings::fromFile("cams.json");

	ofLogNotice("ofApp::setup()") << "Loaded " << ip_info.size() << " locations.";

	//print out the cams + info for now to check

	for (auto ip : ip_info) {
		//ofLogNotice("IP CAMS") << "Loaded " << ip;
	}

	//init mode in default single mode
	curr_mode = SINGLE;

	loadCams();

}

//--------------------------------------------------------------
void ofApp::update(){

	for (auto& grabber : ip_grabbers) {
		grabber->update();
	}

}

//--------------------------------------------------------------
void ofApp::draw(){

	//@TODO : 
	// - taking aspect ratio into account!!
	// - draw each cam on their own FBO
	// - customizability on grid
	// - making groid smarter based on # of cams

	int r = 0, c = 0;
	float x = 0, y = 0, totalKbps = 0, totalFPS = 0;

	float w = ofGetWidth() / cols;
	float h = ofGetHeight() / rows;

	for (auto& g : ip_grabbers) {
		x = c * w;
		y = r * h;

		//grid
		r = (r + 1) % rows;
		if (r == 0) c = (c + 1) % cols;

		ofPushMatrix();
		ofPushStyle();

			ofTranslate(x, y);
			ofSetColor(255, 255, 255, 255);
			g->draw(0, 0, w, h);

			ofEnableAlphaBlending();

			drawCamInfo(g);

			ofDisableAlphaBlending();

		ofPopStyle();
		ofPopMatrix();
	}

}

//--------------------------------------------------------------
void ofApp::loadCams() {

	ip_grabbers.clear();

	switch (curr_mode) {
		case SINGLE :
			cam_count = 1;
			rows = 1;
			cols = 1;
			break;
		case GRID :	
			cam_count = ip_info.size();
			//could make this grid calc smart based off size
			rows = 3;
			cols = 3;
			break;
		default :
			break;
	}

	for (auto i = 0; i < cam_count; i++) {

		auto c = std::make_shared<ofx::Video::IPVideoGrabber>();
		auto& settings = nextCam();
		c->setCameraName(settings.getName());
		c->setUsername(settings.getUsername());
		std::cout << settings.getUsername() << std::endl;
		c->setPassword(settings.getPassword());
		std::cout << settings.getPassword() << std::endl;
		c->setURI(settings.getURL());
		c->connect();

		ip_grabbers.push_back(c);
	}

}

//--------------------------------------------------------------
ofx::Video::IpVideoGrabberSettings& ofApp::nextCam() {
	curr_cam = (curr_cam + 1) % ip_info.size();
	return ip_info[curr_cam];
}

void ofApp::drawCamInfo(std::shared_ptr<ofx::Video::IPVideoGrabber> &g) {
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
void ofApp::keyPressed(int key){

	switch (key) {
	case 's':
		curr_mode = SINGLE;
		loadCams();
		break;
	case 'g':
		curr_mode = GRID;
		loadCams();
		break;
	default:
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
