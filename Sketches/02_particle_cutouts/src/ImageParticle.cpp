#include "ImageParticle.h"

//behavior
ofParameter<float> ImageParticle::seek_limit = 50.0;
ofParameter<float> ImageParticle::speed_limit = 2.0;
ofParameter<float> ImageParticle::mass_base = 11.0;
ofParameter<bool> ImageParticle::arrive_on = true;
ofParameter<bool> ImageParticle::seek_on = true;
ofParameter<bool> ImageParticle::trail = true;
ofParameter<int> ImageParticle::arrive_cap = 100;

//asethetic
ofParameter<ofColor> ImageParticle::pcolor = ofColor(0, 0, 0, 100);
ofParameter<ofColor> ImageParticle::tcolor = ofColor(0, 0, 0, 100);
ofParameter<float> ImageParticle::trail_wgt = 5.0;
ofParameter<int> ImageParticle::history_length = 3;
ofParameter<int> ImageParticle::r = 10;
ofParameter<bool> ImageParticle::enable_uniform_size = false;
ofParameter<bool> ImageParticle::enable_true_size = false;
ofParameter<bool> ImageParticle::enable_home_in_history = false;

ImageParticle::ImageParticle(ofTexture* _img) {

	img = _img;

	orig_w = img->getWidth();
	orig_h = img->getHeight();
	size();

	dead = false;
	velocity.set(0, 0);
	acceleration.set(0.0, 0.00);
	location = ofVec2f(0, 0);
	home_location = location;

	//limit the magnitude of a vector
	velocity.limit(speed_limit);
};

ImageParticle::ImageParticle(ofVec2f  _location, ofTexture* _img) {

	img = _img;

	orig_w = img->getWidth();
	orig_h = img->getHeight();
	size();

	dead = false;
	velocity.set(0, 0);
	acceleration.set(0.0, 0.00);
	location = _location;
	home_location = location;

	//limit the magnitude of a vector
	velocity.limit(speed_limit);
}

//@TODO :
//			draw to fbo
//			alpha and colour effects
//			blend effects
void ImageParticle::draw() {
		if (trail) {
			ofPushStyle();
			for (int i = 1; i < history.size(); i++) {
				img->draw(history[i].x - (w / 2), history[i].y - (h / 2), w, h);
			}
			ofPopStyle();
		}

		ofPushStyle();
		img->draw((location.x - (w / 2)), (location.y - h / 2), w, h);
		ofPopStyle();
}

void ImageParticle::update() {

	//update radius + mass incase it's been updated
	//we wilkl want to check that this doesnt eat up fps
	//we may want to come up with a flag to see if it needs updating....
	size();

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

void ImageParticle::drawDebug() {

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

void ImageParticle::size() {
	if (enable_uniform_size) {
		mass = mass_base / r;
		w = r, h = r;
	}else if(enable_true_size) {
		//actual size
		w = orig_w, h = orig_h;
		mass = mass_base / h;
		if(w > h) mass = mass_base / w;
	}else {
		//here we keep in ratio 
		//and use r as a modifier.. 
		w = r;
		h = (orig_h / orig_w) * w;
		mass = mass_base / r;
	}
}

/// Particle seeking provided pt
void ImageParticle::seek(ofVec2f& target) {
	if (seek_on) {
		ofVec2f desiredV;
		desiredV = target - location;

		desiredV.normalize();
		desiredV *= speed_limit;

		ofVec2f steer = desiredV - velocity;
		steer.limit(seek_limit);
		applyforce(steer);
	}
}

//@TODO: not fully convinced this is working.....
/// Particle behavior if within arrive cap of seek target
/// the below alters the speedlimit ( mapped to base limit ) the closer the particle gets to seek pt
void ImageParticle::arrive(ofVec2f& target) {
	if (arrive_on) {
		ofVec2f desiredV = target - location;

		float d = desiredV.length();

		if (d < arrive_cap) {
			float m = ofMap(d, 0, arrive_cap, 0, speed_limit);
			desiredV *= m;
		}
		else {
			desiredV *= speed_limit;
		}

		ofVec2f steer = desiredV - velocity;
		steer.limit(seek_limit);
		applyforce(steer);
	}
}

