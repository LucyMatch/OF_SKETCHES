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
	ofSetBackgroundColor(255);
	ofSetBackgroundAuto(false);

	//@TODO: move to GUI
	//& have a listener that reinits the grid
	//&reinits the block managers
	canvas_margin = 0.15;
	cell_margin = 0.12;
	rows = 3;
	cols = 3;

	spawning_active = true;

	initGrid();
	initParticle();
}

//--------------------------------------------------------------
void ofApp::update(){
	if(spawning_active)spawn();
	framerate();
	for (auto& p : pman) { p.update(); }
}

//--------------------------------------------------------------
void ofApp::draw(){

	ofPushStyle();
	ofSetColor(255, 50);
	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
	ofPopStyle();

	//drawDebug();

	for (auto& p : pman) { p.draw();}

}

//--------------------------------------------------------------
void ofApp::drawDebug() {

	for (auto& c : cells) {
		ofPushStyle();
		ofSetColor(171, 196, 237);
		ofDrawRectangle(c.x, c.y, c.width, c.height);
		ofPopStyle();
	}

	for (auto& p : pman) { p.drawDebug(); }


}

//--------------------------------------------------------------
void ofApp::initGrid() {

	cells.clear();

	//create "grid" coordinates
	//canvas
	auto width_margin = ofGetWidth() * canvas_margin;
	auto height_margin = ofGetHeight() * canvas_margin;
	auto w = ofGetWidth() - (2 * width_margin);
	auto h = ofGetHeight() - (2 * height_margin);
	canvas.set(width_margin, height_margin, w, h);
	//cells
	auto cell_w = ((1.0 / float(cols)) * float(w));
	auto cell_h = ((1.0 / float(rows)) * float(h));
	auto cell_w_m = cell_w * cell_margin;
	auto cell_h_m = cell_h * cell_margin;
	ofVec2f cell((cell_w - (2 * cell_w_m)), (cell_h - (2 * cell_h_m)));

	//save the cells
	for (int x = 0; x < cols; x++) {
		for (int y = 0; y < rows; y++) {
			auto x_step = (canvas.x + cell_w_m) + (x * cell_w);
			auto y_step = (canvas.y + cell_h_m) + (y * cell_h);
			ofRectangle c(x_step, y_step, cell.x, cell.y);
			cells.push_back(c);
		}
	}

}

//--------------------------------------------------------------
void ofApp::initParticle() {
	for (auto& c : cells) {
		BlockManager p(2000);
		p.setup(glm::vec4(c.x, c.y,c.width, c.height));
		pman.push_back(p);
	}
}

//--------------------------------------------------------------
void ofApp::spawn() {
	for (auto& p : pman) {
		p.simpleSpawn();
	}
}

//--------------------------------------------------------------
void ofApp::gui() {

	// Setup RUI
	RUI_SETUP();
	RUI_LOAD_FROM_XML();

	//RUI_NEW_GROUP("GENERAL");
	//RUI_SHARE_PARAM_WCN("Draw Debug", drawDebug);
	//RUI_SHARE_COLOR_PARAM_WCN("void colour", interactive.void_color);

	//RUI_NEW_GROUP("GRID");
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
	ofxSuperLog::getLogger()->setScreenLoggingEnabled(false);

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
	switch (key) {
	case 's' :
		spawning_active = !spawning_active;
		break;
	case 't':
		for (auto& p : pman) {
			auto mid_x = p.draw_dims.x + (p.draw_dims.z / 2);
			auto mid_y = p.draw_dims.y + (p.draw_dims.w / 2);
			p.applySeek(ofVec2f(mid_x, (p.draw_dims.y + p.draw_dims.w)));
		}
		break;
	case 'g':
		for (auto& p : pman) {
			for (auto& _p : p.p) {
				float mid_x = ofGetWindowWidth();
				if (_p.location.x < (p.draw_dims.x + (p.draw_dims.z / 2)))
					mid_x = 0;
				_p.seek(ofVec2f(mid_x, _p.location.y));
			}
		}
		break;
	case 'x':
		ofxSuperLog::getLogger()->setScreenLoggingEnabled(false);
		break;
	case 'z':
		ofxSuperLog::getLogger()->setScreenLoggingEnabled(true);
		break;
	default :
		break;
	}
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
