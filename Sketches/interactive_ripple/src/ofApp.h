#pragma once


//based off https://github.com/patriciogonzalezvivo/ofxFX/blob/master/src/operations/ofxRipples.h 

#include "ofMain.h"
#include "pingPongBuffer.h"

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

		void interactiveVideo();
		void drawInteractive();
		void renderFrame();

		ofShader shader;
		pingPongBuffer render;
		ofFbo texture;
		ofImage testBG;

		//adding this in as when integratesd with vid effects this will be the filter fbo
		ofFbo drawFbo;

		float damping;

		
};
