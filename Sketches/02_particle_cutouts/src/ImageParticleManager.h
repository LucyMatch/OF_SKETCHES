#pragma once
#include "ParticleManager.h"
#include "ImageParticle.h"


//@TODO: 
// do we need to destory these? we have no kill function..

class ImageParticleManager : public ParticleManager {

public:

	ImageParticleManager();

	void assetsLoad(string path);

	virtual void update();
	virtual void draw();

	virtual void simpleSpawn();

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
	static ofParameter<bool> enable_limit, enable_kill , enable_bounce , randomize_assets;
	static ofParameter<ofColor> debug_c;

private:

};