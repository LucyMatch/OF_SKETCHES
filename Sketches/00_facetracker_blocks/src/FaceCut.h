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
	FaceBlockCut(string _label = "default") {
		label = _label;
	}
	virtual void draw() {
		shape.draw();
	}

	virtual void update(ofRectangle rect) {

		shape.clear();
		shape.rectangle(rect.x, rect.y, rect.getWidth(), rect.getHeight());
		set();

		//update tracker
		if (!alive)alive = true;
		timetrack = ofGetElapsedTimeMillis();

	}

	string getLabel() { return label; }
	bool isAlive() { return alive; }
	unsigned long getTimetrack() { return timetrack; }

private:

	unsigned long timetrack;
	bool alive = false;
	string label;
	
};
