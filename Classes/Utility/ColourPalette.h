/*
	v 1 - Colour Palette
	Lucy Matchett - 2022

	Loads colour palettes via xml
	for easy retrieval maybe eventually some colour math etc

*/
#pragma once

#include "ofMain.h"

class ColourPalette {

public:
	ColourPalette();

	void load(string path = "colours.xml");
	void draw();

	ofColor* getColour( int palette_index, int colour_index);
	vector<ofColor>* getPalette( int palette_index);
	vector<ofColor> getPaletteEdited( int palette_index, int excluded_index);
	vector<vector<ofColor>>* getPalettes();

	int getNPalettes();
	int getNColours( int palette_index );


private:
	vector<vector<ofColor>> palettes;
	ofXml xml;
};