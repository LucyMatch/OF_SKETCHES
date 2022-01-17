/*
	v 1 - Image Handler
	Lucy Matchett - 2021

	Just loads a directory of images

	@TODO:
	* Add Other Image utilities - saving frames
	* threaded Image loader
	* maybe restructure in general this is really a directory handler

*/
#pragma once

#include "ofMain.h"

class ImageHandler {

public:

	ImageHandler( string path ) {
		ofDirectory dir;
		dir.listDir(path);
		dir.allowExt("png");
		dir.sort();

		int amt = (int)dir.size();

		images.assign(amt, ofTexture());

		for (int i = 0; i < amt; i++) {
			ofLoadImage(images[i], dir.getPath(i));
			std::cout << "[ IMG MAN ] LOADED FILE: " << dir.getName(i) << std::endl;
		}
	}

	vector<ofTexture>& getImages() {return images;}

	//--------------------------------------------------------------
	bool isDeactivated() {
		return deactivate;
	}

	bool deactivate = false;

private:

	vector<ofTexture> images;

};
