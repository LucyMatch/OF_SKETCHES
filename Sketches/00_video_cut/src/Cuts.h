/*
	version 1
	simple class + polymorph classes building shapes
	these shapes are handled by the cutmanager
	and applied to textures to "mask them"
	and export new assets based on "cuts"
*/
#pragma once

#include "ofMain.h"
#include "ofxGui.h"

/* 
	base cut 
	basic shapes where position + dims are defined
*/

#pragma mark BaseCut
class BaseCut {

	public: 

		BaseCut(){
			initGui();
		}

		void setup() {}

		void draw() {
			ofPushStyle();
				ofSetColor(colour);
				shape.draw();
			ofPopStyle();
		}

		void update( int x, int y ) {

			shape.clear();

			if (shape.getCircleResolution() != circle_reso)
				shape.setCircleResolution(circle_reso);

			switch (shape_mode){
			case 0 :
				shape.circle( x, y, r );
				break;
			case 1 :
				shape.ellipse( x, y, w, h );
				break;
			case 2 :
				shape.rectRounded( x - (w / 2), y - (h / 2), w, h, r );
				break;
			case 3 :
				shape.rectangle( x - (w/2), y - (h/2), w, h );
				break;
			case 4 :
				//equilateral triangle w/ x & y in middle
				a = glm::vec2(x, y + (sqrt(3) / 3) * (r));
				b = glm::vec2(x - (r) / 2, y - (sqrt(3) / 6) * (r));
				c = glm::vec2(x + (r) / 2, y - (sqrt(3) / 6) * (r));
				shape.triangle( a, b, c );
				break;
			default:
				break;
			}

			shape.close();

		}

		void initGui() {
			gui.setName("shape controls");
			gui.add(colour.set("colour", ofColor(255, 255, 255, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));
			gui.add(shape_mode.set("cut shape", 0, 0, 4));
			gui.add(r.set("cut radius", 100, 1, 1000));
			gui.add(w.set("cut width", 100, 1, 1000));
			gui.add(h.set("cut height", 100, 1, 1000));
			gui.add(circle_reso.set("circle resolution", 250, 1, 1000));
		}

		//vars
		ofPath shape;
		glm::vec2 a, b, c;	//for triangle points

		ofParameterGroup gui;
		ofParameter<float> w, h, r;
		ofParameter<int> shape_mode, circle_reso;
		ofParameter<ofColor> colour;
};