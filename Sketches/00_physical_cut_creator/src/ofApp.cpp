#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetFrameRate(60);
    ofSetVerticalSync(false);
    ofEnableSmoothing();
    ofEnableAlphaBlending();

    canvas_dims = glm::vec2(1920, 1080);

    video.setDims(canvas_dims);
    video.setup("ip_cam/cams.json", VideoHandler::VIDEO_IP);
    video.setOutputDims(glm::vec2(ofGetWidth(), ofGetHeight()));

    //colorImg.allocate(video.getODims().x, video.getODims().y);
    //grayImage.allocate(video.getODims().x, video.getODims().y);
    //grayBg.allocate(video.getODims().x, video.getODims().y);
    //grayDiff.allocate(video.getODims().x, video.getODims().y);

    initGui();

}

//--------------------------------------------------------------
void ofApp::update(){
    framerate();

    video.update();

    if (video.isFrameNew()) {

        colorImg.setFromPixels(video.getOriginalPixels());

        grayImage = colorImg;

        if (learn_background) {
            grayBg = grayImage;
            learn_background = false;
        }

        grayDiff.absDiff(grayBg, grayImage);
        grayDiff.threshold(diff_thresh);

        //quick resize
        ofxCvGrayscaleImage _temp = grayDiff;
        _temp.resize(video.getODims().x, video.getODims().y);

        //find contours
        contourFinder.findContours(_temp, cmin, cmax, considered, choles, capprox);

    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(bg_c);
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

    ofSetColor(255,255,255,255);

    video.getFrameTex()->draw(0,0, video.getODims().x, video.getODims().y);
    contourFinder.draw();

    for (auto b : contourFinder.blobs) {


        ofSetColor(ofColor(255,0,0));
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

    if (enable_debug) drawDebug();
    if (enable_info) drawInfo();
}

//--------------------------------------------------------------
void ofApp::drawDebug() {
    int cols = 2, width = 0, height = 0, x = 0, y = 0;
    width = ofGetWidth() / cols;
    height = width * (video.getDims().y / video.getDims().x);

    video.getFrameImg().draw(x, y, width, height);
    colorImg.draw(x + width, y, width, height);
    grayImage.draw(x, y + height, width, height);
    grayBg.draw(x + width, y + height, width, height);
    grayDiff.draw(x , y + (height*2), width, height);
    contourFinder.draw(x + width  , y + (height*2), width, height);

}

//--------------------------------------------------------------
void ofApp::initGui() {

    gui.setup("P R I M A R Y");
    //   //255, 70, 86
    gui.add(bg_c.set("background", ofColor(48, 255, 297, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));
    gui.add(enable_debug.set("enable debug", false));
    gui.add(enable_info.set("enable info", false));
    gui.add(diff_thresh.set("diff threshold", 80, 0, 500));

    gui.add(cmin.set("contour min", 20, 0, 1000));
    gui.add(cmax.set("contour max", 500, 0, 1920*1080));
    gui.add(considered.set("contour considered", 10, 0, 100));
    gui.add(choles.set("contour holes", true));
    gui.add(capprox.set("contour approximation", false));

    gui.add(video.gui);

}

//--------------------------------------------------------------
void ofApp::drawGui(ofEventArgs& args) {
    gui.draw();
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
    ofSetColor(0, 80);
    ofDrawRectangle(5, 5, ofGetWidth() / 2, 200);

    std::stringstream ss;

    ss << "---------------------------------- " << std::endl;
    ss << " Key Controls " << std::endl;
    ss << " GUI SAVE                 :  '7 , 8, 9, 0' " << std::endl;
    ss << " GUI LOAD                 :  'u , i, o, p' " << std::endl;


    ofSetColor(255);
    ofDrawBitmapString(ss.str(), 10, 10 + 12);

    ofDisableAlphaBlending();

    ofPopStyle();
    ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    switch (key) {
    case '[':
        video.prevFeed();
        break;
    case ']':
        video.nxtFeed();
        break;    
    case ' ':
        learn_background = true;
        break;
    case '0':
        gui.saveToFile("1_gui.xml");
        break;
    case '9':
        gui.saveToFile("2_gui.xml");
        break;
    case '8':
        gui.saveToFile("3_gui.xml");
        break;
    case '7':
        gui.saveToFile("4_gui.xml");
        break;
    case 'p':
        gui.loadFromFile("1_gui.xml");
        break;
    case 'o':
        gui.loadFromFile("2_gui.xml");
        break;
    case 'i':
        gui.loadFromFile("3_gui.xml");
        break;
    case 'u':
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
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

