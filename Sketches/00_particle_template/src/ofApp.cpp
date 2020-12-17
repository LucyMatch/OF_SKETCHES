#include "ofApp.h"
#include "ofxSuperLog.h"

//--------------------------------------------------------------
void ofApp::setup(){
	logger();

	ofSetWindowShape(800, 800);
	ofSetFrameRate(60);
	ofSetVerticalSync(false);
	ofSetCircleResolution(100);
	ofEnableSmoothing();
	ofEnableAlphaBlending();
}

//--------------------------------------------------------------
void ofApp::update(){

	framerate();

}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::gui() {

	// Setup RUI
	RUI_SETUP();
	RUI_LOAD_FROM_XML();

	//RUI_NEW_GROUP("GENERAL");
	//RUI_SHARE_PARAM_WCN("Draw Debug", drawDebug);
	//RUI_SHARE_COLOR_PARAM_WCN("void colour", interactive.void_color);

	//RUI_NEW_GROUP("BLOB");
	//RUI_SHARE_PARAM_WCN("Blob Min Radius", Blob::r_min, 0, 1000);
	//RUI_SHARE_PARAM_WCN("Blob Max Radius", Blob::r_max, 0, 1000);
	//RUI_SHARE_PARAM_WCN("Blob Die Increment", Blob::die_rate, 0.0, 100);

} 

//--------------------------------------------------------------
void ofApp::logger() {
	string fileLoggingDirectory = "logs";
	bool logToConsole = true;
	bool logToScreen = true;

	ofSetLoggerChannel(ofxSuperLog::getLogger(logToConsole, logToScreen, "logs"));

	ofLogNotice() << "This is logging to your screen and ";
	ofLogNotice() << "to a file with a timestamped file ";
	ofLogNotice() << "in data/logs, and to your console.";
	ofLogNotice() << "";
	ofLogNotice() << "you can choose where to log to when";
	ofLogNotice() << "you set the logger channel.";
	ofLogNotice() << "";
	ofLogNotice() << "You can also minimize the log window ";
	ofLogNotice() << "by pressing the little x, or make it";
	ofLogNotice() << "wider by dragging the left bar";
	ofxSuperLog::getLogger()->setMaximized(false);

	ofBackground(20, 20, 20);
	ofSetFrameRate(60);
}

//--------------------------------------------------------------
void ofApp::framerate() {
	std::stringstream strm;
	strm << "fps: " << ofGetFrameRate();
	ofSetWindowTitle(strm.str());
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
