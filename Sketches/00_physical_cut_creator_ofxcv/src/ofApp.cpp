#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

    std::cout << "main setup" << std::endl;

    ofSetFrameRate(60);
    ofSetVerticalSync(false);
    ofEnableSmoothing();
    ofEnableAlphaBlending();

    palettes.load();
    setPalette();

    canvas_dims = glm::vec2(1920, 1080);
    
    video.setOutputDims(glm::vec2(ofGetWidth(), ofGetHeight()));
    video.setDims(canvas_dims);
    video.setup("ip_cam/cams.json", VideoHandler::VIDEO_IP);


    initGui();

}

//--------------------------------------------------------------
void ofApp::update() {
    framerate();

    video.update();

    if (video.isFrameNew())
        if(video.getFrameTex())shape.update(*video.getFrameTex());
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofSetColor(bg_c);
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

    ofSetColor(255, 255, 255, 255);

    ofPushMatrix();
    ofPushStyle();
    auto temp = shape.getCurrentDims();
    ofTranslate((ofGetWidth() / 2) - (temp.x / 2), (ofGetHeight() / 2) - (temp.y / 2));

    if (enable_bg_video)
        shape.drawInput();

    if (enable_shape_data)
        shape.drawData();

    if (enable_poly_graphics) {
        auto ps = shape.getPolys();
        graphics.counter = 0;
        for (auto p : ps)
            graphics.draw(p);
    }   

    ofPopStyle();
    ofPopMatrix();

    if (enable_palette_preview) {
        auto p = palettes.getPalette(curr_palette);
        auto size = ofGetWidth() / palettes.getNColours(curr_palette);
        int count = 0;
        ofPushStyle();
        for (auto c : *palettes.getPalette(curr_palette)) {
            ofSetColor(c);
            ofDrawRectangle(size * count++, 0, size, size);
        }
        ofPopStyle();
    }

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
    gui.add(curr_palette.set("current palette", 0, 0, palettes.getNPalettes() - 1));
    gui.add(enable_debug.set("enable debug", false));
    gui.add(enable_info.set("enable info", false));
    gui.add(enable_bg_video.set("enable bg vid", true));
    gui.add(enable_shape_data.set("enable shape data", false));
    gui.add(enable_poly_graphics.set("enable poly graphics", false));
    gui.add(enable_palette_preview.set("enable palette preview", false));

    gui.add(video.gui);
    gui.add(shape.gui);
    gui.add(graphics.gui);
}

//--------------------------------------------------------------
//alt the palette + send the colours where they are needed
//we dont want bg colour to be sent to children...
//---------------------------------------------------------------
void ofApp::setPalette() {
    vector<ofColor> temp;
    for (int i = 0; i < palettes.getNColours(curr_palette); i++) {
        if (i != bg_c_index)temp.push_back(*palettes.getColour(curr_palette, i));
    }
    shape.setPalette(temp);
    graphics.setPalette(temp);
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
    case 'c':
        //reset background colour to palette colour
        bg_c = *palettes.getColour(curr_palette, bg_c_index);
        break;
    case 'v':
        //nxt color in palette is bg colour
        bg_c_index = ++bg_c_index % palettes.getNColours(curr_palette);
        bg_c = *palettes.getColour(curr_palette, bg_c_index);
        setPalette();
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
void ofApp::mouseDragged(int x, int y, int button) {
}
//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
    std::cout << "in mouse pressed" << std::endl;
    if (shape.set_canvas) 
        shape.setFOV(x, y);
    else {
        ofImage tmp;
        tmp.grabScreen(x, y, 1, 1);
        shape.setFinderColour(tmp.getColor(0, 0));
    }
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
    video.setOutputDims(glm::vec2(ofGetWidth(), ofGetHeight()));
    shape.setFullscreenDims(glm::vec2(video.getODims().x, video.getODims().y));
}

