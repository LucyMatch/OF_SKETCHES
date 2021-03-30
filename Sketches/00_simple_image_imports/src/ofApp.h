#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxAnimatable.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void assetsLoad(string path);
		void assetsDraw();
		void assetsDrawRandom(); //todo
		void assetsAnimate();  //todo

		void guiInit();
		void guiDraw(ofEventArgs& args);

		void debugDraw();

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

		vector<ofTexture> images;
		ofFbo image_layer;

		//GUI PRIMARY
		ofxPanel gui;
		ofParameter<ofColor> bg;
		ofParameter<float> bg_width, bg_height;
		ofParameter<int> bg_ref_rate;
		ofParameter<bool> enable_debug;

		ofxPanel image_gui;
		ofParameter<float> img_alpha;

		
};
