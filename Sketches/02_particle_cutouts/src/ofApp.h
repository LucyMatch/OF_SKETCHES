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

		std::map<int, ofBlendMode> blends{
			{0, OF_BLENDMODE_DISABLED },
			{1, OF_BLENDMODE_ALPHA },
			{2, OF_BLENDMODE_ADD },
			{3, OF_BLENDMODE_SUBTRACT },
			{4, OF_BLENDMODE_MULTIPLY },
			{5, OF_BLENDMODE_SCREEN },
		};

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
		ofParameter<int> b_mode_selector;

		ofxPanel repelGui;
		ofxPanel attractGui;
		ofxPanel particleGui;
		ofxPanel pmanGui;

};
