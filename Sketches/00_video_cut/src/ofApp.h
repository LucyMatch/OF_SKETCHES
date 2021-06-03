#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "VideoHandler.h"
#include "CutManager.h"

class ofApp : public ofBaseApp{

	public:

		void setup();
		void update();
		void draw();


		//void setImg();	//assigns curr_img + orig
		//void saveImg();	//saves crop //@todo : save all crops 1 by one / save curr crop options
		//@todo; move placement of cut out of save img - so we can have a save cut to vector of cuts option

		//"cuts" functions
		//eventually to be its own class
		//void saveCut();
		//void undoCut();
		//void clearCuts();
		//void setCutMode(int mode);
		//void saveAllCutsToDisk();
		//void saveCutToDisk(int index);

		void initGui();
		void drawGui(ofEventArgs& args);
		void framerate();
		void drawDebug();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);

		VideoHandler video;
		ofTexture live_cut_tex;
		//vector<ofTexture> cut_tex;

		CutManager cut_man;

		//ImageHandler *input_imgs;
		//ofTexture curr_img, orig_img;
		//int curr_index = 0;

		//save_fbo for drawing to before save img
		//mask_fbo for drawing the shape we want to cut out
		ofFbo mask_fbo; //save_fbo

		//"cuts" vars
		//ofPath simple_shape;
		//vector<ofPath> cuts;

		std::map<int, ofBlendMode> blends{
			{0, OF_BLENDMODE_DISABLED },
			{1, OF_BLENDMODE_ALPHA },
			{2, OF_BLENDMODE_ADD },
			{3, OF_BLENDMODE_SUBTRACT },
			{4, OF_BLENDMODE_MULTIPLY },
			{5, OF_BLENDMODE_SCREEN },
		};

		//@TODO: clean this
		ofxPanel gui;
		ofParameter<ofColor> bg_c, orig_c, curr_c;
		ofParameter<bool> enable_debug, enable_save_debug, enable_orig, enable_multi_cut_view;
		ofParameter<float> ss_w, ss_h, save_pad;
		ofParameter<int> b_mode_selector;
		
};
