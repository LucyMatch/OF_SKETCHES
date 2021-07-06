#pragma once

#include "Cuts.h"

class PolyCuts : public BaseCut {

public:
	PolyCuts(string label = "default") {
		initGui(label);
		shape.setMode(ofPath::POLYLINES);
	}

	virtual void update() {
		if (enable_simplify)shape.simplify(simplify);
		if (enable_tesselate)shape.tessellate();
		if (enable_scale)shape.scale(scale_x, scale_y);
		shape.setCurveResolution(curve_reso);
		shape.setStrokeColor(stroke_colour);
		shape.setStrokeWidth(stroke_w);
	}

	virtual void update(ofPolyline _p) {

		update();

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

		//if (enable_scale)shape.scale(w, h);
			
		shape.close();

		set();
		
	}


	//@TODO: for mouth etc things with iner shapes
	virtual void update(ofPolyline _p, ofPolyline _innerp) {

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

	 virtual void initGui(string label) {
		 //remove base class gui ctrls not using
		 gui.remove(w);
		 gui.remove(h);
		 gui.remove(r);
		 gui.remove(shape_mode);
		 gui.remove(circle_reso);
		 gui.setName(label);
		 
		 gui.add(enable_clear.set("enable shape clear", true));
		 gui.add(enable_subpath.set("enable subpath", false));
		 gui.add(enable_curve.set("enable curve", true));
		 gui.add(curve_reso.set("curve resolution", 200, 0, 200));

		 gui.add(enable_scale.set("enable scale", false));
		 gui.add(scale_x.set("scale x", 1.0, -1.0, 100.0));
		 gui.add(scale_y.set("scale y", 1.0, -1.0, 100.0));

		 gui.add(stroke_w.set("stroke width", 5.0, 0.0, 15.0));
		 gui.add(stroke_colour.set("stroke colour", ofColor(255,255,255,255), ofColor(0,0,0,0), ofColor(255,255,255,255)));

		 gui.add(enable_tesselate.set("enable tesselate", false));
		 gui.add(enable_simplify.set("enable simplify", false));

		 

	}

	 ofParameter<bool> enable_clear, enable_subpath, enable_curve, enable_scale;
	 ofParameter<ofColor> colour, stroke_colour;

	 ofParameter<float> simplify, scale_x, scale_y, stroke_w;
	 ofParameter<int> curve_reso;
	 ofParameter<bool> enable_tesselate, enable_simplify;

private:

};