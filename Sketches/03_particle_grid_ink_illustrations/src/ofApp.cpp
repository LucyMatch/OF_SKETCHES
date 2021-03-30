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

	main_draw.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);

	//@TODO: move to GUI
	//& have a listener that reinits the grid
	//&reinits the block managers
	//or just a buttont oggle that reinits it - and if vals changed then updates... 
	canvas_margin = 0.05;
	cell_margin = 0.12;
	rows = 5;
	cols = 5;

	//not really using this anymore --- maybe remove?
	spawning_active = false;

	initCanvasGrid();
	initParticleMans();
	rings_man.init();
	rings_man.assetsLoad("images/fixed_rings");
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

	//@TODO: add toggle
	rings_man.draw();
}

//--------------------------------------------------------------
void ofApp::draw(){

	main_draw.begin();

	//ofClear(0, 0, 0, 0);

		ofPushStyle();
			ofSetColor(background);
			ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
		ofPopStyle();

		ofPushStyle();
			if(enable_debug)drawDebug();
		ofPopStyle();

	//ofPushStyle();
	//@TODO: add to gui
	//ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofEnableBlendMode( OF_BLENDMODE_SCREEN );
//    ofEnableBlendMode(OF_BLENDMODE_SUBTRACT);
//    ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);

	//@TODO: 
	//update to draw particles on their own fbo - for blending properties....
		for (auto& p : pman) { p.draw(); }

		rings_man.getImageLayer().draw(0, 0, ofGetWidth(), ofGetHeight());

	//ofPopStyle();

	main_draw.end();

	main_draw.draw(0, 0, ofGetWidth(), ofGetHeight());
	
}

//--------------------------------------------------------------
void ofApp::drawDebug() {

	for (auto& c : cells) {
		ofPushStyle();
		ofSetColor(debug_bg);
		ofDrawRectangle(c.x, c.y, c.width, c.height);
		ofPopStyle();
	}

	repel.display();

	attract.display();

	//for (auto& p : pman) { p.drawDebug(); }

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
	gui.add(debug_bg.set("debug background", ofColor(171, 196, 237), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));

	gui.add(enable_debug.set("draw debug", true));
	gui.add(gridSpawnW.set("particle grid w", 4, 1, 100));
	gui.add(gridSpawnH.set("particle grid h", 10, 1, 100));
	gui.add(rings_man.image_ctrl);

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
	repelGui.add(enable_repel.set("repel enable", true));
	repelGui.add(repel.force_ctrl);

	attractGui.setup("A T T R A C T");
	attractGui.add(enable_attract.set("attract enable", false));
	attractGui.add(attract.force_ctrl);
}

//--------------------------------------------------------------
void ofApp::drawGui(ofEventArgs& args) {
	gui.draw();
	blockParticleGui.draw();
	repelGui.draw();
	attractGui.draw();
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
			p.gridSpawn(gridSpawnW, gridSpawnH);
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
		break;
	case 'x':
		ofxSuperLog::getLogger()->setScreenLoggingEnabled(false);
		break;
	case 'z':
		ofxSuperLog::getLogger()->setScreenLoggingEnabled(true);
		break;
	case '1':
		gui.saveToFile("1_gui.xml");
		blockParticleGui.saveToFile("1_blckgui.xml");
		repelGui.saveToFile("1_rplgui.xml");
		break;
	case '2':
		gui.saveToFile("2_gui.xml");
		blockParticleGui.saveToFile("2_blckgui.xml");
		repelGui.saveToFile("2_rplgui.xml");
		break;
	case '3':
		gui.saveToFile("3_gui.xml");
		blockParticleGui.saveToFile("3_blckgui.xml");
		repelGui.saveToFile("3_rplgui.xml");
		break;
	case '4':
		gui.saveToFile("4_gui.xml");
		blockParticleGui.saveToFile("4_blckgui.xml");
		repelGui.saveToFile("4_rplgui.xml");
		break;
	case 'q':
		gui.loadFromFile("1_gui.xml");
		blockParticleGui.loadFromFile("1_blckgui.xml");
		repelGui.loadFromFile("1_rplgui.xml");
		break;
	case 'w':
		gui.loadFromFile("2_gui.xml");
		blockParticleGui.loadFromFile("2_blckgui.xml");
		repelGui.loadFromFile("2_rplgui.xml");
		break;
	case 'e':
		gui.loadFromFile("3_gui.xml");
		blockParticleGui.loadFromFile("3_blckgui.xml");
		repelGui.loadFromFile("3_rplgui.xml");
		break;
	case 'r':
		gui.loadFromFile("4_gui.xml");
		blockParticleGui.loadFromFile("4_blckgui.xml");
		repelGui.loadFromFile("4_rplgui.xml");
		break;
	case '7':
		gui.saveToFile("001_gui.xml");
		blockParticleGui.saveToFile("001_blckgui.xml");
		repelGui.saveToFile("001_rplgui.xml");
		break;
	case '8':
		gui.saveToFile("002_gui.xml");
		blockParticleGui.saveToFile("002_blckgui.xml");
		repelGui.saveToFile("002_rplgui.xml");
		break;
	case '9':
		gui.saveToFile("003_gui.xml");
		blockParticleGui.saveToFile("003_blckgui.xml");
		repelGui.saveToFile("003_rplgui.xml");
		break;
	case '0':
		gui.saveToFile("004_gui.xml");
		blockParticleGui.saveToFile("004_blckgui.xml");
		repelGui.saveToFile("004_rplgui.xml");
		break;
	case 'u':
		gui.loadFromFile("001_gui.xml");
		blockParticleGui.loadFromFile("001_blckgui.xml");
		repelGui.loadFromFile("001_rplgui.xml");
		break;
	case 'i':
		gui.loadFromFile("002_gui.xml");
		blockParticleGui.loadFromFile("002_blckgui.xml");
		repelGui.loadFromFile("002_rplgui.xml");
		break;
	case 'o':
		gui.loadFromFile("003_gui.xml");
		blockParticleGui.loadFromFile("003_blckgui.xml");
		repelGui.loadFromFile("003_rplgui.xml");
		break;
	case 'p':
		gui.loadFromFile("004_gui.xml");
		blockParticleGui.loadFromFile("004_blckgui.xml");
		repelGui.loadFromFile("004_rplgui.xml");
		break;
	default :
		break;
	}

	if (key == OF_KEY_RETURN) {

		//we need to figure out how to get a true screen grab - not frame 
		//as our bg is transparent we get differing colours 
		//just drawing a white bg doesn't help either.. 
		glReadBuffer(GL_FRONT);
		ofSaveScreen("screenshot_" + ofToString(ofGetMonth()) + "_" + ofToString(ofGetDay()) + "_" + ofToString(ofGetYear()) + "_" + ofToString(ofGetHours()) + "_" + ofToString(ofGetMinutes()) + ofToString(ofGetSeconds()) + ".png");
		
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	//simple force
//im going to ignore the pman for now.. lazy ass coding

	if (enable_repel) {
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

	if (enable_attract) {
		//update force
		attract.update(ofVec2f(x, y));

		for (auto& p : pman) {
			for (auto& _p : p.p) {
				//repel for now
				if (attract.inRange(_p)) {
					_p.applyforce(attract.force(_p));
				}

			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	//simple force
	//im going to ignore the pman for now.. lazy ass coding

	//if (enable_repel) {
	//	//update force
	//	repel.update(ofVec2f(x, y));

	//	for (auto& p : pman) {
	//		for (auto& _p : p.p) {
	//			//repel for now
	//			if (repel.inRange(_p)) {
	//				_p.applyforce(repel.force(_p));
	//			}

	//		}
	//	}
	//}

	//if (enable_attract) {
	//	//update force
	//	attract.update(ofVec2f(x, y));

	//	for (auto& p : pman) {
	//		for (auto& _p : p.p) {
	//			//repel for now
	//			if (attract.inRange(_p)) {
	//				_p.applyforce(attract.force(_p));
	//			}

	//		}
	//	}
	//}


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
