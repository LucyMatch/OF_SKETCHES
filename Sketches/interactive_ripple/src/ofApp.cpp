#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofSetFrameRate(60);
	//ofEnableAlphaBlending();
	ofSetVerticalSync(true);

	//allocate & clear fbos
	render.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA32F);
	render.src->begin(); ofClear(0, 0, 0, 0); render.src->end();
	render.dst->begin(); ofClear(0, 0, 0, 0); render.dst->end();
	texture.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA32F);
	texture.begin(); ofClear(0, 0, 0, 0); texture.end();
	drawFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA32F);
	drawFbo.begin(); ofClear(0, 0, 0, 0); drawFbo.end();

	//load shader
	shader.load("ripple");
	//shader.load("ripple2");

	damping = 0.9;

	//may want to try with a loaded texture here too
	//before porting back into video effects
	testBG.loadImage("test.jpg");
}

//--------------------------------------------------------------
void ofApp::update(){
	interactiveVideo();
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(0);
	ofSetColor(255, 255);
	drawInteractive();
}

//--------------------------------------------------------------
void ofApp::interactiveVideo() {

	////draw source / blobs
	render.src->begin();
		ofPushStyle();
		ofPushMatrix();
			//blobs here	
			ofFill();
			ofSetColor(ofNoise(ofGetFrameNum()) * 255 * 5, 255);
			ofDrawCircle(mouseX, mouseY, 15);
		ofPopMatrix();
		ofPopStyle();
	render.src->end();

	////draw shader to fbo
	texture.begin();
	shader.begin();
		shader.setUniformTexture("backbuffer", render.dst->getTexture(), 0);
		shader.setUniformTexture("tex0", render.src->getTexture(), 1);
		shader.setUniform1f("damping", (float)damping );
		shader.setUniform2f("canvasDims", glm::vec2(ofGetWidth(), ofGetHeight()));
		//
		renderFrame();
		//
	shader.end();
	texture.end();

	//draw to ping pong destination
	render.dst->begin();
		texture.draw(0, 0, ofGetWidth(), ofGetHeight());
	render.dst->end();

	render.swap();

	/////

	//render.dst->begin();

	//ofClear(0, 0);
	//ofDisableAlphaBlending();

	//shader.begin();
	//shader.setUniformTexture("backbuffer", render.src->getTexture(), 0);
	//shader.setUniformTexture("tex0", render.src->getTexture(), 1);
	//shader.setUniformTexture("tex1", testBG.getTexture(), 2);

	//renderFrame();

	//shader.end();
	////texture.end();
	//render.dst->end();

	////render.dst->begin();
	////texture.draw(0,0);
	////render.dst->end();

	//render.swap();

}

//--------------------------------------------------------------
void ofApp::drawInteractive() {
	//drawFbo.draw(0, 0, ofGetWidth(), ofGetHeight());
	//render.dst->draw(0, 0, ofGetWidth(), ofGetHeight());

	ofPushStyle();
	ofEnableAlphaBlending();
	render.dst->draw(0, 0, ofGetWidth(), ofGetHeight());
	ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::renderFrame() {
	ofSetColor(255, 255);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
	glTexCoord2f(ofGetWidth(), 0); glVertex3f(ofGetWidth(), 0, 0);
	glTexCoord2f(ofGetWidth(), ofGetHeight()); glVertex3f(ofGetWidth(), ofGetHeight(), 0);
	glTexCoord2f(0, ofGetHeight());  glVertex3f(0, ofGetHeight(), 0);
	glEnd();
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
