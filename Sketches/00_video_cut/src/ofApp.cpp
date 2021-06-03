#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofSetFrameRate(60);
	ofSetVerticalSync(false);
	ofEnableSmoothing();
	ofEnableAlphaBlending();

	//set the mask fbo settings
	mask_fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
	mask_fbo.getTexture().setSwizzle(GL_TEXTURE_SWIZZLE_A, GL_RED);

	//init vid manager
	video.setDims(glm::vec2(1280,720));
	video.setup(); //also here we would define - ip / webcam / local

	//init cut manager
	cut_man.setup();

	//init gui
	initGui();

}

//--------------------------------------------------------------
void ofApp::update(){
	framerate();

	//update video for new frame
	video.update();

	//assign new frame to texture for cutting display
	live_cut_tex = *(video.getFrameTex());

	//live update cut manager w/ mouse position
	cut_man.update(mouseX, mouseY);

	ofPushStyle();
	mask_fbo.begin();

		//draw to mask fbo
		ofSetColor(0,0,0,255);
		ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

		cut_man.draw();

	mask_fbo.end();
	ofPopStyle();

	live_cut_tex.setAlphaMask(mask_fbo.getTexture());

}

//--------------------------------------------------------------
void ofApp::draw(){

	ofSetColor(bg_c);
	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

	//draw "uncut" texture 
	if (enable_orig) {
		//@TODO: figure out blending...
		//ofEnableBlendMode(blends[b_mode_selector]);
		video.draw();
	}
	
	//draw the cut img...
	ofSetColor(curr_c);
	live_cut_tex.draw( 0, 0, video.getDims().x, video.getDims().y);

	if (enable_debug)drawDebug();

}

//--------------------------------------------------------------
void ofApp::drawDebug() {

	ofPushStyle();
		video.draw();
	ofPopStyle();

	mask_fbo.draw(0,0,ofGetWidth(),ofGetHeight());

	cut_man.drawDebug();

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
	gui.add(curr_c.set("cut img colour", ofColor(255, 255, 255, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));

	gui.add(video.gui);

	//you may need to think more about this flow
	//you likely want something closer to how FORCES class works
	gui.add(cut_man.gui);

}

//--------------------------------------------------------------
void ofApp::drawGui(ofEventArgs& args) {
	gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

	switch (key) {
	//case 's':
	//	saveImg();
	//	break;
	case '.':
		video.nxtFeed();
		break;
	case ',':
		video.prevFeed();
		break;
	case 'x':
		//clear saved cuts
		cut_man.clearCuts();
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
	cut_man.saveCut();
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

