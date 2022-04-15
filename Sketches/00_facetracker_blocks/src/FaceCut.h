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
	FaceBlockCut(int _label = 0) {
		label = _label;
	}
	virtual void draw() {
		shape.draw();
	}

	virtual void update(ofRectangle rect) {

		//if (ease_to_new) {
		//	target_size = glm::vec2(rect.getWidth(), rect.getHeight());
		//	target_pos.push_back(rect.position);
		//}

		glm::vec2 p, s;
		p = rect.getPosition();
		s = glm::vec2(rect.getWidth(), rect.getHeight());
		

		//update pos & size if need be
		//if (ease_to_new) {

		//	ofRectangle rect_copy = rect;

		//	if (target_pos.size() > 0) {
		//		bool x_in_range = ofInRange(getPos().x, target_pos[0].x, target_pos[0].x + 0.2),
		//			y_in_range = ofInRange(getPos().y, target_pos[0].y, target_pos[0].y + 0.2);
		//		p.x = x_in_range ? p.x : ofLerp(getPos().x, target_pos[0].x, 0.1);
		//		p.y = y_in_range ? p.y : ofLerp(getPos().y, target_pos[0].y, 0.1);
		//		if (x_in_range && y_in_range)target_pos.erase( target_pos.begin() );
		//	}
		//	bool w_in_range = ofInRange(getSize().x, target_size.x, target_size.x + 0.2),
		//		h_in_range = ofInRange(getSize().y, target_size.y, target_size.y + 0.2);
		//	s = (w_in_range && h_in_range) ? s : glm::vec2(ofLerp(getSize().x, target_size.x, 0.1), ofLerp(getSize().y, target_size.y, 0.1));

		//}

		//update the shape
		shape.clear();
		shape.rectangle(p.x, p.y, s.x, s.y);
		set();	

		//update tracker
		if (!alive) alive = true;	//set to active
		//reset time
		timetrack = ofGetElapsedTimeMillis();

	}

	string getLabel() { return label; }
	bool isAlive() { return alive; }
	unsigned long getTimetrack() { return timetrack; }

private:

	//glm::vec2 target_size;
	//vector<glm::vec2> target_pos;
	unsigned long timetrack;
	bool alive = false;
	string label;
	//bool ease_to_new = true;
	
};
