#include "CutManager.h"

CutManager::CutManager() {}

void CutManager::setup(){
	initGui();
}

void CutManager::update( int x, int y){
	curr_cut.update( x, y );
}

void CutManager::draw(){

	//draw current
	curr_cut.draw();

	//draw all stored
	if(enable_multi_cut_view)
		for (auto& c : cuts)
			c.draw();
}

void CutManager::drawDebug(){
}

void CutManager::initGui(){
	gui.setName("cut manager");
	gui.add(enable_multi_cut_view.set("enable multi cut display", false));
	gui.add(curr_cut.gui);
}

void CutManager::saveCut(){
	cuts.push_back(curr_cut);
}

void CutManager::clearCuts(){
	cuts.clear();
}


//void CutManager::undoCut(){}
//void CutManager::exportCuts(){}