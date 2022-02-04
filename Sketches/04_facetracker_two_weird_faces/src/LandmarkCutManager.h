#pragma once

/*CutManager v2*/
/*
*	this keeps track of each individual landmark / for all active instances
*	we generate a cut per landmark ( left / right / mouth ) per face
*
	@TODO:
	x - add in more support for other landmarks ( curr only tested with eyes / mouth )
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
	void update(vector<ofxFaceTracker2Instance> t) {
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
					update(t[i].getLandmarks(), i);
				}
			}
		}
		checkFaces();
	}

	void update(ofxFaceTracker2Landmarks lm, int index) {
		if (faces.size() > index) {
			for (auto& c : faces[index]) {
				/*
				*	@TODO: JUST MOVE THIS TO PARAMETER GUI EVENT LISTENERS..... 
				*			SO WEHEN VAL IS UPDATED WE TRIGGER CALLBACK TO SET THESE VALS...
				*/
				switch (c.feature) {
					case(ofxFaceTracker2Landmarks::Feature::LEFT_EYE) :
						if (enable_left_eye != c.enabled)c.enabled = enable_left_eye;
						c.cut.colour = left_eye_c;
						break;					
					case(ofxFaceTracker2Landmarks::Feature::RIGHT_EYE) :
						if (enable_right_eye != c.enabled)c.enabled = enable_right_eye;
						c.cut.colour = right_eye_c;
						break;
					case(ofxFaceTracker2Landmarks::Feature::OUTER_MOUTH):
						if (enable_mouth != c.enabled)c.enabled = enable_mouth;
						c.cut.colour = mouth_c;
						break;					
					case(ofxFaceTracker2Landmarks::Feature::INNER_MOUTH):
						if (enable_innermouth != c.enabled)c.enabled = enable_innermouth;
						c.cut.colour = c_innermouth;
						break;
					case(ofxFaceTracker2Landmarks::Feature::LEFT_EYEBROW) :
						if (enable_leftbrow != c.enabled)c.enabled = enable_leftbrow;
						c.cut.colour = c_leftbrow;
						break;
					case(ofxFaceTracker2Landmarks::Feature::RIGHT_EYEBROW) :
						if (enable_rightbrow != c.enabled)c.enabled = enable_rightbrow;
						c.cut.colour = c_rightbrow;
						break;
					//case(ofxFaceTracker2Landmarks::Feature::LEFT_EYE_TOP) :
					//	if (enable_lefteyetop != c.enabled)c.enabled = enable_lefteyetop;
					//	c.cut.colour = c_lefteyetop;
					//	break;
					//case(ofxFaceTracker2Landmarks::Feature::RIGHT_EYE_TOP) :
					//	if (enable_righteyetop != c.enabled)c.enabled = enable_righteyetop;
					//	c.cut.colour = c_righteyetop;
					//	break;
					case(ofxFaceTracker2Landmarks::Feature::LEFT_JAW) :
						if (enable_leftjaw != c.enabled)c.enabled = enable_leftjaw;
						c.cut.colour = c_leftjaw;
						break;
					case(ofxFaceTracker2Landmarks::Feature::RIGHT_JAW) :
						if (enable_rightjaw != c.enabled)c.enabled = enable_rightjaw;
						c.cut.colour = c_rightjaw;
						break;
					//case(ofxFaceTracker2Landmarks::Feature::JAW) :
					//	if (enable_jaw != c.enabled)c.enabled = enable_jaw;
					//	c.cut.colour = c_jaw;
					//	break;
					//case(ofxFaceTracker2Landmarks::Feature::NOSE_BRIDGE) :
					//	if (enable_nosebridge != c.enabled)c.enabled = enable_nosebridge;
					//	c.cut.colour = c_nosebridge;
					//	break;
					case(ofxFaceTracker2Landmarks::Feature::NOSE_BASE) :
						if (enable_nosebase != c.enabled)c.enabled = enable_nosebase;
						c.cut.colour = c_nosebase;
						break;
					case(ofxFaceTracker2Landmarks::Feature::FACE_OUTLINE) :
						if (enable_outline != c.enabled)c.enabled = enable_outline;
						c.cut.colour = c_outline;
						break;
				}
				if(c.enabled)
					c.cut.update(lm.getImageFeature(c.feature));
			}
		}
	}

	void initNewFace() {
		PolyCuts le("left eye"), re("right eye"), m("mouth");
		PolyCuts let("left eye top"), ret("right eye top"), lb("left brow"), rb("right brow");
		PolyCuts lj("left jaw"), rj("right jaw"), j("jaw");
		PolyCuts im("inner mouth");
		PolyCuts nbr("nose bridge"), nb("nose base");
		PolyCuts o("face outline");

		struct LandmarkCut
			lefteye {ofxFaceTracker2Landmarks::Feature::LEFT_EYE, le, true, le.label},
			righteye{ ofxFaceTracker2Landmarks::Feature::RIGHT_EYE, re, true, re.label },
			mouth{ ofxFaceTracker2Landmarks::Feature::OUTER_MOUTH, m, true, m.label },
			leftbrow{ ofxFaceTracker2Landmarks::Feature::LEFT_EYEBROW, lb, true, lb.label },
			rightbrow{ ofxFaceTracker2Landmarks::Feature::RIGHT_EYEBROW, rb, true, rb.label },
			lefteyetop{ ofxFaceTracker2Landmarks::Feature::LEFT_EYE_TOP, let, true, let.label },
			righteyetop{ ofxFaceTracker2Landmarks::Feature::RIGHT_EYE_TOP, ret, true, ret.label }	,		
			leftjaw{ ofxFaceTracker2Landmarks::Feature::LEFT_JAW, lj, true, lj.label },
			rightjaw{ ofxFaceTracker2Landmarks::Feature::RIGHT_JAW, rj, true, rj.label },
			jaw{ ofxFaceTracker2Landmarks::Feature::JAW, j, true, j.label },
			innermouth{ ofxFaceTracker2Landmarks::Feature::INNER_MOUTH, im, true, im.label },
			nosebridge{ ofxFaceTracker2Landmarks::Feature::NOSE_BRIDGE, nbr, true, nbr.label },
			nosebase{ ofxFaceTracker2Landmarks::Feature::NOSE_BASE, nb, true, nb.label },
			outline{ ofxFaceTracker2Landmarks::Feature::FACE_OUTLINE, o, true, o.label };

			vector<LandmarkCut> face;

			face.push_back(outline);
			//face.push_back(jaw);
			face.push_back(leftjaw);
			face.push_back(rightjaw);
			//face.push_back(nosebridge);
			face.push_back(nosebase);
			face.push_back(mouth);
			face.push_back(innermouth);
			face.push_back(leftbrow);
			face.push_back(rightbrow);
			face.push_back(lefteye);
			face.push_back(righteye);
			//face.push_back(lefteyetop);
			//face.push_back(righteyetop);
			


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
			if (kill)faces.erase(faces.begin() + id);
		}
	}

	void draw() {
		for (auto& f : faces)
			for (auto& c : f)
				if (c.enabled)c.cut.draw();
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

	void exportCuts(ofTexture* tex) {
		for (auto& f : faces)
			for (auto& c : f) {
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

	ofTexture getCutTexture(PolyCuts c, ofTexture tex) {

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

		gui.add(timeout.set("face timeout", 0.0, 10.0, 10000.0));

		///add static cuts gui
		gui.add(PolyCuts::enable_curve.set("enable curve", true));
		gui.add(PolyCuts::enable_scale.set("enable scale", false));
		gui.add(PolyCuts::enable_shape_mode.set("enable shape mode", false));
		gui.add(PolyCuts::scale_x.set("scale x", 1.0, 0.0, 5.0));
		gui.add(PolyCuts::scale_y.set("scale y", 1.0, 0.0, 5.0));
		gui.add(PolyCuts::shape_mode.set("cut shape", 0, 0, 4));

		gui.add(enable_left_eye.set("enable left eye", true));
		gui.add(enable_right_eye.set("enable right eye", true));
		gui.add(enable_mouth.set("enable mouth", true));
		gui.add(enable_leftbrow.set("enable left brow", true));
		gui.add(enable_rightbrow.set("enable right brow", true));
		//gui.add(enable_lefteyetop.set("enable left eye top", true));
		//gui.add(enable_righteyetop.set("enable right eye top", true));
		gui.add(enable_leftjaw.set("enable left jaw", true));
		gui.add(enable_rightjaw.set("enable right jaw", true));
		//gui.add(enable_jaw.set("enable jaw", true));
		gui.add(enable_innermouth.set("enable inner mouth", true));
		//gui.add(enable_nosebridge.set("enable nose bridge", true));
		gui.add(enable_nosebase.set("enable nose base", true));
		gui.add(enable_outline.set("enable outline", true));

		gui.add(left_eye_c.set("left eye colour", ofColor(255, 255, 255, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));
		gui.add(right_eye_c.set("right eye colour", ofColor(255, 255, 255, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));
		gui.add(mouth_c.set("mouth colour", ofColor(255, 255, 255, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));

		gui.add(c_leftbrow.set("left brow c", ofColor(255, 255, 255, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));
		gui.add(c_rightbrow.set("right brow c", ofColor(255, 255, 255, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));
		//gui.add(c_lefteyetop.set("left eye top c", ofColor(255, 255, 255, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));
		//gui.add(c_righteyetop.set("right eye top c", ofColor(255, 255, 255, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));
		gui.add(c_leftjaw.set("left jaw c", ofColor(255, 255, 255, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));
		gui.add(c_rightjaw.set("right jaw c", ofColor(255, 255, 255, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));
		//gui.add(c_jaw.set("jaw c", ofColor(255, 255, 255, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));
		gui.add(c_innermouth.set("inner mouth", ofColor(255, 255, 255, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));
		//gui.add(c_nosebridge.set("nose bridge", ofColor(255, 255, 255, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));
		gui.add(c_nosebase.set("nose base", ofColor(255, 255, 255, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));
		gui.add(c_outline.set("outline", ofColor(255, 255, 255, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));



		//we never update these 
		//gui.add(PolyCuts::curve_reso.set("curve reso", 200, 0, 1000));
		//gui.add(PolyCuts::enable_clear.set("enable clear", true));
		//gui.add(PolyCuts::enable_subpath.set("enable subpath", false));
		PolyCuts::curve_reso.set("curve reso", 200, 0, 1000);
		PolyCuts::enable_clear.set("enable clear", true);
		PolyCuts::enable_subpath.set("enable subpath", false);
	}

	ofParameterGroup gui;
	ofParameter<bool> enable_left_eye, enable_right_eye, enable_mouth;
	ofParameter<ofColor> left_eye_c, right_eye_c, mouth_c;
	ofParameter<float> timeout;

	//@WIP : new ones testing out
	ofParameter<bool>
		enable_leftbrow,
		enable_rightbrow,
		enable_lefteyetop,
		enable_righteyetop,
		enable_leftjaw,
		enable_rightjaw,
		enable_jaw,
		enable_innermouth,
		enable_nosebridge,
		enable_nosebase,
		enable_outline;

	ofParameter<ofColor>
		c_leftbrow,
		c_rightbrow,
		c_lefteyetop,
		c_righteyetop,
		c_leftjaw,
		c_rightjaw,
		c_jaw,
		c_innermouth,
		c_nosebridge,
		c_nosebase,
		c_outline;

	vector< vector<LandmarkCut> > faces;

private:

	ofFbo save_fbo;
	unsigned long time;


};
