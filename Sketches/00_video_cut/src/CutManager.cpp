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
void CutManager::exportCuts( ofTexture *tex, bool individual ) {

	//duplicate cuts - so we aren't using raw cuts
	//raw cuts container can be updated - this would cause buffer issues
	vector<BaseCut> _cuts = cuts;

	//our texture
	ofTexture temp_tex = *tex;
	
	//fbo for drawing masks
	//@TODO: may wanna size this to the texture
	ofFbo temp_fbo;
	temp_fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
	temp_fbo.getTexture().setSwizzle(GL_TEXTURE_SWIZZLE_A, GL_RED);

	//found shape bounding boxes
	ofRectangle bounding;

	//if not doing individual draw to the mask now
	if (!individual) {
		ofPushStyle();
			temp_fbo.begin();
				ofSetColor(0, 0, 0, 255);
				ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
				for (auto& c : _cuts)
					c.draw();
			temp_fbo.end();
		ofPopStyle();
		temp_tex.setAlphaMask(temp_fbo.getTexture());
	}

	for( auto i = 0; i < _cuts.size(); ++i){

		////draw to the mask individually
		if (individual) {
			ofPushStyle();
				temp_fbo.begin();
					ofSetColor(0, 0, 0, 255);
					ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
					_cuts[i].draw();
				temp_fbo.end();
			ofPopStyle();
			temp_tex.setAlphaMask(temp_fbo.getTexture());
		}


		////get resize info
		if (_cuts[i].shape.getOutline().size() > 0)
			bounding = _cuts[i].shape.getOutline()[0].getBoundingBox();

		
		////this may need work - with the pad / positioning.... 
		glm::vec2 pos = bounding.getPosition();
		glm::vec2 size( bounding.getWidth() + save_pad, bounding.getHeight() + save_pad );

		////draw subsection of texture to save fbo
		save_fbo.clear();
		save_fbo.allocate( size.x, size.y, GL_RGBA );
		save_fbo.begin();
			temp_tex.drawSubsection( 0, 0, size.x, size.y, pos.x, pos.y );
		save_fbo.end();

		////export
		ofPixels tmp_pix;
		save_fbo.getTexture().readToPixels( tmp_pix );
		ofImage new_img(tmp_pix);
		new_img.allocate(int(size.x), int(size.y), OF_IMAGE_COLOR_ALPHA);
		new_img.save("outputs/cuts_" + ofToString(i) + "_" + ofToString(ofGetMonth()) + "_" + ofToString(ofGetDay()) + "_" + ofToString(ofGetYear()) + "_" + ofToString(ofGetHours()) + "_" + ofToString(ofGetMinutes()) + ofToString(ofGetSeconds()) + ".png", OF_IMAGE_QUALITY_BEST);

	}

}
