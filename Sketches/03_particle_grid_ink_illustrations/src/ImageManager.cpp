#include  "ImageManager.h"

ImageManager::ImageManager() {
	init();
}

void ImageManager::assetsLoad(string path) {
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

void ImageManager::draw() {
	image_layer.begin();

	ofPushStyle();

		  //ofEnableBlendMode(OF_BLENDMODE_ADD);
	//    ofEnableBlendMode( OF_BLENDMODE_SCREEN );
	//    ofEnableBlendMode(OF_BLENDMODE_SUBTRACT);
	//    ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);


	ofSetColor(255, img_alpha);
	for (auto& img : images)
		img.draw(0, 0, ofGetWidth(), ofGetHeight());


	ofPopStyle();

	image_layer.end();
}

void ImageManager::assetsDrawRandom() {
	//
}

void ImageManager::assetsAnimate() {
	//
}

void ImageManager::init() {
	image_layer.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
	image_ctrl.add(img_alpha.set("image alpha", 255, 0, 255));
}

ofFbo& ImageManager::getImageLayer() {
	return image_layer;
}