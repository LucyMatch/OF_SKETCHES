#pragma once

/*CutManager v2*/
/*
*	this keeps track of each individual landmark / for all active instances	
*	we generate a cut per landmark ( left / right / mouth ) per face
* 
	@TODO:
	x - add in more support for other landmarks ( curr only tested with eyes / mouth )
	x - expand into other "polyccuts" i.e. different shapes based of poly points
	x - expand into non - texture cut poly's - just a triangle where the nose is etc...
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
	string label;
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
		initGui();
	}

	//sending ALL instances... 
	void update( vector<ofxFaceTracker2Instance> t ) {
		if (t.size() > 0) { 
			for (int i = 0; i < t.size(); i++) {
				//now we check to see if there are elements for this face yet
				if (i < faces.size()) {
					//exists
					update(t[i].getLandmarks(), i);
				}
				else {
					//create new
					initNewFace();
					update( t[i].getLandmarks(), i );
				}
			}
		}
		checkFaces();
	}

	void update(ofxFaceTracker2Landmarks lm, int index) {
		if (faces.size() > index) {
			for (auto& c : faces[index]) {
				/*
				*	current solve for how many instances of polycuts
				*	this approaches features as singletons not faces
				*	this structure was designed for export utility
				*/
				if (c.feature == ofxFaceTracker2Landmarks::Feature::LEFT_EYE) {
					if(enable_left_eye != c.enabled)c.enabled = enable_left_eye;
					c.cut.colour = left_eye_c;
				}
				if (c.feature == ofxFaceTracker2Landmarks::Feature::RIGHT_EYE) {
					if(enable_right_eye != c.enabled)c.enabled = enable_right_eye;
					c.cut.colour = right_eye_c;
				}
				if (c.feature == ofxFaceTracker2Landmarks::Feature::OUTER_MOUTH) {
					if(enable_mouth != c.enabled)c.enabled = enable_mouth;
					c.cut.colour = mouth_c;
				}
				c.cut.update(lm.getImageFeature(c.feature));
			}
		}
	}

	void initNewFace( ) {
		PolyCuts re("left eye"), le("right eye"), m("mouth");

	struct LandmarkCut 
		lefteye { ofxFaceTracker2Landmarks::Feature::LEFT_EYE, re, true, re.label },
		righteye{ ofxFaceTracker2Landmarks::Feature::RIGHT_EYE, le, true, le.label },
		mouth{ ofxFaceTracker2Landmarks::Feature::OUTER_MOUTH, m, true, m.label };

		vector<LandmarkCut> face;

		face.push_back(lefteye);
		face.push_back(righteye);
		face.push_back(mouth);

		faces.push_back(face);
	}

	void checkFaces() {
		if (faces.size() > 0) {
			time = ofGetElapsedTimeMillis();
			bool kill = false;
			int id;
			for (int i = 0; i < faces.size(); ++i) {
				if (faces[i][0].cut.alive && time - timeout > faces[i][0].cut.timetrack) {
					kill = true; id = i;
				}
			}
			if(kill)faces.erase(faces.begin() + id);
			//@TODO: here remove the stuff from gui too... 
			//figure out how without removing eveyrthing
		}
	}

	void draw() {
	for(auto& f : faces)
		for (auto& c : f)
			if(c.enabled)c.cut.draw();
	}	

	void draw(ofTexture* tex) {
		for (auto& f : faces)
			for (auto& c : f)
				if (c.enabled) {
					c.cut.set();
					getCutTexture(c.cut, *tex).draw(c.cut.getPos());
				}
	}

	void drawDebug() {
		for (auto& f : faces)
			for (auto& c : f)
				c.cut.draw();
	}

	void exportCuts( ofTexture *tex ){
		for (auto& f : faces)
			for (auto& c : f){
				if (c.enabled) {
					glm::vec2 size = c.cut.getSize();
					////export
					ofPixels tmp_pix;
					getCutTexture(c.cut, *tex).readToPixels(tmp_pix);
					ofImage new_img(tmp_pix);
					new_img.allocate(int(size.x), int(size.y), OF_IMAGE_COLOR_ALPHA);
					new_img.save("outputs/cuts_" + c.label + "_" + ofToString(ofGetMonth()) + "_" + ofToString(ofGetDay()) + "_" + ofToString(ofGetYear()) + "_" + ofToString(ofGetHours()) + "_" + ofToString(ofGetMinutes()) + ofToString(ofGetSeconds()) + ".png", OF_IMAGE_QUALITY_BEST);
				}
			}
	}

	ofTexture getCutTexture( PolyCuts c, ofTexture tex ){

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

		gui.add(timeout.set("face timeout", 0.0, 0.0, 10000.0));

		gui.add(enable_left_eye.set("enable left eye", true));
		gui.add(enable_right_eye.set("enable right eye", true));
		gui.add(enable_mouth.set("enable mouth", true));

		gui.add(left_eye_c.set("left eye colour", ofColor(255, 255, 255, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));
		gui.add(right_eye_c.set("right eye colour", ofColor(255, 255, 255, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));
		gui.add(mouth_c.set("mouth colour", ofColor(255, 255, 255, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));

		///add static cuts gui
		gui.add(PolyCuts::enable_clear.set("enable clear", true));
		gui.add(PolyCuts::enable_subpath.set("enable subpath", false));
		gui.add(PolyCuts::enable_curve.set("enable curve", true));
		gui.add(PolyCuts::enable_scale.set("enable scale", false));
		gui.add(PolyCuts::scale_x.set("scale x", 1.0, 0.0, 3.0));
		gui.add(PolyCuts::scale_y.set("scale y", 1.0, 0.0, 3.0));
		gui.add(PolyCuts::curve_reso.set("curve reso", 200, 0, 1000));
	}

	ofParameterGroup gui;
	ofParameter<bool> enable_left_eye, enable_right_eye, enable_mouth;
	ofParameter<ofColor> left_eye_c, right_eye_c, mouth_c;
	ofParameter<float> timeout;

	vector< vector<LandmarkCut> > faces;

private:

	ofFbo save_fbo;
	unsigned long time;
	

};
