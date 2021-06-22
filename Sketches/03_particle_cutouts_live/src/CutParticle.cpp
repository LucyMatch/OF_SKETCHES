#include "CutParticle.h"

//behavior
//--------------------------------------------------------------
//ofParameter<float> CutParticle::seek_limit = 50.0;
ofParameter<float> CutParticle::speed_limit = 2.0;
ofParameter<float> CutParticle::mass_base = 11.0;
//ofParameter<bool> CutParticle::arrive_on = true;
//ofParameter<bool> CutParticle::seek_on = true;
ofParameter<bool> CutParticle::trail = false;
//ofParameter<int> CutParticle::arrive_cap = 100;
//ofParameter<int> CutParticle::b_mode_selector = 1;

//asethetic
//--------------------------------------------------------------
ofParameter<ofColor> CutParticle::pcolor = ofColor(0, 0, 0, 100);
ofParameter<ofColor> CutParticle::tcolor = ofColor(0, 0, 0, 100);
ofParameter<float> CutParticle::trail_wgt = 5.0;
ofParameter<int> CutParticle::history_length = 3;
ofParameter<int> CutParticle::r = 10;
ofParameter<int> CutParticle::size_min = 10;
ofParameter<int> CutParticle::size_max = 10;
ofParameter<bool> CutParticle::enable_true_size = true;
ofParameter<bool> CutParticle::enable_random_size = true;
ofParameter<bool> CutParticle::enable_continous_resizing = false;
ofParameter<bool> CutParticle::enable_home_in_history = false;


//--------------------------------------------------------------
CutParticle::CutParticle(ofTexture* _img) {

	img = *_img;

	orig_w = img.getWidth();
	orig_h = img.getHeight();
	size();

	dead = false;
	velocity.set(0, 0);
	acceleration.set(0.0, 0.00);
	location = ofVec2f(0, 0);
	home_location = location;

	//limit the magnitude of a vector
	velocity.limit(speed_limit);
};

//--------------------------------------------------------------
CutParticle::CutParticle(ofTexture* _img, glm::vec2  _location) {

	img = *_img;

	orig_w = img.getWidth();
	orig_h = img.getHeight();
	size();

	dead = false;
	velocity.set(0, 0);
	acceleration.set(0.0, 0.00);
	location = _location;
	home_location = location;

	//limit the magnitude of a vector
	velocity.limit(speed_limit);
}

//--------------------------------------------------------------
void CutParticle::draw() {

	if (trail) {
		for (int i = 1; i < history.size(); i++) 
			if (img.isAllocated())img.draw(history[i].x - (w / 2), history[i].y - (h / 2), w, h);
	}

	if(img.isAllocated())img.draw((location.x - (w / 2)), (location.y - h / 2), w, h);
}

//--------------------------------------------------------------
void CutParticle::update() {

	if (enable_continous_resizing)size();

	//limit the magnitude of a vector
	velocity.limit(speed_limit);

	velocity = velocity + acceleration;
	location = location + velocity;

	//reset acceleration each frame
	acceleration = acceleration * 0;

	//for trail / history
	if (trail) {
		history.push_back(location);
		int diff = history.size() - history_length;
		if (history.size() > history_length) {
			if (enable_home_in_history) {
				//for trails that originate from home pt
				for (int i = 1; i < diff; i++) {
					history.erase(history.begin() + i);
				}
			}
			else {
				for (int i = 0; i < diff; i++) {
					history.erase(history.begin() + i);
				}
			}
		}
	}
}

//--------------------------------------------------------------
void CutParticle::updateFrame(ofTexture* _img) {
	img = *_img;
}

//--------------------------------------------------------------
void CutParticle::drawDebug() {

	if (trail) {
		ofPushStyle();
		ofSetColor(tcolor);
		ofSetLineWidth(trail_wgt);
		for (int i = 1; i < history.size(); i++) {
			ofDrawLine(history[i - 1].x, history[i - 1].y, history[i].x, history[i].y);
		}
		ofPopStyle();
	}

	ofPushStyle();
	ofSetColor(pcolor);
	ofFill();
	ofDrawRectangle((location.x - (r / 2)), (location.y - r / 2), r, r);
	ofPopStyle();

}

//--------------------------------------------------------------
void CutParticle::size() {

	if (enable_true_size) {
		//actual size
		w = orig_w, h = orig_h;
		mass = mass_base / h;
		if (w > h) mass = mass_base / w;
	}
	else {

		auto size = r;
		if (enable_random_size)
			size = ofRandom(size_min, size_max);

		//here we keep in ratio 
		//and use r as a modifier.. 
		w = size;
		h = (orig_h / orig_w) * w;
		mass = mass_base / size;
	}
}

//--------------------------------------------------------------
/// Particle seeking provided pt
//--------------------------------------------------------------
//void CutParticle::seek(ofVec2f& target) {
//	if (seek_on) {
//		ofVec2f desiredV;
//		desiredV = target - location;
//
//		desiredV.normalize();
//		desiredV *= speed_limit;
//
//		ofVec2f steer = desiredV - velocity;
//		steer.limit(seek_limit);
//		applyforce(steer);
//	}
//}

//@TODO: not fully convinced this is working.....
/// Particle behavior if within arrive cap of seek target
/// the below alters the speedlimit ( mapped to base limit ) the closer the particle gets to seek pt
/// //--------------------------------------------------------------
//void CutParticle::arrive(ofVec2f& target) {
//	if (arrive_on) {
//		ofVec2f desiredV = target - location;
//
//		float d = desiredV.length();
//
//		if (d < arrive_cap) {
//			float m = ofMap(d, 0, arrive_cap, 0, speed_limit);
//			desiredV *= m;
//		}
//		else {
//			desiredV *= speed_limit;
//		}
//
//		ofVec2f steer = desiredV - velocity;
//		steer.limit(seek_limit);
//		applyforce(steer);
//	}
//}

