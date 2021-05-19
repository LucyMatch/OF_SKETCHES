#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ImageHandler.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void setImg();	//assigns curr_img + orig
		void saveImg();	//saves crop

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

		ImageHandler *input_imgs;
		ofTexture curr_img, orig_img;
		int curr_index = 0;

		//save_fbo for drawing to before save img
		//mask_fbo for drawing the shape we want to cut out
		ofFbo save_fbo, mask_fbo;

		ofPath simple_shape;
		vector<ofPath> cuts;

		std::map<int, ofBlendMode> blends{
			{0, OF_BLENDMODE_DISABLED },
			{1, OF_BLENDMODE_ALPHA },
			{2, OF_BLENDMODE_ADD },
			{3, OF_BLENDMODE_SUBTRACT },
			{4, OF_BLENDMODE_MULTIPLY },
			{5, OF_BLENDMODE_SCREEN },
		};

		ofxPanel gui;
		ofParameter<ofColor> bg_c, orig_c, curr_c;
		ofParameter<bool> enable_debug, enable_save_debug, enable_orig, enable_multi_cut_view;
		ofParameter<float> ss_w, ss_h, save_pad;
		ofParameter<int> b_mode_selector;
		
};
