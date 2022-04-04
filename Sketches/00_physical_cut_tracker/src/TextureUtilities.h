/*TextureUtilities v1*/
#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class TextureUtlities {

private:

public:

	ofParameterGroup gui;
	ofParameter<float> save_pad;

	TextureUtlities() {
		initGui();
	}

	void initGui() {
		gui.setName("cut manager");
		gui.add(save_pad.set("save padding", 5, 0, 100));
	}

	ofTexture getCutTexture(ofTexture mask, ofTexture content) {
		ofFbo temp_fbo, save_fbo;
		temp_fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
		temp_fbo.getTexture().setSwizzle(GL_TEXTURE_SWIZZLE_A, GL_RED);

		ofPushStyle();
		temp_fbo.begin();
			ofSetColor(0, 0, 0, 255);
			ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
			mask.draw(0,0);
		temp_fbo.end();
		ofPopStyle();

		content.setAlphaMask(temp_fbo.getTexture());

		save_fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
		save_fbo.begin();
			content.draw(0, 0);
		save_fbo.end();

		return save_fbo.getTexture();
	}

	ofTexture getCutTexture(ofTexture mask, ofTexture content, glm::vec2 pos, glm::vec2 size) {

		ofFbo temp_fbo, save_fbo;
		temp_fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
		temp_fbo.getTexture().setSwizzle(GL_TEXTURE_SWIZZLE_A, GL_RED);

		ofPushStyle();
		temp_fbo.begin();
			ofSetColor(0, 0, 0, 255);
			ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
			ofSetColor(255, 255, 255, 255);
			mask.draw(0, 0);
		temp_fbo.end();
		ofPopStyle();


		content.setAlphaMask(temp_fbo.getTexture());

		size.x -= save_pad;
		size.y -= save_pad;

		save_fbo.allocate(size.x, size.y, GL_RGBA);
		save_fbo.begin();
			content.drawSubsection(0, 0, size.x, size.y, pos.x, pos.y);
		save_fbo.end();

		return save_fbo.getTexture();
	}

	void exportCut(ofTexture mask, ofTexture content, int id) {
		glm::vec2 size(ofGetWidth(), ofGetHeight());
		////export
		ofPixels tmp_pix;
		getCutTexture(mask, content).readToPixels(tmp_pix);
		ofImage new_img(tmp_pix);
		new_img.allocate(int(size.x), int(size.y), OF_IMAGE_COLOR_ALPHA);
		new_img.save("outputs/cuts_" + ofToString(id) + "_" + ofToString(ofGetMonth()) + "_" + ofToString(ofGetDay()) + "_" + ofToString(ofGetYear()) + "_" + ofToString(ofGetHours()) + "_" + ofToString(ofGetMinutes()) + ofToString(ofGetSeconds()) + ".png", OF_IMAGE_QUALITY_BEST);
	}

	void exportCut(ofTexture mask, ofTexture content, glm::vec2 pos, glm::vec2 size, int id) {
		////export
		ofPixels tmp_pix;
		getCutTexture(mask, content, pos, size).readToPixels(tmp_pix);
		ofImage new_img(tmp_pix);
		new_img.allocate(int(size.x), int(size.y), OF_IMAGE_COLOR_ALPHA);
		new_img.save("outputs/cuts_" + ofToString(id) + "_" + ofToString(ofGetMonth()) + "_" + ofToString(ofGetDay()) + "_" + ofToString(ofGetYear()) + "_" + ofToString(ofGetHours()) + "_" + ofToString(ofGetMinutes()) + ofToString(ofGetSeconds()) + ".png", OF_IMAGE_QUALITY_BEST);
	}
};