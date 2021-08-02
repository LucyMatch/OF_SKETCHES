#pragma once

#include "PolyCuts.h"

ofParameter<bool> PolyCuts::enable_clear = true,
PolyCuts::enable_subpath = false,
PolyCuts::enable_curve = true,
PolyCuts::enable_scale = false;
ofParameter<float> PolyCuts::scale_x = 1.0, PolyCuts::scale_y = 1.0;
ofParameter<int> PolyCuts::curve_reso = 500;


PolyCuts::PolyCuts(string label) {
		shape.setMode(ofPath::POLYLINES);
}

void PolyCuts::update() {
		if (enable_scale)shape.scale(scale_x, scale_y);
		shape.setCurveResolution(curve_reso);
}

void PolyCuts::update(ofPolyline _p) {

		update();

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

		//if (enable_scale)shape.scale(w, h);

		shape.close();

		set();

		//update tracker
		if (!alive)alive = true;
		timetrack = ofGetElapsedTimeMillis();

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



