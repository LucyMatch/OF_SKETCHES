#pragma once

#include "Cuts.h"

class PolyCuts : public BaseCut {

public:
	PolyCuts() {
		initGui();
		shape.setMode(ofPath::POLYLINES);
	}

	virtual void update(ofPolyline _p) {

		if(enable_clear)shape.clear();
		if (enable_subpath)shape.newSubPath();

		if (enable_curve)shape.curveTo(_p[0]);

		for (auto& p : _p) {
			if (enable_curve)
				shape.curveTo(p);
			else
				shape.lineTo(p);
		}

		if (enable_curve)shape.curveTo(_p[0]);

		if (enable_scale)shape.scale(w, h);
			
		shape.close();

		
	}

	 virtual void initGui() {
		//add any additional controls here
		 gui.add(enable_clear.set("enable shape clear", true));
		 gui.add(enable_subpath.set("enable subpath", false));
		 gui.add(enable_curve.set("enable curve", false));
		 gui.add(enable_scale.set("enable scale", false));
	}

private:

	ofParameter<bool> enable_clear, enable_subpath, enable_curve, enable_scale;

};