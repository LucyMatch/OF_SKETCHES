#pragma once
#include "ParticleManager.h"
#include "ImageParticle.h"


//@TODO: 
// do we need to destory these? we have no kill function..

class ImageParticleManager : public ParticleManager {

public:
	ImageParticleManager( vector<ofTexture> &imgs );

	virtual void update();
	virtual void draw();

	//start pos / 0 = x random y = 0 / 1 = x 0 y random / 2 x rand y max / 3 x max y rand
	virtual void simpleSpawn(int start_pos);
	virtual void applyVaryingGravity(float min, float max, int direction);

	void gridSpawn(int colResolution, int rowResolution);
	void imgSpawn();

	void drawDebug();

	int getImgIndex();
	void nextImgIndex();
	void prevImgIndex();


	vector<ImageParticle> p;
	vector<ofTexture> images;
	int curr_asset = 0;			//used for uniform asset across p's
	

	static ofParameter<int> limit, b_mode_selector;
	static ofParameter<bool> enable_limit, enable_kill , enable_bounce , randomize_assets, enable_kill_on_leave_screen;
	static ofParameter<ofColor> debug_c;

private:

};