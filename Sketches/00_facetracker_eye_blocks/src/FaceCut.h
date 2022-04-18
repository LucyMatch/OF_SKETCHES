#pragma once

#include "Cuts.h"

/*
	I hate baseCut library i.e. Cuts.h in classes
	this is an attempt to redo + clean it out - evetually replace but willl screnw up old sketches
*/

#pragma mark Cut
class Cut {

public:

	Cut() {}

	//set the bounding box / size & pos of this cut + store it
	void set() {

		if (shape.getOutline().size() > 0)
			bounding = shape.getOutline()[0].getBoundingBox();
		else
			return;

		pos = bounding.getPosition();
		size = glm::vec2(bounding.getWidth(), bounding.getHeight());
		center = bounding.getCenter();

	}

	virtual void draw() {
		shape.draw();
	}

	virtual void updateShape(ofPath _shape) {
		shape = _shape;
	}

	void updatePosition(glm::vec2 p) {
		shape.moveTo(p);
	}

	glm::vec2 getPos() { return pos; }
	glm::vec2 getSize() { return size; }
	glm::vec2 getCenter() { return center; }
	ofRectangle getBounding() { return bounding; }
	ofPath getShape() { return shape; }

//private:
	//vars
	ofPath shape;
	ofRectangle bounding;
	glm::vec2 pos, size, center;

};

#pragma mark face block cut
class FaceBlockCut : public Cut {

public:
	FaceBlockCut(int _label = 0) {
		label = _label;
	}
	virtual void draw() {
		shape.draw();
	}

	virtual void update(ofRectangle rect) {

		glm::vec2 p, s;
		p = rect.getPosition();
		s = glm::vec2(rect.getWidth(), rect.getHeight());

		//update the shape
		shape.clear();
		shape.rectangle(p.x, p.y, s.x, s.y);
		set();	

		//update tracker
		if (!alive) alive = true;	//set to active
		//reset time
		timetrack = ofGetElapsedTimeMillis();

	}

	int getLabel() { return label; }
	bool isAlive() { return alive; }
	unsigned long getTimetrack() { return timetrack; }

private:

	unsigned long timetrack;
	bool alive = false;
	int label;
	
};

#pragma mark face block cut
class EyeBlockCut : public Cut {

public:
	EyeBlockCut(string _label = "") {
		label = _label;
	}
	virtual void draw() {
		shape.draw();
	}
	virtual void update(ofRectangle rect) {

		glm::vec2 p, s;
		p = rect.getPosition();
		//s = glm::vec2(rect.getWidth(), rect.getHeight());
		s = enable_squares ? glm::vec2(rect.getWidth(), rect.getWidth()) : glm::vec2(rect.getWidth(), rect.getHeight());
		p = enable_squares ? glm::vec2(p.x, p.y - (s.y/2)) : p;
		//update the shape
		shape.clear();
		ofRectangle temp;
		temp.set(p.x, p.y, s.x, s.y);
		temp.scaleFromCenter(scale, scale);
		//shape.rectangle(p.x, p.y, s.x, s.y);
		//shape.scale(scale, scale);
		shape.rectangle(temp);
		set();
	}

	void setScale(float s) { scale = s; }
	void setPositionRange(float range) { n_range = range; }
	void setScaleRange(float range) { s_range = range; }
	void setSquares(bool state) { enable_squares = state; }
	void setUniformScaling(bool state) { enable_uniform_scaling = state; }
	
	string getLabel() { return label; }
	float getPosRange() { return n_range; }
	ofRectangle getModifiedBounding(bool centered = true) {

		ofRectangle copy = getBounding();

		if (s_range > 0) {
			auto s = ofRandom(s_range) + 1;
			auto s2 = enable_uniform_scaling ? s : ofRandom(s_range) + 1;
			if(centered)
				copy.scaleFromCenter(s, s2);
			else
				copy.scale(s, s2);
		}
		if (n_range > 0)
			copy.translate( ofRandom(-n_range, n_range), ofRandom(-n_range, n_range) );

		return copy;
	}

private:

	bool enable_squares = false, enable_uniform_scaling = true;
	float scale = 1;
	float n_range = 0;
	float s_range = 0;
	string label;

};
