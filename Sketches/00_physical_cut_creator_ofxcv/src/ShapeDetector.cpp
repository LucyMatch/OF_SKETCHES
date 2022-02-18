#include "ShapeDetector.h"

using namespace ofxCv;
using namespace cv;

//--------------------------------------------------------------
ShapeDetector::ShapeDetector() {
    initGui();
}

//--------------------------------------------------------------
void ShapeDetector::update(ofPixels p) {
    if (p.getWidth() > 0) {

        if (p.getWidth() != current_frame_dims.x || p.getHeight() != current_frame_dims.y)
            sizeImgs(p.getWidth(), p.getHeight());

        output_frame.allocate(p);
        ofPixels frame = p;

        if (enable_bg_learning) {
            if (learn_background) {
                background.reset();
                learn_background = false;
            }

            background.setLearningTime(bg_learning_time);
            background.setIgnoreForeground(bg_ignore_learning_rate);
            background.setThresholdValue(bg_threshold);
            background.update(p, bg_thresh_result);
            bg_thresh_result.update();

            frame = bg_thresh_result.getPixels();
        }

        if (enable_colour_track) 
            finder.setTargetColor(target_colour, colour_track_mode ? TRACK_COLOR_HS : TRACK_COLOR_RGB);

            finder.setSortBySize(true);
            finder.setMinArea(cmin);
            finder.setMaxArea(cmax);
            finder.setInvert(cinvert);
            finder.setThreshold(cthreshold);
            finder.setFindHoles(choles);
            finder.setSimplify(csimplify);

            //resize here
            //OF_INTERPOLATE_NEAREST_NEIGHBOR =1 OF_INTERPOLATE_BILINEAR =2 OF_INTERPOLATE_BICUBIC =3
            if (enable_fullscreen || enable_manual_scale)
                frame.resize(getCurrentDims().x, getCurrentDims().y, OF_INTERPOLATE_NEAREST_NEIGHBOR);

            finder.findContours( frame );
    }
    
    
}

//--------------------------------------------------------------
void ShapeDetector::update(ofTexture tex) {
    ofFbo fbo;
    if(enable_FOV)
        fbo.allocate(FOV.width, FOV.height, GL_RGB);
    else
        fbo.allocate(tex.getWidth(), tex.getHeight(), GL_RGB);

    fbo.begin();
    if (enable_FOV)
        tex.drawSubsection(0, 0, FOV.width, FOV.height, FOV.getX(), FOV.getY());
    else
        tex.draw(0,0);
    fbo.end();

    ofPixels p;
    fbo.readToPixels(p);

    update(p);
}

//--------------------------------------------------------------
void ShapeDetector::drawInput() {
    if(output_frame.isAllocated())output_frame.draw(0, 0, getCurrentDims().x, getCurrentDims().y);
}

//--------------------------------------------------------------
void ShapeDetector::drawData() {
    finder.draw();

    int n = finder.size();
    for (int i = 0; i < n; i++) {
        if (finder.getHole(i)) {
            ofDrawBitmapStringHighlight("hole", finder.getCenter(i).x, finder.getCenter(i).y);
        }
    }

}

//--------------------------------------------------------------
void ShapeDetector::drawDebug() {

    if (output_frame.isAllocated())
        output_frame.draw(0,0);

    if(enable_bg_learning && bg_thresh_result.isAllocated())
        bg_thresh_result.draw(0,0);

    if (enable_colour_track) {
        ofPushStyle();
        ofSetColor(target_colour);
        ofDrawRectangle(0, 0 ,100,100);
        ofPopStyle();
    }

    finder.draw();
}

//--------------------------------------------------------------
void ShapeDetector::sizeImgs(int w, int h) {
    current_frame_dims = glm::vec2(w, h);
    bg_thresh_result.allocate(w,h,OF_IMAGE_COLOR);
    learn_background = true;
    calcFullscreenDims();
}

//--------------------------------------------------------------
void ShapeDetector::initGui() {

    ofParameterGroup _output;
    _output.setName("output");
    _output.add(enable_FOV.set("enable FOV", false));
    _output.add(enable_fullscreen.set("fullscreen", false));
    _output.add(enable_manual_scale.set("enable manual Scaling", false));
    _output.add(manual_scale.set("manual scale", 1, 0.1, 25));

    ofParameterGroup _method;
    _method.setName("method");
    _method.add(enable_bg_learning.set("enable BG Learning", false));
    _method.add(bg_learning_time.set("bg learning time", 0, 0, 30));
    _method.add(bg_ignore_learning_rate.set("bg ignore learning time", true));
    _method.add(bg_threshold.set("bg threshold", 10, 0, 255));
    _method.add(enable_colour_track.set("enable colour tracking", false));
    _method.add(colour_track_mode.set("c track mode hue / sat", false));


    ofParameterGroup _contours;
    _contours.setName("contours");
    _contours.add(cthreshold.set("contour threshold", 80, 0, 255));
    _contours.add(cmin.set("contour min", 0, 0, 5000));
    _contours.add(cmax.set("contour max", 10000, 0, 1920 * 1080));
    _contours.add(choles.set("contour holes", true));
    _contours.add(cinvert.set("contour invert", false));
    _contours.add(csimplify.set("contour simplify", false));

    gui.add(_method);
    gui.add(_contours);
    gui.add(_output);

    //additional listeners
    enable_colour_track.addListener( this, &ShapeDetector::updateContourMode );

}

//--------------------------------------------------------------
void ShapeDetector::setFullscreenDims(glm::vec2 d) {
    fullscreen_dims = d;
}

//--------------------------------------------------------------
void ShapeDetector::calcFullscreenDims() {

    //basing off colorImg as origin reference... 
    //as it will always be sized correctly for the incoming texture dimensions
    glm::vec2 temp;

    //keeping aspect ratio we want to scale to full screen.
    if (current_frame_dims.x > current_frame_dims.y)
        temp = glm::vec2( ofGetHeight() * (current_frame_dims.x / current_frame_dims.y), ofGetHeight());
    else 
        temp = glm::vec2( ofGetWidth(), ofGetWidth() * (current_frame_dims.y / current_frame_dims.x) );

    setFullscreenDims(temp);
}

//--------------------------------------------------------------
glm::vec2 ShapeDetector::getCurrentDims() {
    if (enable_fullscreen)
        return fullscreen_dims;
    else if (enable_manual_scale)
        return glm::vec2(current_frame_dims.x * manual_scale, current_frame_dims.y * manual_scale);
    else
        return  current_frame_dims;
}

//--------------------------------------------------------------
void ShapeDetector::setFOV(bool state){
    if (state) {
        FOV.set(ofRectangle(0,0,ofGetWidth(), ofGetHeight()));
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
    output_frame.draw(0, 0);
    if (FOV_origin) {
        ofSetColor(ofColor(255, 0, 0, 150));
        ofNoFill();
        //ofDrawRectangle(FOV.getX(), FOV.getY(), abs(FOV.getX() - x), abs(FOV.getY() - y));
        ofDrawRectangle(FOV.getX(), FOV.getY(), x - FOV.getX() ,  y - FOV.getY() );
    }
    ofPopStyle();
}

//--------------------------------------------------------------
void ShapeDetector::setPalette(vector<ofColor> p) {
    palette = p;
}

//--------------------------------------------------------------
void ShapeDetector::updateContourMode(bool& enable_color_track) {
    std::cout << "updating contour color tracking to : " << enable_color_track << std::endl;
    finder.setUseTargetColor(enable_color_track);
}

//--------------------------------------------------------------
void ShapeDetector::setFinderColour( ofColor c) {
    target_colour = c;
}

//--------------------------------------------------------------
vector<ofPolyline> ShapeDetector::getPolys() {
    return finder.getPolylines();
}

