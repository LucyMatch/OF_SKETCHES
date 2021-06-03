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

		void saveCut();
		void clearCuts();
		//void undoCut();
		//void exportCuts();
		//void exportCurrCut();

		ofParameterGroup gui;
		ofParameter<bool> enable_multi_cut_view;

	private :

		BaseCut curr_cut;
		vector<BaseCut> cuts;

};