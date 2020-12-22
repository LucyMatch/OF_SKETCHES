#pragma once
#include "ParticleManager.h"
#include "Block.h"

class BlockManager : public ParticleManager {

public:

	BlockManager() {};

	BlockManager(int _limit) {
		enable_limit = true;
		limit = _limit;
	}

	//--------------------------------------------------------------
	virtual void simpleSpawn() {
		if (enable_limit && p.size() < limit) {
			ofVec2f _loc;
			//_loc.set(ofRandom(draw_dims.x, (draw_dims.x + draw_dims.z)), ofRandom(draw_dims.y, (draw_dims.y + draw_dims.w)));
			_loc.set(ofRandom(draw_dims.x, (draw_dims.x + draw_dims.z)), draw_dims.y);

			Block _p(_loc);

			ofVec2f gravity(0, ofRandom(0.5, 3));
			_p.applyforce(gravity);

			p.push_back(_p);
		}
	}

	//--------------------------------------------------------------
	virtual void draw() {
		for (auto &_p : p) {
			_p.draw();
		}
	}

	//--------------------------------------------------------------
	virtual void update() {

		for (int i = 0; i < p.size(); i++) {

			p[i].update();

			p[i].checkEdges(draw_dims, true);

			//erase dead
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

	vector<Block> p;
	bool enable_limit = true;
	int limit = 100;

private:

};