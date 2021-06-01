#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofSetFrameRate(60);
	ofSetVerticalSync(false);
	ofEnableSmoothing();
	ofEnableAlphaBlending();

	ofSetCircleResolution(1000);

	//set the mask fbo settings
	//mask_fbo.allocate( ofGetWidth(), ofGetHeight(), GL_LUMINANCE );	//this didn't give trans bg
	mask_fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
	mask_fbo.getTexture().setSwizzle(GL_TEXTURE_SWIZZLE_A, GL_RED);

	
	//init gui
	initGui();

	//import images 
	//input_imgs = new ImageHandler("input_images");

	//set img objects
	//setImg();

}

//--------------------------------------------------------------
void ofApp::update(){
	framerate();

	//hard coded for now
	simple_shape.clear();
	simple_shape.ellipse(mouseX, mouseY, ss_w, ss_h);
	simple_shape.close();

	ofPushStyle();
	mask_fbo.begin();

		//draw to mask fbo
		ofSetColor(0,0,0,255);
		ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

		ofSetColor(255, 255, 255, 255);

		simple_shape.draw();

		if (enable_multi_cut_view)
			for (auto& c : cuts)
				c.draw();

	mask_fbo.end();
	ofPopStyle();

	//curr_img.setAlphaMask(mask_fbo.getTexture());

}

//--------------------------------------------------------------
void ofApp::draw(){

	//ofPushStyle();
		ofSetColor(bg_c);
		ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
	//ofPopStyle();

	//draw "uncut" img 
	if (enable_orig) {
		//ofPushStyle();
			//@TODO: is this where we want the blend?
			//add a blend
			ofEnableBlendMode(blends[b_mode_selector]);
			ofSetColor(orig_c);
			//orig_img.draw(0, 0, orig_img.getWidth(), orig_img.getHeight());
		//ofPopStyle();
	}
	
	//draw the cut img...
	ofEnableAlphaBlending();
	ofSetColor(curr_c);
	//curr_img.draw(0,0, curr_img.getWidth(), curr_img.getHeight());


	if (enable_debug)drawDebug();
}

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
	//gui.add(enable_save_debug.set("enable save fbo debug", false));
	gui.add(enable_multi_cut_view.set("enable multi cut display", false));
	gui.add(ss_w.set("simple cut width", 100, 1, 1000));
	gui.add(ss_h.set("simple cut height", 100, 1, 1000));
	//gui.add(save_pad.set("crop img save padding", 4, -20, 20));
	gui.add(enable_orig.set("enable orig", true));
	//gui.add(b_mode_selector.set("blend modes", 1, 0, blends.size() - 1));
	//gui.add(orig_c.set("orig img colour", ofColor(255, 228, 246, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));
	//gui.add(curr_c.set("cut img colour", ofColor(255, 255, 255, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));

	gui.add(video->gui);

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
	//case '.':
	//	curr_index++;
	//	curr_index = curr_index % input_imgs->getImages().size();
	//	setImg();
	//	break;
	//case ',':
	//	curr_index--;
	//	curr_index = curr_index % input_imgs->getImages().size();
	//	setImg();
	//	break;
	case 'x':
		//clear saved cuts
		cuts.clear();
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

