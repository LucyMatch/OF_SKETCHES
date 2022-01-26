/*
	v 1 - Image Handler
	Lucy Matchett - 2021

	Just loads a directory of images

	@TODO:
	* Add Other Image utilities - saving frames
	* threaded Image loader
	* restructure in general ( more like video handler )
	*	so u call a set up / or load directory to load a new directory ( keep oG constructor way too for legacy code )
	*	also then his handler would handle curr image / next image / 
	*	calls to just retrieve one image / or a set of images ratrher than ALL images
	* add proper deconstructor
*/
#pragma once

#include "ofMain.h"

class ImageHandler {

public:

	//--------------------------------------------------------------
	ImageHandler( ) {}

	//legacy way of using imageHandler....
	//loads all images in directory ready for use
	//--------------------------------------------------------------
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

	//--------------------------------------------------------------
	void setActive(bool status) {active = status;}

	//--------------------------------------------------------------
	vector<ofTexture>& getImages() {return images;}

	//--------------------------------------------------------------
	vector<ofTexture>* getImgCollectionPtr() { return &images; }

	//--------------------------------------------------------------
	bool isActive() { return active;}

private:

	bool active = true;
	vector<ofTexture> images;

};
