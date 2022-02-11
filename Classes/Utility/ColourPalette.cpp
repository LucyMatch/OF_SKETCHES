#include "ColourPalette.h"

//--------------------------------------------------------------
ColourPalette::ColourPalette() {
}

//--------------------------------------------------------------
void ColourPalette::load(string path){
	if (!xml.load(path)) {
		ofLogError() << "Couldn't load file";
	}
	else {
		auto new_palettes = xml.find("//palettes/palette");	//grab palettes
	
		for (auto& p : new_palettes) {
	
			auto colours = p.getChildren("c");	//grab the palettes colours 
			vector<ofColor> _p;					//create temp palettte container

			for (auto& c : colours) {

				//xml coilour format is string w/ ' ' space delimiter
				auto _c = ofSplitString( c.getAttribute("rgb").getValue(), " " );
				if(_c.size() >= 3)
					_p.push_back( ofColor( ofToInt(_c[0]), ofToInt(_c[1]), ofToInt(_c[2])) );
			}

			//finished filling temp palette - add to palettes
			palettes.push_back(_p);
		}
	}
}

//--------------------------------------------------------------
void ColourPalette::draw(){
	ofPushStyle();
	int w,  h = ofGetHeight() / getNPalettes();
	for (int rows = 0; rows < getNPalettes(); rows++) {
		w = ofGetWidth() / palettes[rows].size();
		for (int cell = 0; cell < palettes[rows].size(); cell++) {
			ofPushMatrix();
			ofTranslate( w * cell , h * rows );
			ofSetColor(palettes[rows][cell]);
			ofDrawRectangle( 0, 0, w, h );
			ofPopMatrix();
		}
	}
	ofPopStyle();
}

//--------------------------------------------------------------
ofColor* ColourPalette::getColour(int palette_index, int colour_index){
	if(getNPalettes() > palette_index && getNColours(palette_index) > colour_index)
		return &palettes[palette_index][colour_index];
}

//--------------------------------------------------------------
vector<ofColor>* ColourPalette::getPalette(int palette_index){
	if (getNPalettes() > palette_index)
		return &palettes[palette_index];
}

//--------------------------------------------------------------
vector<vector<ofColor>>* ColourPalette::getPalettes(){
	return &palettes;
}

//--------------------------------------------------------------
int ColourPalette::getNPalettes(){
	return std::size(palettes);
}

//--------------------------------------------------------------
int ColourPalette::getNColours(int palette_index) {
	return std::size(palettes[palette_index]);
}