#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(60);
	ofSetVerticalSync(false);
	ofEnableAlphaBlending();

	guiInit();
	assetsLoad("images");

	image_layer.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
	
}

//--------------------------------------------------------------
void ofApp::update(){
	assetsDraw();
}

//--------------------------------------------------------------
void ofApp::draw(){
	if (ofGetFrameNum() % bg_ref_rate == 0) {
		ofPushStyle();
		ofSetColor(bg);
		ofDrawRectangle(0, 0, bg_width, bg_height);
		ofPopStyle();
	}
	image_layer.draw(0, 0, ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void ofApp::assetsDraw() {


	image_layer.begin();

	ofPushStyle();

	//	  ofEnableBlendMode(OF_BLENDMODE_ADD);
	//    ofEnableBlendMode( OF_BLENDMODE_SCREEN );
	//    ofEnableBlendMode(OF_BLENDMODE_SUBTRACT);
	//    ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);


	ofSetColor(255, img_alpha);
	for (auto& img : images) 
		img.draw(0, 0, ofGetWidth(), ofGetHeight());


	ofPopStyle();

	image_layer.end();


}

//--------------------------------------------------------------
void ofApp::assetsDrawRandom() {

}

//--------------------------------------------------------------
void ofApp::assetsLoad(string path) {

	ofDirectory dir;
	dir.listDir(path);
	dir.allowExt("png");
	dir.sort();

	int amt = (int)dir.size();

	images.assign(amt, ofTexture());


	for (int i = 0; i < amt; i++) {
		ofLoadImage(images[i], dir.getPath(i));
		std::cout << "[ LOADER MAN ] LOADED FILE: " << dir.getName(i) << std::endl;
	}
}

//--------------------------------------------------------------
void ofApp::debugDraw() {
	ofPushStyle();
	//ofSetColor();
	ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::guiInit() {
	gui.setup("P R I M A R Y");
	gui.add(enable_debug.set("debug on", false));
	gui.add(bg.set("background", ofColor(104, 166, 218, 80), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));
	gui.add(bg_width.set("background width", ofGetWidth(), 0, ofGetWidth()));
	gui.add(bg_height.set("background height", ofGetHeight(), 0, ofGetHeight()));
	gui.add(bg_ref_rate.set("background refresh rate", 1, 1, 60));


	image_gui.setup("I M A G E");
	image_gui.add(img_alpha.set("image alpha", 255, 0, 255));
}

//--------------------------------------------------------------
void ofApp::guiDraw(ofEventArgs& args) {
	gui.draw();
	image_gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    switch (key) {
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
