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

	//@TODO: for cuts class
	//hard coded for now
	//simple_shape.clear();
	//simple_shape.ellipse(mouseX, mouseY, ss_w, ss_h);
	//simple_shape.circle(mouseX, mouseY, ss_w);
	//simple_shape.close();

	//live update cut manager w/ mouse position
	cut_man.update(mouseX, mouseY);

	ofPushStyle();
	mask_fbo.begin();

		//draw to mask fbo
		ofSetColor(0,0,0,255);
		ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

		cut_man.draw();

		//ofSetColor(255, 255, 255, 255);
		//simple_shape.draw();

		//if (enable_multi_cut_view)
		//	for (auto& c : cuts)
		//		c.draw();

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
		ofSetColor(orig_c);
		video.getFrameTex()->draw(0, 0, video.getDims().x, video.getDims().y);
	}
	
	//draw the cut img...
	ofEnableAlphaBlending();
	ofSetColor(curr_c);
	live_cut_tex.draw( 0, 0, video.getDims().x, video.getDims().y);


	if (enable_debug)drawDebug();

}

//@TODO: for "cuts" class
//--------------------------------------------------------------
//void ofApp::saveCut() {
//	cuts.push_back(simple_shape);
//}

//--------------------------------------------------------------
//void ofApp::setImg() {
//	//if curr_index is valid - set curr img 
//	if (curr_index < input_imgs->getImages().size())
//		curr_img = input_imgs->getImages()[curr_index];
//	//save new img into orig img for debug / display
//	orig_img = curr_img;
//}

//--------------------------------------------------------------
//void ofApp::saveImg() {
//
//	//add cut to "cuts"
//	//this way we can store various cuts for viewing
//	cuts.push_back(simple_shape);
//
//	//resizing
//	//@TODO: this is ahwole suite of functions - mayeb a class or img handler
//	//1. where is the shape drawn?
//	//2. what is the size of the shape / bounding rect
//	//3. draw a section of curr_img to save fbo
//	//4. texture from save fbo turned into an img for saving
//
//	//this would be gathered in smarter way
//	//LHC not centered like ellipse..
//	glm::vec2 pos( mouseX - (ss_w/2 + ( save_pad/2 )), mouseY - (ss_h/2 +( save_pad/2 )) );
//	glm::vec2 size( ss_w + save_pad, ss_h + save_pad );
//
//	//draw subsection to save_fbo
//	save_fbo.clear();
//	save_fbo.allocate( size.x, size.y, GL_RGBA );
//	save_fbo.begin();
//		curr_img.drawSubsection( 0, 0, size.x, size.y, pos.x, pos.y );
//	save_fbo.end();
//
//	//get the pixels from the fbo texture to assign to a new img for saving
//	//is there a better way?
//	
//	ofPixels tmp_pix;
//
//	save_fbo.getTexture().readToPixels( tmp_pix );
//
//	ofImage new_img(tmp_pix);
//	new_img.allocate(int(size.x), int(size.y), OF_IMAGE_COLOR_ALPHA);
//
//	new_img.save("outputs/screenshot_" + ofToString(ofGetMonth()) + "_" + ofToString(ofGetDay()) + "_" + ofToString(ofGetYear()) + "_" + ofToString(ofGetHours()) + "_" + ofToString(ofGetMinutes()) + ofToString(ofGetSeconds()) + ".png", OF_IMAGE_QUALITY_BEST);
//
//}

//--------------------------------------------------------------
void ofApp::drawDebug() {

	ofPushStyle();
		video.draw();
	ofPopStyle();

	mask_fbo.draw(0,0,ofGetWidth(),ofGetHeight());

	//simple_shape.draw();

	//if(enable_save_debug)save_fbo.draw(0,0, save_fbo.getWidth(), save_fbo.getHeight());

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
	//gui.add(enable_save_debug.set("enable save fbo debug", false));
	//gui.add(enable_multi_cut_view.set("enable multi cut display", false));
	//gui.add(ss_w.set("simple cut width", 100, 1, 1000));
	//gui.add(ss_h.set("simple cut height", 100, 1, 1000));
	//gui.add(save_pad.set("crop img save padding", 4, -20, 20));
	gui.add(enable_orig.set("enable orig", true));
	//gui.add(b_mode_selector.set("blend modes", 1, 0, blends.size() - 1));
	//gui.add(orig_c.set("orig img colour", ofColor(255, 228, 246, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));
	//gui.add(curr_c.set("cut img colour", ofColor(255, 255, 255, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));

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

