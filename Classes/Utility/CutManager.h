/*CutManager v1*/
#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Cuts.h"

class CutManager {

	public :
		CutManager();

		void setup();
		void update( int x, int y);
		void draw();

		void drawDebug();
		void initGui();

		BaseCut* saveCut();
		void clearCuts();
		void undoCut();
		void exportCuts( ofTexture *tex );
		//void exportCurrCut();

		ofTexture getCutTexture( BaseCut c, ofTexture tex );
		size_t getCutsSize();

		ofFbo save_fbo;

		ofParameterGroup gui;
		ofParameter<bool> enable_multi_cut_view;
		ofParameter<float> save_pad;

	private :

		BaseCut curr_cut;
		vector<BaseCut> cuts;

};