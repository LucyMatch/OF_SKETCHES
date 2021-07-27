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
    if (video.cam.isFrameNew()) {
        tracker.update(video.cam); 
        auto t = tracker.getInstances();
        cut_man.update(t);
    }

}

//--------------------------------------------------------------
void ofApp::draw(){

    ofSetColor(bg_c);
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

    //draw "uncut" texture 
    if (enable_orig)
        video.draw();

    
    cut_man.draw();

    cut_man.draw(video.getFrameTex());


    ofEnableAlphaBlending();

    if (enable_debug)
        drawDebug();
    
}

//--------------------------------------------------------------
void ofApp::drawDebug() {

    // Draw tracker landmarks
    tracker.drawDebug();

    // Draw estimated 3d pose
    tracker.drawDebugPose();

    cut_man.drawDebug();

    cut_man.draw();
    
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
    gui.add(blend_mode.set("blend mode", 0, 0, 5));

    gui.add(video.gui);
    gui.add(cut_man.gui);

    //@TODO:
    //if we get new "faces" - check with size of instances
    //we need to remove cut_man and re add

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
    case 'x':
        cut_man.exportCuts(video.getFrameTex());
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