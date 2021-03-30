#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "BlockManager.h"
#include "Forces.h"
#include "ImageManager.h"


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

		ofFbo main_draw, main_capture;

		ImageManager rings_man;

		vector<BlockManager> pman;
		Repel repel;
		Attract attract;

		float canvas_margin, cell_margin;
		int rows, cols;
		bool spawning_active;
		ofRectangle canvas;
		vector<ofRectangle> cells;


		//GUI PRIMARY
		ofxPanel gui;
		ofParameter<ofColor> background, debug_bg;
		ofParameter<bool> enable_debug, enable_attract, enable_repel;
		ofParameter<int> gridSpawnW, gridSpawnH;

		ofxPanel baseParticleGui;
		ofxPanel blockParticleGui;
		ofxPanel repelGui;
		ofxPanel attractGui;
		
};
