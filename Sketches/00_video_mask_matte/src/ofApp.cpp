#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetVerticalSync(false);
    ofEnableSmoothing();
    ofEnableAlphaBlending();

    palettes.load();

    video.setOutputDims(glm::vec2(ofGetWidth(), ofGetHeight()));
    video.setDims(glm::vec2(700,394));
    video.setup("videos", VideoHandler::VIDEO_LOCAL);
    video.setVolume(5);

    matte.setOutputDims(glm::vec2(ofGetWidth(), ofGetHeight()));
    matte.setDims(glm::vec2(700, 394));
    matte.setup("mattes", VideoHandler::VIDEO_LOCAL);

    composite.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);

    //initGui();
}

//--------------------------------------------------------------
void ofApp::update(){
    framerate();

    video.update();
    matte.update();

    if (video.isFrameNew()) {
        //update matte frame to match the original frame so they sync
        matte.local_cam.setFrame(video.local_cam.getCurrentFrame());

        //may need to draw matte to an fbo first and do some stuff 
        ofFbo temp_fbo;
        temp_fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
        temp_fbo.getTexture().setSwizzle(GL_TEXTURE_SWIZZLE_A, GL_RED);

        ofPushStyle();
        temp_fbo.begin();
            matte.draw( );
        temp_fbo.end();
        ofPopStyle();

        //lets mask this
        ofTexture original_texture = *video.getFrameTex();
        ofTexture mask_texture = temp_fbo.getTexture();
        original_texture.setAlphaMask(mask_texture);

        //masked_tex = original_texture;

        ofPushStyle();
            composite.begin();
            //add toggel for this for trails
            //ofSetColor(bg_c);
            //ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

            original_texture.draw(0,0);
            composite.end();
        ofPopStyle();
    }
        
}

//--------------------------------------------------------------
void ofApp::draw(){

    ofSetColor(bg_c);
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

    //video.draw();
    ofPushMatrix();
    ofScale(1.5, 1.5);
    composite.draw(0,0);
    //masked_tex.draw(0,0);
    ofPopMatrix();

    if (enable_palette_preview)drawPalette();
    if (enable_debug) drawDebug();
    if (enable_info) drawInfo();
}

//--------------------------------------------------------------
void ofApp::drawDebug() {
    ofPushMatrix();
    ofScale(0.7, 0.7);
        video.getFrameTex()->draw(0, 0);
    ofPopMatrix();    
    ofPushMatrix();
    ofTranslate(ofGetWidth() /2 , 0);
    ofScale(0.5, 0.5);
        matte.getFrameTex()->draw(0, 0);
    ofPopMatrix();
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
void ofApp::initGui() {
    gui.setup("P R I M A R Y");
    gui.add(bg_c.set("background", ofColor(247, 237, 226, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));
    gui.add(curr_palette.set("current palette", 0, 0, palettes.getNPalettes() - 1));
    gui.add(enable_debug.set("enable debug", false));
    gui.add(enable_info.set("enable info", false));
    gui.add(enable_palette_preview.set("enable palette preview", false));

    gui.add(video.gui);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
        switch (key) {
        case ' ':
            composite.clear();
            composite.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
            ofPushStyle();
            composite.begin();
            ofSetColor(bg_c);
            ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
            composite.end();
            ofPopStyle();
            break;
        case 'c':
            //reset background colour to palette colour
            bg_c = *palettes.getColour(curr_palette, bg_c_index);
            break;
        case 'v':
            //nxt color in palette is bg colour
            bg_c_index = ++bg_c_index % palettes.getNColours(curr_palette);
            bg_c = *palettes.getColour(curr_palette, bg_c_index);
            break;
        case '.':
            video.nxtFeed();
            matte.nxtFeed();
            break;
        case ',':
            video.prevFeed();
            matte.prevFeed();
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
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

