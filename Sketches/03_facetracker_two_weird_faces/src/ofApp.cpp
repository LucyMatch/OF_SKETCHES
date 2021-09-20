#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetVerticalSync(false);
    ofEnableSmoothing();
    ofEnableAlphaBlending();

    main_draw.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);

    //init vid manager
    video.setDims(glm::vec2(1280, 720));
    video.setup();

    ////init gui
    initGui();
    
    // Setup tracker
    tracker.setup();

}

//--------------------------------------------------------------
void ofApp::update(){
    
    if (enable_interval_export) {
        time = ofGetElapsedTimeMillis();
        if (time - time_interval > time_elapsed) 
            intervalExport();
    }

    framerate();

    //update video for new frame
    video.update();
    
    // Update tracker when there are new frames
    // if yes also update cut manager

    if (video.cam.isFrameNew()) {

        tracker.update(video.cam); 
        auto t = tracker.getInstances();
        cut_man.update(t);

    }

    

}

//--------------------------------------------------------------
void ofApp::draw(){


    // draw to main fbo

    ofPushStyle();
    main_draw.begin();

    ofSetColor( cuts_c );
    cut_man.draw( video.getFrameTex() );


    main_draw.end();
    ofPopStyle();

    ofSetColor( bg_c );
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

    //draw "uncut" texture 
    if (enable_orig)video.draw();

    //@TODO: add blending of FBO here...
    //       we want to be able to clear it
    //       we want to "draw trails"
    //main_draw.draw(0, 0);

    ofSetColor(cuts_c);
    cut_man.draw(video.getFrameTex());
    

    if (enable_debug)
        drawDebug();

    if (show_info)
        drawInfo();
    
}

//--------------------------------------------------------------
void ofApp::drawDebug() {

    // Draw tracker landmarks
    tracker.drawDebug();

    // Draw estimated 3d pose
    tracker.drawDebugPose();

    cut_man.drawDebug();

    cut_man.draw();

    cut_man.draw(video.getFrameTex());

    
}

//--------------------------------------------------------------
void ofApp::intervalExport() {
    //reset time tracker
    time_elapsed = ofGetElapsedTimeMillis();
    cut_man.exportCuts(video.getFrameTex());
}

//--------------------------------------------------------------
void ofApp::framerate() {
    std::stringstream strm;
    strm << "fps: " << ofGetFrameRate();
    ofSetWindowTitle(strm.str());
}

//--------------------------------------------------------------
void ofApp::drawInfo() {
    ofPushMatrix();
    ofPushStyle();

    ofEnableAlphaBlending();

    // draw the info box
    ofSetColor(0,80);
    ofDrawRectangle(5,5, ofGetWidth() /2, 200);

    std::stringstream ss;

    //@TODO: UPDATE THIS WITH RELEVANT INFO
    // 
    // ofToString formatting available in 0072+
    ss << "          Video Feed: " << video.getVideoTitle() << std::endl;
    ss << "          Export Interval: " << time_interval / 1000 << " /s" << std::endl;
    string enabled = (enable_interval_export) ? "yes" : "no";
    ss << "          Export Interval Active: " << enabled << std::endl;
    ss << "---------------------------------- " <<  std::endl;
    ss << " Key Controls " << std::endl;
    ss << " Next Video              :  '.' " << std::endl;
    ss << " Prev Video              :  ',' " << std::endl;
    ss << " Single Export           :  'x' " << std::endl;
    ss << " Toggle Interval Export  :  'z' " << std::endl;
    ss << " Toggle This Info        :  'b' " << std::endl;
    ss << " Spawn                   :  'p' " << std::endl;
    ss << " Random Spawn            :  'o' " << std::endl;
    ss << " Clear Particles         :  '-' " << std::endl;
    ss << " Clear Fbos              :  '=' " << std::endl;


    ofSetColor(255);
    ofDrawBitmapString(ss.str(), 10, 10 + 12);

    ofDisableAlphaBlending();

    ofPopStyle();
    ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::initGui() {

    gui.setup("P R I M A R Y");
    //48, 255, 297   //255, 70, 86
    gui.add(bg_c.set("background", ofColor(46, 184, 139, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));
    gui.add(cuts_c.set("cuts colour", ofColor(255, 255, 255, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));
    gui.add(enable_debug.set("enable debug", false));
    gui.add(enable_orig.set("enable orig", true));
    gui.add(time_interval.set("export interval", 1000, 100, 50000));

    gui.add(video.gui);
    gui.add(cut_man.gui);

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
    case 'b':
        show_info = !show_info;
        break;    
    case 'z':
        enable_interval_export = !enable_interval_export;
        break;
    case '1':
        //@TODO: once gui tidied up - update this to actually save all
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