#pragma once

#include "Cuts.h"

class PolyCuts : public BaseCut {

public:
	PolyCuts(string _label = "default");
	virtual void update();
	virtual void update(ofPolyline _p);
	//@TODO: for mouth etc things with iner shapes
	virtual void update(ofPolyline _p, ofPolyline _innerp);

	static ofParameter<bool> enable_clear, enable_subpath, enable_curve, enable_scale;
	static ofParameter<float> scale_x, scale_y;
	static ofParameter<int> curve_reso;

	unsigned long timetrack;
	bool alive = false;
	string label;

private:

};
