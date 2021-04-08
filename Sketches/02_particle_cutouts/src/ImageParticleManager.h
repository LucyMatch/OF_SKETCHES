#pragma once
#include "ParticleManager.h"
#include "ImageParticle.h"


//@TODO: 
// do we need to destory these? we have no kill function..

class ImageParticleManager : public ParticleManager {

public:

	ImageParticleManager();

	//@TODO: create image manager external class - this load them & shares reference
	//		that way they are not reloaded per pman
	//		and all image functionality can happen else where
	void assetsLoad(string path);

	virtual void update();
	virtual void draw();

	//start pos / 0 = x random y = 0 / 1 = x 0 y random / 2 x rand y max / 3 x max y rand
	virtual void simpleSpawn(int start_pos);
	virtual void applySeek();
	virtual void applySeek(ofVec2f loc);

	void gridSpawn(int colResolution, int rowResolution);
	void imgSpawn();

	void drawDebug();

	int getImgIndex();
	void nextImgIndex();
	void prevImgIndex();


	vector<ImageParticle> p;
	vector<ofTexture> images;
	int curr_asset = 0;			//used for uniform asset across p's
	

	static ofParameter<int> limit;
	static ofParameter<bool> enable_limit, enable_kill , enable_bounce , randomize_assets, enable_kill_on_leave_screen;
	static ofParameter<ofColor> debug_c;

private:

};