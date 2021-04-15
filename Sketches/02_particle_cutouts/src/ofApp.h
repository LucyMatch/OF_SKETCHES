#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Forces.h"
#include "ImageParticleManager.h"
#include "ImageHandler.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void drawDebug();

		void initImages();
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

		//vector<ImageHandler> imgman;
		ImageHandler *rings, *rocks, *eyes, *mouthes, *misc;

		vector<ImageParticleManager> pman;

		Repel repel;
		Attract attract;

		ofxPanel gui;
		ofParameter<ofColor> background;
		ofParameter<bool> enable_debug, enable_attract, enable_repel, enable_simple_spawn, enable_varying_gravity;
		ofParameter<float> canvas_margin, cell_margin;
		ofParameter<int> rows, cols;
		ofParameter<int> grid_spawn_w, grid_spawn_h;
		ofParameter<int> pman_direction, v_gravity_direction;
		ofParameter<float> v_gravity_min, v_gravity_max, pman_bg_alpha;

		ofxPanel repelGui;
		ofxPanel attractGui;
		ofxPanel particleGui;
		ofxPanel pmanGui;

};
