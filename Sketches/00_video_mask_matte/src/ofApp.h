#pragma once

#include "ofMain.h"
#include "ColourPalette.h"
#include "Videohandler.h"


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void drawDebug();
		void drawPalette();

		void initGui();
		void drawGui(ofEventArgs& args);
		void drawInfo();
		void framerate();

		void keyPressed(int key);
		void mousePressed(int x, int y, int button);
		void windowResized(int w, int h);

		ColourPalette palettes;
		int bg_c_index = 0;

		VideoHandler video;
		VideoHandler matte;

		ofFbo composite;
		//ofTexture masked_tex;

		ofxPanel gui;
		ofParameter<ofColor> bg_c;
		ofParameter<bool> 
			enable_palette_preview = false,
			enable_debug = false,
			enable_info = false;
		ofParameter<int> curr_palette = 0;

};
