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

private:

	vector<ofTexture> images;

};
