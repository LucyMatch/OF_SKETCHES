#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetVerticalSync(false);
	ofEnableSmoothing();
	ofEnableAlphaBlending();

	//init vid manager
	video.setDims(glm::vec2(1280, 720));
	video.setup(); //also here we would define - ip / webcam / local

	//init cut manager
	cut_man.setup();

	//init gui
	initGui();

}

//--------------------------------------------------------------
void ofApp::update() {

	framerate();

	//update video for new frame
	video.update();

	//live update cut manager w/ mouse position
	cut_man.update(mouseX, mouseY);

	//update cuts & ps
	for (auto& p : p_man) {
		if (video.cam.isFrameNew() && p.p.size() > 0) {
			ofTexture tmp_tex;
			tmp_tex = cut_man.getCutTexture( p.getCut() , *(video.getFrameTex()));
			p.update(tmp_tex);
		}	
		if (enable_auto_spawn)p.spawn();
		if (p.p.size() > 0) {
			if (enable_varying_gravity)p.applyVaryingGravity(v_gravity_min, v_gravity_max, v_gravity_direction);
			p.update();
		}
	}

	

}

//--------------------------------------------------------------
void ofApp::draw() {

	ofSetColor(bg_c);
	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

	//draw "original" texture 
	if (enable_orig)video.draw();

	//@TODO: sometimes memory error
	//sometimes this is out of bounds??
	//or w / h of p is fucked sometimes - so the draw throws an errr
	for (auto& p : p_man)
		p.draw();

	if(enable_debug)drawDebug();

}

//--------------------------------------------------------------
void ofApp::drawDebug() {

	cut_man.draw();

	for (auto& p : p_man)
		p.drawDebug();

}

//--------------------------------------------------------------
void ofApp::framerate() {
	std::stringstream strm;
	strm << "fps: " << ofGetFrameRate();
	ofSetWindowTitle(strm.str());
}

//--------------------------------------------------------------
void ofApp::initGui() {

	gui.setup("P R I M A R Y");

	gui.add(bg_c.set("background", ofColor(255, 228, 246, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));
	gui.add(enable_debug.set("enable debug", false));
	gui.add(enable_orig.set("enable orig", true));
	//gui.add(b_mode_selector.set("blend modes", 1, 0, blends.size() - 1));
	//gui.add(curr_c.set("cut img colour", ofColor(255, 255, 255, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));
	gui.add(enable_auto_spawn.set("enable auto spawning", false));
	gui.add(enable_varying_gravity.set("enable varying gravity", false));
	gui.add(v_gravity_direction.set("gravity direction", 0, 0, 3));
	gui.add(v_gravity_min.set("gravity min", 0.5, 0.5, 25));
	gui.add(v_gravity_max.set("gravity max", 10, 0.5, 25));


	gui.add(video.gui);

	//you may need to think more about this flow
	//you likely want something closer to how FORCES class works
	gui.add(cut_man.gui);

	//@TODO: add pman gui
	//make those vals static we have a few pmen now
	//gui.add()

}

//--------------------------------------------------------------
void ofApp::drawGui(ofEventArgs& args) {
	gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

	switch (key) {
	case 's':
		cut_man.exportCuts(video.getFrameTex());
		break;
	case '.':
		video.nxtFeed();
		break;
	case ',':
		video.prevFeed();
		break;
	case 'x':
		//clear saved cuts
		cut_man.clearCuts();
		p_man.clear();
		break;
	case 'p':
		//spawn 
		for (auto& p : p_man)
			p.spawn();
		break;
	case 'o':
		//random spawn 
		for (auto& p : p_man)
			p.randomSpawn();
		break;
	case '-':
		//clear p's only 
		for (auto& p : p_man)
			p.clear();
		break;
	case '1':
		gui.saveToFile("1_gui.xml");
		break;
	case '2':
		gui.saveToFile("2_gui.xml");
		break;
	case '3':
		gui.saveToFile("3_gui.xml");
		break;
	case '4':
		gui.saveToFile("4_gui.xml");
		break;
	case 'q':
		gui.loadFromFile("1_gui.xml");
		break;
	case 'w':
		gui.loadFromFile("2_gui.xml");
		break;
	case 'e':
		gui.loadFromFile("3_gui.xml");
		break;
	case 'r':
		gui.loadFromFile("4_gui.xml");
		break;
	default:
		break;
	}

	if (key == OF_KEY_RETURN) {
		//screen shot
		glReadBuffer(GL_FRONT);
		ofSaveScreen("screenshot_" + ofToString(ofGetMonth()) + "_" + ofToString(ofGetDay()) + "_" + ofToString(ofGetYear()) + "_" + ofToString(ofGetHours()) + "_" + ofToString(ofGetMinutes()) + ofToString(ofGetSeconds()) + ".png");
	}

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

	//@TODO: determine if this how we wanna do this...
	//_cpm holds a pointer to the cut
	CutParticleManager _cpm( *(cut_man.saveCut()) );
	p_man.push_back( _cpm );

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

