#include "ShapeDetector.h"

//--------------------------------------------------------------
ShapeDetector::ShapeDetector() {
    initGui();
}

//--------------------------------------------------------------
void ShapeDetector::update(ofPixels p) {

    //@TODO : if using canvas field - draw subsection 
    //we want it centered + resized to full screen

    if (p.getWidth() > 0) {

        //check sizing
        if (p.getWidth() != colorImg.getWidth() || p.getHeight() != colorImg.getHeight())
            sizeImgs( p.getWidth(),p.getHeight());

        colorImg.setFromPixels(p);
        grayImage = colorImg;

        if (learn_background) {
            grayBg = grayImage;
            learn_background = false;
        }

        grayDiff.absDiff(grayBg, grayImage);
        grayDiff.threshold(diff_thresh);

        ofxCvGrayscaleImage _temp = grayDiff;

        //if output dims > 0 then we want to resize
        if (output_dims.x > 0 || output_dims.y > 0) {
            _temp.resize(output_dims.x, output_dims.y);
        }

        //find contours
        contourFinder.findContours(_temp, cmin, cmax, considered, choles, capprox);
    }
}

//--------------------------------------------------------------
void ShapeDetector::draw() {

    //hardcoding some colors...
    int c = 0;
    ofColor cs[] = {ofColor(246, 189, 96), ofColor(245, 202, 195) , ofColor(132, 165, 157) , ofColor(242, 132, 130) };

    //just drawing them - really i sthink we wanna keep track + store them - maybe they are an extension of base cut? or i extend blobs?
    //i think thats what blobs does though.... need to review how that works

    ofPushStyle();

    for (auto b : contourFinder.blobs) {

        //fill
        ofSetColor(cs[c]);
        ofFill();
        ofSetLineWidth(0);
        ofSetPolyMode(OF_POLY_WINDING_NONZERO);
        ofBeginShape();
        for (auto p : b.pts) {
            ofVertex(p);
        }
        /*ofVertex(b.pts[b.nPts]);*/
        ofEndShape();

        //outline
        ofSetColor(ofColor(0,0,0));
        ofNoFill();
        ofSetLineWidth(5);
        ofBeginShape();
        for (auto p : b.pts) {
            ofVertex(p);
        }
        /*ofVertex(b.pts[b.nPts]);*/
        ofEndShape();
        //b.draw();

        
        //if (b.hole) {
        //    ofDrawBitmapString("hole",
        //        b.boundingRect.getCenter().x,
        //        b.boundingRect.getCenter().y);
        //}

        c = ++c % std::size(cs);

    }

    ofPopStyle();

}

//--------------------------------------------------------------
void ShapeDetector::drawData() {
    contourFinder.draw();

    for (auto b : contourFinder.blobs) {

        ofSetColor(ofColor(255, 0, 0));
        ofRectangle r = b.boundingRect;
        ofDrawRectangle(r);

        ofSetColor(255);
        b.draw();


        if (b.hole) {
            ofDrawBitmapString("hole",
                b.boundingRect.getCenter().x,
                b.boundingRect.getCenter().y);
        }
    }
}

//--------------------------------------------------------------
void ShapeDetector::drawDebug() {
    int cols = 2, width = 0, height = 0, x = 0, y = 0;
    width = ofGetWidth() / cols;
    height = width * (colorImg.getHeight() / colorImg.getWidth());

    colorImg.draw(x + width, y, width, height);
    grayImage.draw(x, y + height, width, height);
    grayBg.draw(x + width, y + height, width, height);
    grayDiff.draw(x, y + (height * 2), width, height);
    contourFinder.draw(x + width, y + (height * 2), width, height);
}

//--------------------------------------------------------------
void ShapeDetector::sizeImgs(int w, int h) {
    colorImg.allocate(w, h);
    grayImage.allocate(w, h);
    grayBg.allocate(w, h);
    grayDiff.allocate(w, h);
    learn_background = true;
}

//--------------------------------------------------------------
void ShapeDetector::initGui() {
    gui.add(enable_FOV.set("enable FOV", false));
    gui.add(diff_thresh.set("diff threshold", 80, 0, 500));
    gui.add(cmin.set("contour min", 20, 0, 1000));
    gui.add(cmax.set("contour max", 500, 0, 1920 * 1080));
    gui.add(considered.set("contour considered", 10, 0, 100));
    gui.add(choles.set("contour holes", true));
    gui.add(capprox.set("contour approximation", false));
}

//--------------------------------------------------------------
void ShapeDetector::setOutputDims(glm::vec2 d) {
    output_dims = d;
}

//--------------------------------------------------------------
void ShapeDetector::setFOV(bool state){
    if (state) {
        FOV.set(ofRectangle());
        set_canvas = true;
        FOV_origin = false;
    }
    else {
        set_canvas = false;
    }
}

//--------------------------------------------------------------
void ShapeDetector::setFOV(int x, int y) {
    if (!FOV_origin) {
        FOV.setPosition(x, y);
        FOV_origin = true;
    }
    else{
        FOV.setWidth(abs(FOV.getX() - x));
        FOV.setHeight(abs(FOV.getY() - y));
        setFOV(false);
    }
}

//--------------------------------------------------------------
void ShapeDetector::drawFOV() {
    ofDrawRectangle(FOV);
}

//--------------------------------------------------------------
void ShapeDetector::drawLiveFOVConfig(int x, int y) {
    ofPushStyle();
    ofSetColor(ofColor(255, 255, 255, 50));
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    ofSetColor(ofColor(255, 255, 255, 255));
    colorImg.draw(0, 0);
    if (FOV_origin) {
        ofSetColor(ofColor(255, 0, 0, 150));
        ofNoFill();
        ofDrawRectangle(FOV.getX(), FOV.getY(), abs(FOV.getX() - x), abs(FOV.getY() - y));
    }
    ofPopStyle();
}
