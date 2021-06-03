#include "CutManager.h"

CutManager::CutManager() {}

void CutManager::setup(){
	initGui();
}

void CutManager::update( int x, int y){
	curr_cut.update( x, y );
}

void CutManager::draw(){

	//draw current
	curr_cut.draw();

	//draw all stored
	if(enable_multi_cut_view)
		for (auto& c : cuts)
			c.draw();
}

void CutManager::drawDebug(){
	curr_cut.draw();
}

void CutManager::initGui(){
	gui.setName("cut manager");
	gui.add(enable_multi_cut_view.set("enable multi cut display", false));
	gui.add(curr_cut.gui);
}

void CutManager::saveCut(){
	cuts.push_back(curr_cut);
}

void CutManager::clearCuts(){
	cuts.clear();
}


//void CutManager::undoCut(){}
//void CutManager::exportCuts(){}
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