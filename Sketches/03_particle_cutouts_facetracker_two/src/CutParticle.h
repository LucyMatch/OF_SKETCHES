/*
lucy matchett - 08/19/2021
Cut Particle
differs from 03_Particle_cut_outs_live/CutParticle.h / .cpp 
that wasn't ready to be an external class... 
so this is likely to diverge from that class

@TODO:
things I would like : 

1 -- make the gui control non static.... 
	but rather controlled by the particle manager
	so for example eye particles would have a different effect... 
	this ould require using the event from manager gui to update children on event fired... 

2 -- add home / seeking back in! 
	with home location set as partman's spawn location i.e. curr feature location.. 
*/

#pragma once
#include "Particle.h"

class CutParticle : public Particle {

public:

	CutParticle(ofTexture* _img);
	CutParticle(ofTexture* _img, glm::vec2  _location);
	virtual void draw();
	virtual void update();
	//virtual void seek(ofVec2f& target);
	//virtual void arrive(ofVec2f& target);

	void updateFrame(ofTexture* _img);
	void drawDebug();
	void size();

	//@TODO: test & see if you can just redclare these as non static
	//		do this once you have everything working baseline.. 
	static ofParameter<float> speed_limit, mass_base, trail_wgt, seek_limit;
	static ofParameter<ofColor> pcolor, tcolor;
	static ofParameter<int> arrive_cap, history_length;
	static ofParameter<bool> trail, arrive_on, seek_on;
	static ofParameter<int> r, size_min, size_max, b_mode_selector;
	static ofParameter<bool> enable_true_size, enable_home_in_history, enable_random_size, enable_continous_resizing;

	ofTexture img;
	float w, h;
	float orig_w, orig_h;

private:

};
