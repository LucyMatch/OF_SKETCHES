#pragma once

/*CutManager v2*/
/*
this manager / relationshiop to cuts is very different from v1
v1 has an "active cut" / one controlled / static cut
this version has a series of cuts that are constantly active
these cuts are kept track of
- so i'm not going to keep it as a polymorph...
*/

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxFaceTracker2.h"
#include "PolyCuts.h"

/*
struct for storing our cuts - with relation to landmarks
*/
struct LandmarkCut {
	ofxFaceTracker2Landmarks::Feature feature;
	PolyCuts cut;
	bool enabled;
};

/*
	enum Feature {
	LEFT_EYE_TOP, RIGHT_EYE_TOP,
	LEFT_EYEBROW, RIGHT_EYEBROW,
	LEFT_EYE, RIGHT_EYE,
	LEFT_JAW, RIGHT_JAW, JAW,
	OUTER_MOUTH, INNER_MOUTH,
	NOSE_BRIDGE, NOSE_BASE,
	FACE_OUTLINE, ALL_FEATURES
};
*/

class LandmarkCutManager {

public:
	LandmarkCutManager() {

		PolyCuts 
			re("left eye"), 
			le("right eye"), 
			m("mouth");

		struct LandmarkCut 
			lefteye { ofxFaceTracker2Landmarks::Feature::LEFT_EYE, re, true },
			righteye{ ofxFaceTracker2Landmarks::Feature::RIGHT_EYE, le, true },
			mouth{ ofxFaceTracker2Landmarks::Feature::OUTER_MOUTH, m, true };

		cuts.push_back(lefteye);
		cuts.push_back(righteye);
		cuts.push_back(mouth);

		initGui();

	}
	void update(ofxFaceTracker2Landmarks lm) {
		for (auto &c : cuts) {
			if (c.feature == ofxFaceTracker2Landmarks::Feature::LEFT_EYE && enable_left_eye != c.enabled)c.enabled = enable_left_eye;
			if (c.feature == ofxFaceTracker2Landmarks::Feature::RIGHT_EYE &&  enable_right_eye != c.enabled)c.enabled = enable_right_eye;
			if (c.feature == ofxFaceTracker2Landmarks::Feature::OUTER_MOUTH && enable_mouth != c.enabled)c.enabled = enable_mouth;
			c.cut.update(lm.getImageFeature(c.feature));
		}
	}
	void draw() {
		for (auto& c : cuts)
			if(c.enabled)c.cut.draw();
	}	
	void draw(ofTexture* tex) {
		for (auto& c : cuts)
			if (c.enabled) {
				c.cut.set();
				getCutTexture(c.cut, *tex).draw(c.cut.getPos());
			}
	}

	void drawDebug() {
		for (auto& c : cuts)
			c.cut.draw();
	}

	void exportCuts( ofTexture *tex ){
		for (auto& c : cuts) {
			glm::vec2 size = c.cut.getSize();
			////export
			ofPixels tmp_pix;
			getCutTexture(c.cut, *tex).readToPixels(tmp_pix);
			ofImage new_img(tmp_pix);
			new_img.allocate(int(size.x), int(size.y), OF_IMAGE_COLOR_ALPHA);
			new_img.save("outputs/cuts_" + ofToString(c.feature) + "_" + ofToString(ofGetMonth()) + "_" + ofToString(ofGetDay()) + "_" + ofToString(ofGetYear()) + "_" + ofToString(ofGetHours()) + "_" + ofToString(ofGetMinutes()) + ofToString(ofGetSeconds()) + ".png", OF_IMAGE_QUALITY_BEST);
		}
	}

	ofTexture getCutTexture( PolyCuts c, ofTexture tex ){
		//set our cut
		//this saves its sizes - before we retrieve them
		c.update();
		c.set();

		//our texture
		ofTexture temp_tex = tex;

		//fbo for drawing masks
		//@TODO: may wanna size this to the texture
		ofFbo temp_fbo, save_fbo;
		temp_fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
		temp_fbo.getTexture().setSwizzle(GL_TEXTURE_SWIZZLE_A, GL_RED);

		//draw so we can mask
		ofPushStyle();
		temp_fbo.begin();
			ofSetColor(0, 0, 0, 255);
			ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
			c.draw();
		temp_fbo.end();
		ofPopStyle();

		temp_tex.setAlphaMask(temp_fbo.getTexture());

		glm::vec2 pos = c.getPos();
		glm::vec2 size = c.getSize();

		//cout << pos.x << " " << pos.y << endl;
		//cout << size.x << " " << size.y << endl;

		save_fbo.allocate(size.x, size.y, GL_RGBA);
		save_fbo.begin();
		temp_tex.drawSubsection(0, 0, size.x, size.y, pos.x, pos.y);
		save_fbo.end();

		return save_fbo.getTexture();
	}

	void initGui() {
		gui.clear();
		gui.setName("CUT MANAGER");
		gui.add(enable_left_eye.set("enable left eye", true));
		gui.add(enable_right_eye.set("enable right eye", true));
		gui.add(enable_mouth.set("enable mouth", true));
		gui.add(cuts[0].cut.gui);
		gui.add(cuts[1].cut.gui);
		gui.add(cuts[2].cut.gui);
	}

	ofParameterGroup gui;
	ofParameter<bool> enable_left_eye, enable_right_eye, enable_mouth;

	vector<LandmarkCut> cuts;

private:

	ofFbo save_fbo;
	

};
