#pragma once

#include "Cuts.h"

/*
@TODO: 
	x - incorporate shape modifiers ( i.e. circle around eye / square around eye etc...)
	x - add in more support for other landmarks ( curr only tested with eyes / mouth )
*/

class PolyCuts : public BaseCut {

public:
	PolyCuts(string _label = "default");
	virtual void draw();
	virtual void update();
	virtual void update(ofPolyline _p);
	virtual void updateModShape( int x, int y);
	//@TODO: for mouth etc things with iner shapes
	virtual void update(ofPolyline _p, ofPolyline _innerp);

	static ofParameter<bool> enable_clear, enable_subpath, enable_curve, enable_scale, enable_shape_mode;
	static ofParameter<float> scale_x, scale_y;
	static ofParameter<int> curve_reso, shape_mode;

	unsigned long timetrack;
	bool alive = false;
	string label;
	ofPath mod_shape;

private:

};
