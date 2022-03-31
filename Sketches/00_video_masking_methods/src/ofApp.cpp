#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetVerticalSync(false);    //works best if true for method 2
    ofEnableSmoothing();
    ofEnableAlphaBlending();

    method = GREEN_SCREEN;

    loadVideos();
    setVideos();

    composite.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);

}

//--------------------------------------------------------------
void ofApp::update(){
    framerate();

    ofFbo temp_fbo;
    ofTexture original_texture;

    video.update();
    matte.update();

    switch (method) {
        case MASK_PLAY:
            if (video.isFrameNew())
                matte.setFrame(video.getCurrentFrame());
            else break;
        case MASK_FRAME:
            //if MASK PLAY has valid frame is sails thru here
            //so we need a conditional for mask frame
            if (method == MASK_FRAME) {
                if (video.getCurrentFrame() == video.getTotalNumFrames()) {
                    setVideos();
                }
                else {
                    video.nextFrame();
                    matte.nextFrame();
                }
            }

        original_texture = video.getTexture();

        temp_fbo.allocate(video.getWidth(), video.getHeight(), GL_RGBA);
        temp_fbo.getTexture().setSwizzle(GL_TEXTURE_SWIZZLE_A, GL_RED);

        //draw our mask
        ofPushStyle();
            temp_fbo.begin();
            //matte.draw(0,0, video.getWidth(), video.getHeight());
            matte.draw(0,0);
            temp_fbo.end();
        ofPopStyle();

        //lets mask this
        mask = temp_fbo.getTexture();
        original_texture.setAlphaMask(mask);

        ofPushStyle();
            composite.begin();
            //add toggel for this for trails
            ofSetColor(bg_c, composite_alpha);
            ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
            ofSetColor(255);
            original_texture.draw(0,0);
            composite.end();
        ofPopStyle();

       break;
    case GREEN_SCREEN:

        ofPixels pix = video.getPixels();
        ofPixels green = pix.getChannel(0);

        //pix.setChannel(0, pix.getChannel(1));
        //pix.setChannel(1, green);

        pix.setColor(ofColor(255, 255, 255));


        ofTexture tmp_tex;
        tmp_tex.allocate(pix);
        mask.allocate(pix); // so we can see it

        original_texture.allocate(video.getWidth(), video.getHeight(), GL_RGBA);
        original_texture = video.getTexture();
        original_texture.setAlphaMask(mask);
        

        ofPushStyle();
        composite.begin();
            //add toggel for this for trails
            ofSetColor(bg_c, composite_alpha);
            ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
            //ofSetColor(255);
            original_texture.draw(0, 0);
        composite.end();
        ofPopStyle();

        break;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){

    ofPushStyle();
        ofSetColor(bg_c);
        ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    ofPopStyle();

    ofPushMatrix();
        //ofScale(1.5, 1.5);
        composite.draw(0,0);
    ofPopMatrix();

    if (enable_debug) drawDebug();
    if (enable_info) drawInfo();
}

//--------------------------------------------------------------
void ofApp::loadVideos() {

    ofDirectory dir;

    switch (method) {
    case MASK_FRAME :
        //sail right thru // same as MASK_PLAY

    case MASK_PLAY:
        dir.listDir("black_originals");
        dir.sort();
        video.load(dir.getPath(curr_src));
        dir.listDir("mattes");
        dir.sort();
        matte.load(dir.getPath(curr_src));
        break;
    case GREEN_SCREEN:
        dir.listDir("green_originals");
        dir.sort();
        video.load(dir.getPath(curr_src));
        break;

    }

}

//--------------------------------------------------------------
void ofApp::setVideos() {

    switch (method) {
    case MASK_PLAY:
        matte.setVolume(0);
        video.setVolume(5);
        video.play();
        //matte.play();
        matte.setPaused(true);
        video.firstFrame();
        matte.firstFrame();
       break;
    case MASK_FRAME:
        video.setPaused(true);
        video.firstFrame();
        matte.setPaused(true);
        matte.firstFrame();
       break;
    case GREEN_SCREEN:
        video.setVolume(5);
        video.play();
        break;
    }

}

//--------------------------------------------------------------
void ofApp::nextVideo() {

    ofDirectory dir;

    switch (method) {
    case MASK_FRAME:
    case MASK_PLAY:
        dir.listDir("black_originals");
        curr_src = ++curr_src % (int)dir.size();
        break;
    case GREEN_SCREEN:
        dir.listDir("green_originals");
        curr_src = ++curr_src % (int)dir.size();
        break;
    }

    closeVideos();
    loadVideos();
    setVideos();
}

//--------------------------------------------------------------
void ofApp::closeVideos() {
    video.close();
    matte.close();
}

//--------------------------------------------------------------
void ofApp::nextMethod() {

    closeVideos();

    int current = static_cast<int>(method);
    current = ++current % MASK_METHOD_ITEMS;
    method = static_cast<MASK_METHOD>(current);

    //reset the things also
    curr_src = 0;
    loadVideos();
    setVideos();

}

//--------------------------------------------------------------
void ofApp::drawDebug() {

    ofPushMatrix();
    ofPushStyle();
    ofScale(0.5, 0.5);
    ofSetColor(255,255,255,255);

        if (video.isLoaded())video.getTexture().draw(0,0);
        ofSetColor(0);
        ofDrawRectangle(5, 35, 50, 25);
        ofSetColor(255, 255, 255, 255);
        ofDrawBitmapString("origin video", 10, 50);

        if(matte.isLoaded()) matte.getTexture().draw(video.getWidth(), 0);
        ofSetColor(0);
        ofDrawRectangle(video.getWidth() + 1, 35, 50, 25);
        ofSetColor(255, 255, 255, 255);
        ofDrawBitmapString("matte video", video.getWidth() + 10, 50);

        mask.draw(0,video.getHeight());
        ofSetColor(0);
        ofDrawRectangle(5, video.getHeight() + 35, 50, 25);
        ofSetColor(255, 255, 255, 255);
        ofDrawBitmapString("mask texture", 10, video.getHeight() + 50);

        composite.draw(mask.getWidth(), video.getHeight());
        ofSetColor(0);
        ofDrawRectangle(mask.getWidth() + 5, video.getHeight() + 35, 50, 25);
        ofSetColor(255, 255, 255, 255);
        ofDrawBitmapString("composite", mask.getWidth() +  10, video.getHeight() + 50);

        string mode;
        switch (method) {
        case MASK_PLAY:    mode = " MASK_PLAY ";  break;
        case MASK_FRAME:   mode = " MASK_FRAME ";  break;
        case GREEN_SCREEN: mode = " GREEN_SCREEN ";break;
        }
        ofSetColor(0);
        ofDrawRectangle(15, ofGetHeight() - 115, 100, 25);
        ofSetColor(255, 255, 255, 255);
        ofDrawBitmapString(mode, 20, ofGetHeight() -100);
    ofPopStyle();
    ofPopMatrix();    
}

//--------------------------------------------------------------
void ofApp::initGui() {
    gui.setup("P R I M A R Y");
    gui.add(bg_c.set("background", ofColor(247, 237, 226, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));
    gui.add(composite_alpha.set("comp alpha", 255, 0, 255));
    gui.add(enable_debug.set("enable debug", true));
    gui.add(enable_info.set("enable info", false));
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
        switch (key) {
        case '.':
            //next video
            nextVideo();
            break;        
        case '/':
            //next vmask method
            nextMethod();
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
    ss << " NEXT VIDEO SOURCE        :  '.' " << std::endl;
    ss << " NEXT MASK METHOD         :  '/' " << std::endl;


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

