#pragma once

/*CutManager v3*/
/*
*	this is a build on landmarkcut manager from facetracker 2 weird face
* we are doing simplier stuff so im stripping this but keeping some of the utlities in place

*/

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxFaceTracker2.h"
#include "FaceCut.h"

/*
struct for storing our cuts - with relation to landmarks
*/
struct Face {
	FaceBlockCut cut;
	bool enabled;
	int label;
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

class FaceCutManager {

public:

	FaceCutManager() {
		initGui();
	}

	//sending ALL instances... 
	void update(vector<ofxFaceTracker2Instance> instances) {

		for (auto t : instances) {
			int face_index = getFace(t.getLabel());

			if (face_index < 0)
				face_index = initNewFace(t.getLabel(), t.getBoundingBox());

			if (face_index >= 0)
				update(face_index, t.getBoundingBox());
		}
		checkFaces();
	}

	int getFace(int _label) {
		for (int i = 0; i < (int)faces.size(); i++) {
			if (faces[i].label == _label) return i;
		}
		return -1;
	}

	void update(int index, ofRectangle updated_bounding) {
		faces[index].cut.update(updated_bounding);
	}

	int initNewFace(int _label, ofRectangle new_bounding) {
		FaceBlockCut cut;
		struct Face new_face {cut, true, _label};
		//new_face.cut.update(new_bounding);
		faces.push_back(new_face);
		return (int)faces.size() - 1;
	}

	void checkFaces() {
		if (faces.size() > 0) {
			time = ofGetElapsedTimeMillis();
			bool kill = false;
			int id;
			for (int i = 0; i < faces.size(); ++i) {
				if (faces[i].cut.isAlive() && time - timeout > faces[i].cut.getTimetrack()) {
					kill = true; id = i;
				}
			}
			if (kill)faces.erase(faces.begin() + id);
		}
	}

	void draw() {
		for (auto& f : faces)
			f.cut.draw();
	}

	void draw(ofTexture* tex) {
		for (auto& f : faces)
				if (f.enabled) {
					f.cut.set();
					getCutTexture(f.cut, *tex).draw(f.cut.getPos());
				}
	}

	void drawDebug() {
		for (auto& f : faces)
				f.cut.draw();
	}

	void exportCuts(ofTexture* tex) {
		for (auto& f : faces)
			if (f.enabled) {
				glm::vec2 size = f.cut.getSize();
				////export
				ofPixels tmp_pix;
				getCutTexture(f.cut, *tex).readToPixels(tmp_pix);
				ofImage new_img(tmp_pix);
				new_img.allocate(int(size.x), int(size.y), OF_IMAGE_COLOR_ALPHA);
				new_img.save("outputs/cuts_" + ofToString(f.label) + "_" + ofToString(ofGetMonth()) + "_" + ofToString(ofGetDay()) + "_" + ofToString(ofGetYear()) + "_" + ofToString(ofGetHours()) + "_" + ofToString(ofGetMinutes()) + ofToString(ofGetSeconds()) + ".png", OF_IMAGE_QUALITY_BEST);
			}
	}

	ofTexture getCutTexture(FaceBlockCut c, ofTexture tex) {

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
		gui.add(timeout.set("face timeout", 10.0, 10.0, 10000.0));
	}

	ofParameterGroup gui;
	ofParameter<float> timeout;
	vector<Face> faces;

private:

	ofFbo save_fbo;
	unsigned long time;

};
