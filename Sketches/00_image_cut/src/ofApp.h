#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ImageHandler.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void drawDebug();
		void saveImg();

		void initGui();
		void drawGui(ofEventArgs& args);
		void framerate();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		ImageHandler *input_imgs;
		ofTexture curr_img;
		int curr_index = 0;		///hook this up to a next prev

		//save_fbo for drawing to before save img
		//mask_fbo for drawing the shape we want to cut out
		ofFbo save_fbo, mask_fbo;

		ofPath simple_shape;

		ofxPanel gui;
		ofParameter<ofColor> bg_c;
		ofParameter<bool> enable_debug, enable_save_debug;
		
};
