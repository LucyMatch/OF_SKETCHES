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
