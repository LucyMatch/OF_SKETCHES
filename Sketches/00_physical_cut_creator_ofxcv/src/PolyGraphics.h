/*
* 
* just testing some drawing effects that can be down with polylines
* this would exist as a polymorph or something - else - just want easy tests for  now...

*/
#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class PolyGraphics {

public:


	PolyGraphics() {
		initGui();
	}

    //--------------------------------------------------------------
	void draw(ofPolyline poly) {
        if (enable_draw_shapes)
            drawShapes(poly);
        if (enable_draw_paths)
            drawPaths(poly);
        counter = ++counter % std::size(palette);
	}
     
    //--------------------------------------------------------------
	void drawShapes(ofPolyline& poly) {

        ofPushStyle();

            if(enable_poly_simplify)poly.simplify(poly_simplify);

            if (enable_thick_outline)drawThickOutline(poly);

            //fill
            ofSetColor(palette[counter]);
            ofFill();
            ofSetLineWidth(0);
            ofSetPolyMode(OF_POLY_WINDING_NONZERO);
            ofBeginShape();
            for (auto p : poly.getVertices()) {
                ofVertex(p);
            }
            ofEndShape();

            if (enable_outline) {
                //outline
                ofSetColor(outline_colour);
                ofNoFill();
                ofSetLineWidth(outline_width);
                ofBeginShape();
                for (auto p : poly.getVertices()) {
                    ofVertex(p);
                }
                ofEndShape();
            }

        ofPopStyle();
	}	
    
    //--------------------------------------------------------------
	void drawPaths(ofPolyline& poly) {
        ofPushStyle();

            if (enable_poly_simplify)poly.simplify(poly_simplify);
            if (enable_thick_outline)drawThickOutline(poly);

            ofPath path;
            path.setCurveResolution(200);
            path.setFillColor(palette[counter]);

            for (auto p : poly) {
                path.curveTo(p.x, p.y);
            }
            path.simplify(path_simplify);
            path.draw();

            if (enable_outline) {
                ofPath outline;
                outline = path;
                outline.setColor(outline_colour);
                outline.setFilled(false);
                outline.setStrokeWidth(outline_width);
                outline.draw();
            }
        ofPopStyle();
	}

    //--------------------------------------------------------------
    void drawThickOutline(ofPolyline& poly) {

    }

    void setPalette(vector<ofColor> p) {
        palette = p;
        counter = 0;
    }

    //--------------------------------------------------------------
	void initGui() {

        gui.setName("Poly Graphics");
        gui.add(enable_outline.set("enbale outline", true));
        gui.add(outline_width.set("outline", 5, 0, 10));
        gui.add(outline_colour.set("outline colour", ofColor(0, 0, 0, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));
        gui.add(enable_thick_outline.set("enbale thick", true));
        gui.add(outline_thick.set("outline thick", 1, 1, 2)); 
        gui.add(thick_outline_colour.set("thick outline colour", ofColor(0, 0, 0, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));
        gui.add(enable_poly_simplify.set("enable simplify", false));
        gui.add(poly_simplify.set("simplify", 0.5, 0.0, 1.0));

        ofParameterGroup _grad;
        _grad.setName("Gradient");
        _grad.add(enable_gradient.set("enable grad", false));
        _grad.add(grad_itr.set("grad iterations", 10, 1, 200));
        _grad.add(grad_stops.set("grad stops", 2, 2, 12));

        ofParameterGroup _shape;
        _shape.setName("Shape");
        _shape.add(enable_draw_shapes.set("enable shapes", false));

        ofParameterGroup _path;
        _path.setName("Path");
        _path.add(enable_draw_paths.set("enable paths", false));
        _path.add(enable_path_simplify.set("enable path simplify", false));
        _path.add(path_simplify.set("path simplify", 0.5, 0.0, 1.0));

        gui.add(_path);
        gui.add(_shape);
        gui.add(_grad);
	}

	//--------------------------

	//colours for fun drawing
	vector<ofColor> palette;
	int counter = 0;

	//GUI
	ofParameterGroup gui;
	ofParameter<bool> enable_gradient, enable_draw_shapes, enable_draw_paths, enable_poly_simplify, enable_outline, enable_thick_outline, enable_path_simplify;
	ofParameter<float> grad_itr, grad_stops, outline_width, outline_thick, poly_simplify, path_simplify;
    ofParameter<ofColor> outline_colour, thick_outline_colour;

};
