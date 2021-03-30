#pragma once
#include "ParticleManager.h"
#include "ImageParticle.h"


//@TODO: 
// do we need to destory these? we have no kill function..

class ImageParticleManager : public ParticleManager {

public:

	ImageParticleManager() {
		draw_dims = { 0, 0, ofGetWidth(), ofGetHeight() };
	};

	ImageParticleManager(int _limit) {
		enable_limit = true;
		limit = _limit;
	}

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
	virtual void simpleSpawn() {
		if (enable_limit && p.size() < limit) {
			ofVec2f _loc;
			_loc.set(ofRandom(draw_dims.x, (draw_dims.x + draw_dims.z)), ofRandom(draw_dims.y, (draw_dims.y + draw_dims.w)));
			//_loc.set(ofRandom(draw_dims.x, (draw_dims.x + draw_dims.z)), draw_dims.y);

			//randomly pick from images vector

			ImageParticle _p(_loc, &images[ofRandom(0, images.size() -1)] );

			ofVec2f gravity(0, ofRandom(3, 5));
			_p.applyforce(gravity);

			p.push_back(_p);
		}
	}

	void gridSpawn(int colResolution, int rowResolution) {

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

				ImageParticle _p(_l, &images[ofRandom(0, images.size() - 1)]);
				_p.setHome(ofVec2f(_l.x, _l.y));
				p.push_back(_p);
			}
		}
	}

	//--------------------------------------------------------------
	virtual void draw() {
		for (auto& _p : p) {
			_p.draw();
		}
	}

	//--------------------------------------------------------------
	void drawDebug() {
		for (auto& _p : p) {
			_p.drawDebug();
		}
	}

	//--------------------------------------------------------------
	virtual void update() {

		for (int i = 0; i < p.size(); i++) {

			p[i].update();

			p[i].checkEdges(draw_dims, true);

			//@TODO: add toggle for this
			///erase dead
			if (p[i].dead) {
				p.erase(p.begin() + i);
			}
		}
	};

	//--------------------------------------------------------------
	virtual void applySeek() {
		for (int i = 0; i < p.size(); i++) {
			p[i].seek(p[i].home_location);
		}
	};

	//--------------------------------------------------------------
	virtual void applySeek(ofVec2f loc) {
		for (int i = 0; i < p.size(); i++) {
			p[i].seek(loc);
		}
	};

	vector<ofTexture> images;
	vector<ImageParticle> p;
	bool enable_limit = false;
	int limit = 100;


private:

};