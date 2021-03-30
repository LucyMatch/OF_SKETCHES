#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "BlockManager.h"
#include "Forces.h"


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void drawDebug();

		void initCanvasGrid();
		void initParticleMans();
		void spawn();

		void logger();
		void framerate();
		void initGui();
		void drawGui(ofEventArgs& args);

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

		vector<BlockManager> pman;

		float canvas_margin, cell_margin;
		int rows, cols;
		bool spawning_active;
		ofRectangle canvas;
		vector<ofRectangle> cells;
		Repel repel;
		Attract attract;

		//GUI PRIMARY
		ofxPanel gui;
		ofParameter<ofColor> background;
		ofParameter<bool> enableDebug;

		ofxPanel baseParticleGui;
		ofxPanel blockParticleGui;
		ofxPanel repelGui;
		
};
