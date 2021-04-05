#include "ImageParticleManager.h"

ofParameter<int> ImageParticleManager::limit = 100;
//@TODO: fully integrate enable_limit / limit
ofParameter<bool> ImageParticleManager::enable_limit = true;
ofParameter<bool> ImageParticleManager::enable_kill = true;
ofParameter<bool> ImageParticleManager::enable_bounce = false;
ofParameter<bool> ImageParticleManager::randomize_assets = true;
ofParameter<ofColor> ImageParticleManager::debug_c = ofColor(0, 0, 0, 100);

//--------------------------------------------------------------
ImageParticleManager::ImageParticleManager() {
	draw_dims = { 0, 0, ofGetWidth(), ofGetHeight() };
};

//--------------------------------------------------------------
void ImageParticleManager::assetsLoad(string path) {
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

//@TODO: add more toggles here
//--------------------------------------------------------------
void ImageParticleManager::simpleSpawn() {
	if ((enable_limit && p.size() < limit) || !enable_limit) {
		ofVec2f _loc;
		_loc.set(ofRandom(draw_dims.x, (draw_dims.x + draw_dims.z)), ofRandom(draw_dims.y, (draw_dims.y + draw_dims.w)));
		//_loc.set(ofRandom(draw_dims.x, (draw_dims.x + draw_dims.z)), draw_dims.y);

		ImageParticle _p(_loc, &images[getImgIndex()]);

		ofVec2f gravity(0, ofRandom(3, 5));
		_p.applyforce(gravity);

		p.push_back(_p);
	}
}

//--------------------------------------------------------------
void ImageParticleManager::gridSpawn(int colResolution, int rowResolution) {

	//@TODO: apply enable_limit to this - decide how you want to reduce.. 
	//or based on particle limit

	p.clear();

	int w = draw_dims.z;
	int h = draw_dims.w;
	ofVec2f start(draw_dims.x, draw_dims.y);

	int cols = w / colResolution;
	int rows = h / rowResolution;

	for (int i = 0; i < cols; i++) {
		for (int j = 0; j < rows; j++) {
			ofVec3f _l(i * colResolution, j * rowResolution, 0);
			//apply margins
			_l.x += colResolution;
			_l.y += rowResolution;
			ofMatrix4x4 translation = ofMatrix4x4::newTranslationMatrix(start);
			ofPushMatrix();
			ofMultMatrix(translation);
			ofPushMatrix();
			_l = _l * translation;

			ImageParticle _p(_l, &images[getImgIndex()]);
			_p.setHome(ofVec2f(_l.x, _l.y));
			p.push_back(_p);
		}
	}
}

//currently will generate a particle for each unique image asset
//@TODO: 
//		may want to add more params / work on location placement
//--------------------------------------------------------------
void ImageParticleManager::imgSpawn() {

	for (auto& i : images) {
		ofVec2f _loc;
		_loc.set(ofRandom(draw_dims.x, (draw_dims.x + draw_dims.z)), ofRandom(draw_dims.y, (draw_dims.y + draw_dims.w)));

		ImageParticle _p(_loc, &i);

		p.push_back(_p);
	}

}

//--------------------------------------------------------------
void ImageParticleManager::draw() {
	for (auto& _p : p) {
		_p.draw();
	}
}

//--------------------------------------------------------------
void ImageParticleManager::drawDebug() {

	ofPushStyle();
	ofSetColor(debug_c);
	ofDrawRectangle(draw_dims[0], draw_dims[1], draw_dims[2], draw_dims[3]);
	ofPopStyle();

	for (auto& _p : p) {
		_p.drawDebug();
	}
}

//--------------------------------------------------------------
void ImageParticleManager::update() {

	for (int i = 0; i < p.size(); i++) {

		p[i].update();

		if (enable_bounce) {
			//bounce
			p[i].checkEdges(draw_dims, false);
		}
		else {
			//mark as dead
			p[i].checkEdges(draw_dims, true);
			//erase dead
			if(p[i].dead && enable_kill)p.erase(p.begin() + i);
		}

	}

};

//--------------------------------------------------------------
void ImageParticleManager::applySeek() {
	for (int i = 0; i < p.size(); i++) {
		p[i].seek(p[i].home_location);
	}
};

//--------------------------------------------------------------
void ImageParticleManager::applySeek(ofVec2f loc) {
	for (int i = 0; i < p.size(); i++) {
		p[i].seek(loc);
	}
};

//--------------------------------------------------------------
int ImageParticleManager::getImgIndex() {
	int img_index = curr_asset;
	if (randomize_assets)  img_index = ofRandom(0, images.size() - 1);
	return img_index;
}

//--------------------------------------------------------------
void ImageParticleManager::nextImgIndex() {
	curr_asset++;
	curr_asset = curr_asset % images.size();
	std::cout << "CURR IMG UPDATED TO : " << curr_asset << std::endl;
}

//--------------------------------------------------------------
void ImageParticleManager::prevImgIndex() {
	curr_asset--;
	curr_asset = curr_asset % images.size();
	std::cout << "CURR IMG UPDATED TO : " << curr_asset << std::endl;
}