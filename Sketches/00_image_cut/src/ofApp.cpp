#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofSetFrameRate(60);
	ofSetVerticalSync(false);
	ofEnableSmoothing();
	ofEnableAlphaBlending();

	ofSetCircleResolution(1000);

	//set the mask fbo settings
	//mask_fbo.allocate( ofGetWidth(), ofGetHeight(), GL_LUMINANCE );
	mask_fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
	mask_fbo.getTexture().setSwizzle(GL_TEXTURE_SWIZZLE_A, GL_RED);

	

	initGui();

	//import images
	//@TODO: make dymaic w/ curr img - so we can cycle through loaded imgs
	input_imgs = new ImageHandler("input_images");
	if (input_imgs->getImages().size() > 0)
		curr_img = input_imgs->getImages()[0];

}

//--------------------------------------------------------------
void ofApp::update(){
	framerate();

	//hard coded for now
	simple_shape.ellipse( mouseX - 50, mouseY - 50, 100, 100);

	ofPushStyle();
	mask_fbo.begin();
		//draw to mask fbo
		ofSetColor(0,0,0,255);
		ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
		ofSetColor(255, 255, 255, 255);
		simple_shape.draw();
	mask_fbo.end();
	ofPopStyle();

	curr_img.setAlphaMask(mask_fbo.getTexture());

}

//--------------------------------------------------------------
void ofApp::draw(){

	ofPushStyle();
		ofSetColor(bg_c);
		ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
	ofPopStyle();

	//glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	curr_img.draw(0,0, curr_img.getWidth(), curr_img.getHeight());
	if (enable_debug)drawDebug();
}

//--------------------------------------------------------------
void ofApp::saveImg() {

	//resizing
	//@TODO: this is ahwole suite of functions - mayeb a class or img handler
	//1. where is the shape drawn?
	//2. what is the size of the shape / bounding rect
	//3. draw a section of curr_img to save fbo
	//4. texture from save fbo turned into an img for saving

	//this would be gathered in smarter way
	//LHC not centered like ellipse..
	glm::vec2 pos( mouseX - 102, mouseY - 102 );
	glm::vec2 size( 104, 104 );

	//draw subsection to save_fbo
	save_fbo.clear();
	save_fbo.allocate( size.x, size.y, GL_RGBA );
	save_fbo.begin();
		curr_img.drawSubsection( 0, 0, size.x, size.y, pos.x, pos.y );
	save_fbo.end();

	//get the pixels from the fbo texture to assign to a new img for saving
	//is there a better way?
	
	ofPixels tmp_pix;

	save_fbo.getTexture().readToPixels( tmp_pix );

	ofImage new_img(tmp_pix);
	new_img.allocate(int(size.x), int(size.y), OF_IMAGE_COLOR_ALPHA);

	new_img.save("outputs/screenshot_" + ofToString(ofGetMonth()) + "_" + ofToString(ofGetDay()) + "_" + ofToString(ofGetYear()) + "_" + ofToString(ofGetHours()) + "_" + ofToString(ofGetMinutes()) + ofToString(ofGetSeconds()) + ".png", OF_IMAGE_QUALITY_BEST);

}

//--------------------------------------------------------------
void ofApp::drawDebug() {
	mask_fbo.draw(0,0,ofGetWidth(),ofGetHeight());
	simple_shape.draw();
	if(enable_save_debug)save_fbo.draw(0,0, save_fbo.getWidth(), save_fbo.getHeight());
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
	gui.add(enable_save_debug.set("enable save fbo debug", false));
}

//--------------------------------------------------------------
void ofApp::drawGui(ofEventArgs& args) {
	gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

	switch (key) {
	case 's':
		saveImg();
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
