#include "ofApp.h"
#include "ofxSuperLog.h"

//--------------------------------------------------------------
void ofApp::setup(){
	logger();

	initGui();

	//ofSetWindowShape(800, 800);
	ofSetFrameRate(60);
	ofSetVerticalSync(false);
	ofSetCircleResolution(100);
	ofEnableSmoothing();
	ofEnableAlphaBlending();
	//ofSetBackgroundColor(255);
	ofSetBackgroundAuto(false);

	//@TODO: move to GUI
	//& have a listener that reinits the grid
	//&reinits the block managers
	canvas_margin = 0.15;
	cell_margin = 0.12;
	rows = 1;
	cols = 1;

	spawning_active = false;

	initCanvasGrid();
	initParticleMans();
}

//--------------------------------------------------------------
void ofApp::update(){
	if(spawning_active)spawn();
	framerate();
	for (auto& p : pman) { p.update(); }

	//i'm applying a home seek on every update
	//we want a "magnet" drawing it back home
	for (auto& p : pman) {
		//p.applySeek();
		//@TODO: add arrive to manager.. being lazy rn
		for (auto& _p : p.p) {
			//toggle for arrive?
			_p.seek(_p.home_location);
			if (_p.arrive_on)_p.arrive(_p.home_location);
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw(){

	ofPushStyle();
	ofSetColor(background);
	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
	ofPopStyle();

	ofPushStyle();
	if(enableDebug)drawDebug();
	ofPopStyle();

	for (auto& p : pman) { p.draw(); }
	

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

	repel.display();


}

//--------------------------------------------------------------
void ofApp::initCanvasGrid() {

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
void ofApp::initParticleMans() {
	for (auto& c : cells) {
		BlockManager p(500);
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
void ofApp::initGui() {
	gui.setup("P R I M A R Y");
	gui.add(background.set("background", ofColor(104, 166, 218, 80), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));
	gui.add(enableDebug.set("draw debug", true));

	blockParticleGui.setup("B L O C K  P");
	blockParticleGui.add(Block::pcolor.set("color", ofColor(0, 0, 0, 100), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));
	blockParticleGui.add(Block::tcolor.set("trail color", ofColor(0, 0, 0, 100), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));

	blockParticleGui.add(Block::r.set("radius", 5, 0, 50));
	blockParticleGui.add(Block::mass_base.set("mass base", 11.0, 0.0, 100.0));
	blockParticleGui.add(Block::speed_limit.set("speed limit", 20.0, 0.0, 50.0));

	blockParticleGui.add(Block::seek_on.set("seek on", true));
	blockParticleGui.add(Block::seek_limit.set("seek limit", 50.0, 0.0, 50.0));
	blockParticleGui.add(Block::arrive_on.set("arrive on",true));
	blockParticleGui.add(Block::arrive_cap.set("arrive cap", 100, 0, 250));

	blockParticleGui.add(Block::trail.set("trail on", true));
	blockParticleGui.add(Block::trail_wgt.set("trail weight", 5.0, 0.0, 10.0));
	blockParticleGui.add(Block::history_length.set("history length", 3, 0, 50));

	repelGui.setup("R E P E L");
	repelGui.add(repel.force_ctrl);
}

//--------------------------------------------------------------
void ofApp::drawGui(ofEventArgs& args) {
	gui.draw();
	blockParticleGui.draw();
	repelGui.draw();
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
	case 'a':
		for (auto& p : pman) {
			p.gridSpawn(20, 5);
		}
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
	case 'h':
		//for (auto& p : pman) {
		//	//p.applySeek();
		//	//@TODO: add arrive to manager.. being lazy rn
		//	for (auto& _p : p.p) {
		//		//toggle for arrive?
		//		_p.seek(_p.home_location);
		//		if (_p.arrive_on)_p.arrive(_p.home_location);
		//	}
		//}
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
	//simple force
	//im going to ignore the pman for now.. lazy ass coding
	//@TODO: add a toggle for different forces
	//@TODO: add force vars to gui

	//update force
	repel.update(ofVec2f(x, y));

	for (auto& p : pman) {
		for (auto& _p : p.p) {
			//repel for now
			if (repel.inRange(_p)) {
				_p.applyforce(repel.force(_p));
			}

		}
	}
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
