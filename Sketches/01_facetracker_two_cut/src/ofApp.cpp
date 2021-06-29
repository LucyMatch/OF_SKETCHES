#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetVerticalSync(false);
    ofEnableSmoothing();
    ofEnableAlphaBlending();

    //init vid manager
    video.setDims(glm::vec2(1280, 720));
    video.setup(); //also here we would define - ip / webcam / local

    //init cut manager
    //cut_man.setup();

    ////init gui
    initGui();
    
    // Setup tracker
    tracker.setup();
}

//--------------------------------------------------------------
void ofApp::update(){
    framerate();

    //update video for new frame
    video.update();
    
    // Update tracker when there are new frames
    if(video.cam.isFrameNew()){
        tracker.update(video.cam);
    }

    //testing
////
/*
    enum Feature {
    LEFT_EYE_TOP, RIGHT_EYE_TOP,
    LEFT_EYEBROW, RIGHT_EYEBROW,
    LEFT_EYE, RIGHT_EYE,
    LEFT_JAW, RIGHT_JAW, JAW,
    OUTER_MOUTH, INNER_MOUTH,
    NOSE_BRIDGE, NOSE_BASE,
    FACE_OUTLINE, ALL_FEATURES
};
*/
    auto t = tracker.getInstances();
    for (auto& _t : t) {
        auto l = _t.getLandmarks();
        //ofSetColor(255, 0, 0);
        //poly.draw();
        //just do one polycut for now - no manager...
        left_eye.update(l.getImageFeature(l.LEFT_EYE));
        right_eye.update(l.getImageFeature(l.RIGHT_EYE));
        mouth.update(l.getImageFeature(l.OUTER_MOUTH));
    }



}

//--------------------------------------------------------------
void ofApp::draw(){

    ofSetColor(bg_c);
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

    //draw "uncut" texture 
    if (enable_orig)
        video.draw();


    left_eye.draw();
    right_eye.draw();
    mouth.draw();


    if (enable_debug)
        drawDebug();
    
}

//--------------------------------------------------------------
void ofApp::drawDebug() {

    // Draw tracker landmarks
    tracker.drawDebug();

    // Draw estimated 3d pose
    tracker.drawDebugPose();
    
}

//--------------------------------------------------------------
void ofApp::framerate() {
    std::stringstream strm;
    strm << "fps: " << ofGetFrameRate();
    ofSetWindowTitle(strm.str());
}

//--------------------------------------------------------------
void ofApp::initGui() {

    gui.setup("P R I M A R Y");

    gui.add(bg_c.set("background", ofColor(255, 228, 246, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));
    gui.add(enable_debug.set("enable debug", false));
    gui.add(enable_orig.set("enable orig", true));

    gui.add(video.gui);
    gui.add(left_eye.gui);
    gui.add(right_eye.gui);
    gui.add(mouth.gui);

}

//--------------------------------------------------------------
void ofApp::drawGui(ofEventArgs& args) {
    gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

    switch (key) {
    case '.':
        video.nxtFeed();
        break;
    case ',':
        video.prevFeed();
        break;
    case '1':
        gui.saveToFile("1_gui.xml");
        break;
    case '2':
        gui.saveToFile("2_gui.xml");
        break;
    case '3':
        gui.saveToFile("3_gui.xml");
        break;
    case '4':
        gui.saveToFile("4_gui.xml");
        break;
    case 'q':
        gui.loadFromFile("1_gui.xml");
        break;
    case 'w':
        gui.loadFromFile("2_gui.xml");
        break;
    case 'e':
        gui.loadFromFile("3_gui.xml");
        break;
    case 'r':
        gui.loadFromFile("4_gui.xml");
        break;
    default:
        break;
    }

    if (key == OF_KEY_RETURN) {
        //screen shot
        glReadBuffer(GL_FRONT);
        ofSaveScreen("screenshot_" + ofToString(ofGetMonth()) + "_" + ofToString(ofGetDay()) + "_" + ofToString(ofGetYear()) + "_" + ofToString(ofGetHours()) + "_" + ofToString(ofGetMinutes()) + ofToString(ofGetSeconds()) + ".png");
    }

}


//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}