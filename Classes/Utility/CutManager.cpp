#include "CutManager.h"

//--------------------------------------------------------------
CutManager::CutManager() {}

//--------------------------------------------------------------
void CutManager::setup(){
	initGui();
}

//--------------------------------------------------------------
void CutManager::update( int x, int y){
	curr_cut.update( x, y );
}

//--------------------------------------------------------------
void CutManager::draw(){

	//draw current
	curr_cut.draw();

	//draw all stored
	if(enable_multi_cut_view)
		for (auto& c : cuts)
			c.draw();
}

//--------------------------------------------------------------
void CutManager::drawDebug(){
	curr_cut.draw();
}

//--------------------------------------------------------------
void CutManager::initGui(){
	gui.setName("cut manager");
	gui.add(enable_multi_cut_view.set("enable multi cut display", false));
	gui.add(save_pad.set("save padding", 5, 0, 100));
	gui.add(curr_cut.gui);
}

//--------------------------------------------------------------
void CutManager::saveCut(){
	curr_cut.set();
	cuts.push_back(curr_cut);
}

//--------------------------------------------------------------
void CutManager::clearCuts(){
	cuts.clear();
}

//--------------------------------------------------------------
void CutManager::undoCut() {
	cuts.erase(cuts.begin() + cuts.size());
}

/* 
	basic export of every cut in cuts of current texture
	disable individual for all cuts to happen from "one texture"
		-like a piece of paper with all the cuts already in it
		-we capture chunk by chunk based on the cuts pos / size

	@TODO : test everything then split this up into functions
			maybe it's own class on a seperate thread actually?
*/
//--------------------------------------------------------------
void CutManager::exportCuts(ofTexture* tex) {

	//duplicate cuts - so we aren't using raw cuts
	//raw cuts container can be updated - this would cause buffer issues
	vector<BaseCut> _cuts = cuts;

	for (auto i = 0; i < _cuts.size(); ++i) {

		glm::vec2 size = _cuts[i].getSize();
		////export
		ofPixels tmp_pix;
		getCutTexture(_cuts[i], tex).readToPixels(tmp_pix);
		ofImage new_img(tmp_pix);
		new_img.allocate(int(size.x), int(size.y), OF_IMAGE_COLOR_ALPHA);
		new_img.save("outputs/cuts_" + ofToString(i) + "_" + ofToString(ofGetMonth()) + "_" + ofToString(ofGetDay()) + "_" + ofToString(ofGetYear()) + "_" + ofToString(ofGetHours()) + "_" + ofToString(ofGetMinutes()) + ofToString(ofGetSeconds()) + ".png", OF_IMAGE_QUALITY_BEST);

	}
}

ofTexture CutManager::getCutTexture(BaseCut c, ofTexture * tex) {
		//our texture
		ofTexture temp_tex = *tex;

		//fbo for drawing masks
		//@TODO: may wanna size this to the texture
		ofFbo temp_fbo, save_fbo;
		temp_fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
		temp_fbo.getTexture().setSwizzle(GL_TEXTURE_SWIZZLE_A, GL_RED);

		ofPushStyle();
			temp_fbo.begin();
				ofSetColor(0, 0, 0, 255);
				ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
				c.draw();
			temp_fbo.end();
		ofPopStyle();	

		temp_tex.setAlphaMask(temp_fbo.getTexture());

		glm::vec2 pos = c.getPos();
		glm::vec2 size = c.getSize();

		save_fbo.allocate( size.x, size.y, GL_RGBA );
		save_fbo.begin();
			temp_tex.drawSubsection( 0, 0, size.x, size.y, pos.x, pos.y );
		save_fbo.end();

		return save_fbo.getTexture();
}
