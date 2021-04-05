#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Forces.h"
#include "ImageParticleManager.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void drawDebug();

		void initCanvasGrid();
		void initParticleMans();

		void gridSpawn();
		void spawn();

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

		ofFbo main_draw;

		ofRectangle canvas;
		vector<ofRectangle> cells;

		vector<ImageParticleManager> pman;

		Repel repel;
		Attract attract;

		ofxPanel gui;
		ofParameter<ofColor> background;
		ofParameter<bool> enable_debug, enable_attract, enable_repel, enable_simple_spawn;
		ofParameter<float> canvas_margin, cell_margin;
		ofParameter<int> rows, cols;
		ofParameter<int> grid_spawn_w, grid_spawn_h;

		ofxPanel repelGui;
		ofxPanel attractGui;
		ofxPanel particleGui;
		ofxPanel pmanGui;

};
