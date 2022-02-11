#pragma once

#include "ofMain.h"
#include "ColourPalette.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

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

		ColourPalette pals;

		bool enable_debug = false;		//draw debug
		bool enable_current = false;	//draw current palette
		int curr_palette = 0;

		vector<ofPath> shapes;
		ofPath curr_shape;
		int curr_shape_colour = 0;
		
};
