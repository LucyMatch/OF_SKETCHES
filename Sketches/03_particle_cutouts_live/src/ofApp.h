#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "VideoHandler.h"
#include "CutManager.h"
#include "CutParticleManager.h"
#include "Cuts.h"

class ofApp : public ofBaseApp {

public:

	void setup();
	void update();
	void draw();

	void initGui();
	void drawGui(ofEventArgs& args);
	void framerate();
	void drawDebug();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);

	VideoHandler video;
	//ofTexture live_cut_tex;

	CutManager cut_man;
	vector<CutParticleManager> p_man;

	//mask_fbo for drawing the shape we want to cut out
	//ofFbo mask_fbo;

	ofxPanel gui;
	ofParameter<ofColor> bg_c, curr_c;
	ofParameter<bool> enable_debug, enable_orig, enable_auto_spawn, enable_varying_gravity;
	ofParameter<int> v_gravity_direction;
	ofParameter<float> v_gravity_min, v_gravity_max;

	//@todo
	// 
	// 
	// 
	// 	   //ofParameter<int> b_mode_selector;

	//enable_spawn_on_cut // - do we even need this
	//enable_auto_spawning
	//
	//key press trigger for spawn one particle for each cut

	//std::map<int, ofBlendMode> blends{
	//	{0, OF_BLENDMODE_DISABLED },
	//	{1, OF_BLENDMODE_ALPHA },
	//	{2, OF_BLENDMODE_ADD },
	//	{3, OF_BLENDMODE_SUBTRACT },
	//	{4, OF_BLENDMODE_MULTIPLY },
	//	{5, OF_BLENDMODE_SCREEN },
	//};
};
