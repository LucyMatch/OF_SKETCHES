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
void ShapeDetector::initGui() {
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
