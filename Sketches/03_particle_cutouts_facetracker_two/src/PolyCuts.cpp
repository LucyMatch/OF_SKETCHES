#pragma once

#include "PolyCuts.h"

ofParameter<bool> PolyCuts::enable_clear = true,
PolyCuts::enable_subpath = false,
PolyCuts::enable_curve = true,
PolyCuts::enable_scale = false,
PolyCuts::enable_shape_mode = false;
ofParameter<float> PolyCuts::scale_x = 1.0, PolyCuts::scale_y = 1.0;
ofParameter<int> PolyCuts::curve_reso = 500;
ofParameter<int> PolyCuts::shape_mode = 0;


PolyCuts::PolyCuts(string _label) {
		label = _label;
		shape.setMode(ofPath::POLYLINES);
}

void PolyCuts::draw() {
	ofPushStyle();
		ofSetColor(colour);
		if (enable_shape_mode) {
			mod_shape.setColor(colour);
			mod_shape.draw();
		}
		else {
			shape.setColor(colour);
			shape.draw();
		}
	ofPopStyle();
}

void PolyCuts::update() {
	if (enable_scale) {
		set();
		shape.translate( -getCenter() );
		shape.scale(scale_x, scale_y);
		shape.translate( getCenter() );
	}
		shape.setCurveResolution(curve_reso);
}

void PolyCuts::update(ofPolyline _p) {

	ofPushMatrix();
		if (enable_clear)shape.clear();
		if (enable_subpath)shape.newSubPath();

		if (enable_curve)shape.curveTo(_p[0]);

		for (auto& p : _p) {
			if (enable_curve)
				shape.curveTo(p);
			else
				shape.lineTo(p);
		}

		if (enable_curve)shape.curveTo(_p[0]);

		shape.close();

		update();
		set();

		if (enable_shape_mode) {
			auto pos = getCenter();
			updateModShape(pos.x, pos.y);
		}
	ofPopMatrix();

		//update tracker
		if (!alive)alive = true;
		timetrack = ofGetElapsedTimeMillis();

}


void PolyCuts::updateModShape(int x, int y) {

	mod_shape.clear();

	if (mod_shape.getCircleResolution() != circle_reso)
		mod_shape.setCircleResolution(circle_reso);

	switch (shape_mode) {
	case 0:
		mod_shape.circle(x, y, r);
		break;
	case 1:
		mod_shape.ellipse(x, y, w, h);
		break;
	case 2:
		mod_shape.rectRounded(x - (w / 2), y - (h / 2), w, h, r);
		break;
	case 3:
		mod_shape.rectangle(x - (w / 2), y - (h / 2), w, h);
		break;
	case 4:
		//equilateral triangle w/ x & y in middle
		a = glm::vec2(x, y + (sqrt(3) / 3) * (r));
		b = glm::vec2(x - (r) / 2, y - (sqrt(3) / 6) * (r));
		c = glm::vec2(x + (r) / 2, y - (sqrt(3) / 6) * (r));
		mod_shape.triangle(a, b, c);
		break;
	default:
		break;
	}

	mod_shape.close();

}


//@TODO: for mouth etc things with iner shapes
void PolyCuts::update(ofPolyline _p, ofPolyline _innerp) {

		update();

		//if (enable_clear)shape.clear();
		//if (enable_subpath)shape.newSubPath();

		//if (enable_curve)shape.curveTo(_p[0]);

		//for (auto& p : _p) {
		//	if (enable_curve)
		//		shape.curveTo(p);
		//	else
		//		shape.lineTo(p);
		//}

		//if (enable_curve)shape.curveTo(_p[0]);

		////if (enable_scale)shape.scale(w, h);

		//shape.close();

		//set();

}



