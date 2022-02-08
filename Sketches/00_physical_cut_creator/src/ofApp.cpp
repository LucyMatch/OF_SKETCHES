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

    shape.setOutputDims(glm::vec2(video.getODims().x, video.getODims().y));

    initGui();

}

//--------------------------------------------------------------
void ofApp::update(){
    framerate();

    video.update();

    if (video.isFrameNew())
        shape.update(video.getFramePixels());
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(bg_c);
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

    ofSetColor(255,255,255,255);

    if(enable_bg_video)
        video.getFrameTex()->draw(0,0, video.getODims().x, video.getODims().y);

    if(enable_shape_data)
        shape.drawData();

    shape.draw();

    if (enable_debug) drawDebug();
    if (enable_info) drawInfo();
    if (shape.set_canvas) shape.drawLiveFOVConfig(mouseX, mouseY);
}

//--------------------------------------------------------------
void ofApp::drawDebug() {
    shape.drawDebug();
}

//--------------------------------------------------------------
void ofApp::initGui() {

    gui.setup("P R I M A R Y");
    gui.add(bg_c.set("background", ofColor(247, 237, 226, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));
    gui.add(enable_debug.set("enable debug", false));
    gui.add(enable_info.set("enable info", false));
    gui.add(enable_bg_video.set("enable bg vid", false));
    gui.add(enable_shape_data.set("enable shape data", false));

    gui.add(video.gui);
    gui.add(shape.gui);
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
    case '/':
        shape.setFOV(true);
        break;
    case ' ':
       shape.learn_background = true;
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
void ofApp::mousePressed(int x, int y, int button){
    std::cout << "in mouse pressed" << std::endl;
    if (shape.set_canvas) shape.setFOV(x, y);
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    video.setOutputDims(glm::vec2(ofGetWidth(), ofGetHeight()));
    shape.setOutputDims(glm::vec2(video.getODims().x, video.getODims().y));
}

