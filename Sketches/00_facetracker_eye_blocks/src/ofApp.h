#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxFaceTracker2.h"
#include "ColourPalette.h"
#include "VideoHandler.h"
#include "FaceCutManager.h"


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void initGui();	//called by main
		void drawGui(ofEventArgs& args);
		void framerate();
		void drawDebug();
		void drawPalette();
		void drawInfo();

		void keyPressed(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);

		VideoHandler video_input;
		ofxFaceTracker2 tracker;
		FaceCutManager cut_man;

		ofFbo foreground_canvas, main_canvas;

		ColourPalette palettes;
		int bg_c_index = 0;

		ofxPanel gui;
		ofParameter<ofColor> bg_c;
		ofParameter<float> foreground_alpha;
		ofParameter<bool> 
			enable_debug = false,
			enable_info = false,
			enable_original_video = false,
			enable_palette_preview = false;
		ofParameter<int> curr_palette;
		ofParameter<bool> enable_eye_grid, enable_sin_grid, enable_cell_grid, enable_pos_grid, enable_cut_mod_grid;
		ofParameter<float> eye_grid_res_w, eye_grid_res_h;
		ofParameter<float> canvas_scale;
		
};
