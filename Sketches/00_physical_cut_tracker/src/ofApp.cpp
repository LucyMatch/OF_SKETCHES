#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetVerticalSync(false);
    ofEnableSmoothing();
    ofEnableAlphaBlending();

    palettes.load();
    bg_c = *palettes.getColour(curr_palette, bg_c_index);
    shape_detector.setPalette(palettes.getPaletteEdited(curr_palette, bg_c_index));

    video_input.setOutputDims(glm::vec2(ofGetWidth(), ofGetHeight()));
    video_input.setDims(glm::vec2(1920/2,1080/2));
    video_input.setup();

    //@TODO: review tracker settings 
    //+ add to a gui
    //may want a manager - or goes in shape man?

    // wait for half a frame before forgetting something
    tracker.setPersistence(15);
    // an object can move up to 50 pixels per frame
    tracker.setMaximumDistance(100);

    initGui();
}

//--------------------------------------------------------------
void ofApp::update(){
    framerate();

    video_input.update();

    if (video_input.isFrameNew())
        if (video_input.getFrameTex()) {

            shape_detector.update(*video_input.getFrameTex());

            //update tracker
            tracker.track( shape_detector.finder.getBoundingRects());



            ////untested the following but is the general flow i think

            ////for colours..
            int colour_option = 0;
            auto palette = palettes.getPaletteEdited( curr_palette, bg_c_index );

            ////for polys
            auto polys = shape_detector.finder.getPolylines();

            vector<CutFollower>& followers = tracker.getFollowers();

            for (int i = 0; i < followers.size(); i++) {
                //auto label = tracker.getLabelFromIndex(i);
                if (!followers[i].isColorSet()) {
                    followers[i].setColor(palette[colour_option]);
                    colour_option = ++colour_option % (int)palette.size();
                }

                //update it's poly
                // //i dont think these line up???
                // //i keep getting out of bounds :(((( 
                if (i < polys.size())
                    followers[i].update(polys[i]);
            }

        }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofPushStyle();
        ofSetColor(bg_c);
        ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    ofPopStyle();

    ofPushMatrix();
    ofPushStyle();
        auto temp = shape_detector.getCurrentDims();
        ofTranslate((ofGetWidth() / 2) - (temp.x / 2), (ofGetHeight() / 2) - (temp.y / 2));

        if (enable_bg_video)shape_detector.drawInput();
        if (enable_shape_data)shape_detector.drawData();

        vector<CutFollower>& followers = tracker.getFollowers();
        for (int i = 0; i < followers.size(); i++) {
            followers[i].draw();
        }

    ofPopStyle();
    ofPopMatrix();


    if (enable_palette_preview) drawPalette();
    if (enable_debug) drawDebug();
    if (enable_info) drawInfo();
    if (shape_detector.set_canvas) shape_detector.drawLiveFOVConfig(mouseX, mouseY);
}

//--------------------------------------------------------------
void ofApp::drawDebug() {
    shape_detector.drawDebug();
    vector<CutFollower>& followers = tracker.getFollowers();
    for (int i = 0; i < followers.size(); i++) {
        followers[i].drawDebug();
    }
}

//--------------------------------------------------------------
void ofApp::initGui() {
    gui.setup("P R I M A R Y");
    gui.add(bg_c.set("background", ofColor(247, 237, 226, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));
    gui.add(enable_debug.set("enable debug", false));
    gui.add(enable_info.set("enable info", false));
    gui.add(enable_bg_video.set("enable bg vid", true));
    gui.add(enable_shape_data.set("enable shape data", false));
    gui.add(enable_palette_preview.set("enable palette preview", false));
    gui.add(curr_palette.set("current palette", 0, 0, palettes.getNPalettes() - 1));

    gui.add(video_input.gui);
    gui.add(shape_detector.gui);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    switch (key) {
    case '[':
        video_input.prevFeed();
        break;
    case ']':
        video_input.nxtFeed();
        break;
    case '/':
        shape_detector.setFOV(true);
        break;
    case ' ':
        shape_detector.learn_background = true;
        break;
    case 'c':
        //reset background colour to palette colour
        bg_c = *palettes.getColour(curr_palette, bg_c_index);
        break;
    case 'v':
        //nxt color in palette is bg colour
        bg_c_index = ++bg_c_index % palettes.getNColours(curr_palette);
        bg_c = *palettes.getColour(curr_palette, bg_c_index);
        shape_detector.setPalette(palettes.getPaletteEdited(curr_palette, bg_c_index));
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
void ofApp::drawPalette() {
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
    ss << " RESET BG COLOUR          :  'c' " << std::endl;
    ss << " NXT BG COLOUR            :  'v' " << std::endl;
    ss << " PREV VID INPUT           :  '[' " << std::endl;
    ss << " NXT VID INPUT            :  ']' " << std::endl;
    ss << " LEARN BG                 :  ' ' " << std::endl;
    ss << " ENABLE LIVE FOV EDITING  :  '/' " << std::endl;
    ss << "  --> USE MOUSE TO SET BOUNDARIES WHEN ENABLED " << std::endl;
    ss << "    " << std::endl;
    ss << "---------------------------------- " << std::endl;
    ss << " CLICK MOUSE WHILE IN COLOUR DETECT MODE TO SET DETECTION COLOUR " << std::endl;



    ofSetColor(255);
    ofDrawBitmapString(ss.str(), 10, 10 + 12);

    ofDisableAlphaBlending();

    ofPopStyle();
    ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    if (shape_detector.set_canvas)
        shape_detector.setFOV(x, y);
    else {
        ofImage tmp;
        tmp.grabScreen(x, y, 1, 1);
        shape_detector.setFinderColour(tmp.getColor(0, 0));
    }
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    video_input.setOutputDims(glm::vec2(ofGetWidth(), ofGetHeight()));
    shape_detector.setFullscreenDims(glm::vec2(video_input.getODims().x, video_input.getODims().y));
}

