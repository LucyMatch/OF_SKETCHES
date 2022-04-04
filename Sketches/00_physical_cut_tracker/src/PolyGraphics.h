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
	void draw(ofPolyline poly, ofColor fill = ofColor(255,255,255,255)) {
        if (enable_draw_shapes)
            drawShapes(poly, fill);
        if (enable_draw_paths)
            drawPaths(poly, fill);
        if (enable_gradient)
            drawGradient(poly);
        counter = ++counter % std::size(palette);
	}
     
    //--------------------------------------------------------------
	void drawShapes(ofPolyline& poly, ofColor fill) {

        ofPushStyle();

            if(enable_poly_simplify)poly.simplify(poly_simplify);

            if (enable_drop_shadow)drawThickOutline(poly, false, drop_shadow_colour);
            if (enable_thick_outline)drawThickOutline(poly, true, thick_outline_colour);

            //fill
            ofSetColor(fill);
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
	void drawPaths(ofPolyline& poly, ofColor fill) {
        ofPushStyle();

            if (enable_poly_simplify)poly.simplify(poly_simplify);
           

            ofPath path;
            path.setCurveResolution(200);
            path.setFillColor(fill);

            for (auto p : poly) {
                path.curveTo(p.x, p.y);
            }
            path.simplify(path_simplify);

            if (enable_drop_shadow)drawThickOutline(path, false, drop_shadow_colour);
            if (enable_thick_outline)drawThickOutline(path, true, thick_outline_colour);

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
    void drawGradient(ofPolyline& poly) {
        ofPushStyle();

        if (enable_poly_simplify)poly.simplify(poly_simplify);


        ofPath path;
        path.setCurveResolution(200);
        path.setFillColor(palette[counter]);

        for (auto p : poly) {
            path.curveTo(p.x, p.y);
        }
        path.simplify(path_simplify);

        if(enable_drop_shadow)drawThickOutline(path, false, drop_shadow_colour);
        if (enable_thick_outline)drawThickOutline(path, true, thick_outline_colour);

        //gradient
        //just start with simple 2 stop - but eventually we want to control amt of stops..
        //start_c
        //end_c
        //itr // how many steps between scale 0 - 1
        int c_1 = counter, c_2 = (counter +1 )%palette.size();
        float stop_range = 1 / (grad_stops - 1);

        for (int x = 0; x < (grad_stops - 1); x++) {
            for (int i = grad_itr; i > 0; i--) {
                float _scale = ofMap(i, 0, grad_itr - 1, (x * stop_range) + 0.1, stop_range, true);
                ofPushMatrix();
                ofScale(_scale);
                ofColor _c = palette[c_1].getLerped(palette[c_2], _scale);
                path.setFillColor(_c);
                path.draw();
                ofPopMatrix();
            }
        }


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
    void drawThickOutline(ofPolyline poly, bool translate, ofColor c) {
        ofPushMatrix();
        ofPushStyle();
        //we need to translate
        //although this is good "drop shadow"
        if(translate)ofTranslate(0,0);
        ofScale(outline_thick);
        if (translate)ofTranslate(poly.getCentroid2D());
            //fill
            ofSetColor(c);
            ofFill();
            ofSetLineWidth(0);
            ofSetPolyMode(OF_POLY_WINDING_NONZERO);
            ofBeginShape();
            for (auto p : poly.getVertices()) {
                ofVertex(p);
            }
            ofEndShape();
        ofPopStyle();
        ofPopMatrix();
    }    
    
    //--------------------------------------------------------------
    void drawThickOutline(ofPath path, bool translate, ofColor c) {
        //we need to translate
        //although this is good "drop shadow"
        path.setFillColor(c);
        ofPushMatrix();
        if (translate)ofTranslate(0, 0);
        ofScale(outline_thick);
        path.draw();
        if (translate)ofTranslate(path.getOutline()[0].getCentroid2D());
        ofPopMatrix();
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
        gui.add(outline_thick.set("outline thick", 1, 1, 5)); 
        gui.add(thick_outline_colour.set("thick outline colour", ofColor(0, 0, 0, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));
        gui.add(enable_drop_shadow.set("enable drop shadow", false));
        gui.add(drop_shadow_colour.set("drop dshadow colour", ofColor(0, 0, 0, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));
        gui.add(enable_poly_simplify.set("enable simplify", false));
        gui.add(poly_simplify.set("simplify", 0.5, 0.0, 10.0));

        ofParameterGroup _grad;
        _grad.setName("Gradient");
        _grad.add(enable_gradient.set("enable grad",  false));
        _grad.add(grad_itr.set("grad iteration amt", 25, 1, 200));
        _grad.add(enable_gradient_itr_animate.set("grad iteration aanimate", false));
        _grad.add(grad_itr_animate_rate.set("iteration aanimate rate", 0.25, 0.01, 5));
        _grad.add(grad_stops.set("grad stops", 2, 2, 12));

        ofParameterGroup _shape;
        _shape.setName("Shape");
        _shape.add(enable_draw_shapes.set("enable shapes", false));

        ofParameterGroup _path;
        _path.setName("Path");
        _path.add(enable_draw_paths.set("enable paths", true));
        _path.add(enable_path_simplify.set("enable path simplify", false));
        _path.add(path_simplify.set("path simplify", 0.5, 0.0, 10.0));

        gui.add(_path);
        gui.add(_shape);
        gui.add(_grad);
	}

	//--------------------------

	//colours for fun drawing
	vector<ofColor> palette;
	int counter = 0;

    //misc
    bool grad_animate_dir_lr = true;

	//GUI
	ofParameterGroup gui;
	ofParameter<bool> enable_gradient, enable_gradient_itr_animate, enable_draw_shapes, enable_draw_paths, enable_poly_simplify, enable_outline, enable_thick_outline, enable_path_simplify, enable_drop_shadow;
	ofParameter<float> grad_itr, grad_stops, outline_width, outline_thick, poly_simplify, path_simplify, grad_itr_animate_rate;
    ofParameter<ofColor> outline_colour, thick_outline_colour, drop_shadow_colour;

};
