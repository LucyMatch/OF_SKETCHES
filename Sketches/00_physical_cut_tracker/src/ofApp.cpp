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
    graphics.setPalette(palettes.getPaletteEdited(curr_palette, bg_c_index));

    video_input.setOutputDims(glm::vec2(ofGetWidth(), ofGetHeight()));
    video_input.setDims(glm::vec2(1920/2,1080/2));
    video_input.setup();

    initGui();

    //fuck media  man lets do it like this
    //int feed_amount = 1;
    //for (int i = 0; i < feed_amount; i++) {
    //    VideoHandler* vh = new VideoHandler(glm::vec2(1920, 1080));
    //    vh->setOutputDims(glm::vec2(ofGetWidth(), ofGetHeight()));
    //    vh->setMode(VideoHandler::VIDEO_LOCAL, "videos", false);
    //    vh->setFeed(0, false);
    //    vh->setup("videos", VideoHandler::VIDEO_LOCAL);

    //    videos.push_back(vh);
    //}

    video.setOutputDims(glm::vec2(ofGetWidth(), ofGetHeight()));
    video.setMode(VideoHandler::VIDEO_LOCAL, "videos", false);
    video.setFeed(0, false);
    video.setup("videos", VideoHandler::VIDEO_LOCAL);


    //@TODO: review tracker settings 
    //+ add to a gui
    //may want a manager - or goes in shape man?
    // wait for half a frame before forgetting something
    tracker.setPersistence(0);
    // an object can move up to 50 pixels per frame
    tracker.setMaximumDistance(100);

    
}

//--------------------------------------------------------------
void ofApp::update(){
    framerate();

    //for (auto& v : videos) {
    //    v->update();
    //}

    video.update();

    //update video input
    video_input.update();

    if (video_input.isFrameNew()) {
        if (video_input.isFrameAllocated()) {

            shape_detector.update(*video_input.getFrameTex());

            //update tracker
            tracker.track( shape_detector.finder.getBoundingRects() );
        
            //for colours..
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

                if (i < polys.size())
                    followers[i].update(polys[i]);

            }

        }
    }

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofPushStyle();
        ofSetColor(bg_c);
        ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    ofPopStyle();


    //ofPushMatrix();
    //ofPushStyle();
    //auto temp = shape_detector.getCurrentDims();
    //ofTranslate((ofGetWidth() / 2) - (temp.x / 2), (ofGetHeight() / 2) - (temp.y / 2));

    if (enable_bg_video)shape_detector.drawInput();
    if (enable_shape_data)shape_detector.drawData();

    //ofPopStyle();
    //ofPopMatrix();

    
    //now for our masking 
        //if (videos[0]->isFrameAllocated()) {
        if (video.isFrameAllocated()) {

            vector<CutFollower> followers = tracker.getFollowers();
            for (int i = 0; i < followers.size(); i++) {

                if (!followers[i].getDead()) {

                    //trying to debug
                    bool no_size = false;


                    auto pos = followers[i].getPosition();
                    auto size = followers[i].getSize();

                    if (enable_texture_masking) {
                        ofFbo temp_fbo;
                        temp_fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
                        temp_fbo.getTexture().setSwizzle(GL_TEXTURE_SWIZZLE_A, GL_RED);

                        temp_fbo.begin();
                        ofSetColor(0, 0, 0, 255);
                        ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
                        graphics.draw(followers[i].getShape(), ofColor(255, 255, 255, 255));
                        temp_fbo.end();

                        pos.x -= size.x / 2;
                        pos.y -= size.y / 2;

                        //*videos[0]->getFrameTex();
                        ofTexture orig_copy = *video.getFrameTex();
                        orig_copy.setAlphaMask(temp_fbo.getTexture());

                        if (size.x < 2 || size.y < 2) {
                            std::cout << size.x << " x " << size.y << std::endl;
                            no_size = true;
                        }
                        else {
                            save_fbo.clear();
                            save_fbo.allocate(size.x, size.y, GL_RGBA);
                            save_fbo.begin();
                            orig_copy.drawSubsection(0, 0, size.x, size.y, pos.x, pos.y); //position mostly works
                            save_fbo.end();
                        }

                        //debugging
                        //ofPushStyle();
                        ////temp_fbo.getTexture().draw(i * 200, 0, 200, 200);
                        //video.getFrameTex()->draw(i * 200, 0, 100, 100);
                        //orig_copy.draw((i * 200) + 100, 0, 100, 100);

                        //ofNoFill();
                        //ofSetColor(255,0,0);
                        //ofDrawRectangle(i * 200, 0, 200, 200);
                        //ofPopStyle();

                    }


                    //ofPushMatrix();
                    //ofPushStyle();
                    //auto temp = shape_detector.getCurrentDims();
                    //ofTranslate((ofGetWidth() / 2) - (temp.x / 2), (ofGetHeight() / 2) - (temp.y / 2));

                    if (enable_poly_graphics) {
                        ofColor c = no_size ? ofColor(255, 0, 0, 255) : followers[i].getColor();
                        graphics.draw(followers[i].getShape(), c);
                    }
                    if (enable_texture_masking) {
                        if (!no_size)save_fbo.draw(pos);
                        //orig_copy.draw(0, 0);
                        //ofSetColor(255, 0, 0);
                        //ofNoFill();
                        //ofDrawRectangle(pos.x, pos.y, size.x, size.y);
                    }

                    //ofPopStyle();
                    //ofPopMatrix();
                }
        }
     }

    if (enable_palette_preview) drawPalette();
    if (enable_debug) drawDebug();
    if (enable_info) drawInfo();
    if (shape_detector.set_canvas) shape_detector.drawLiveFOVConfig(mouseX, mouseY);
}

//--------------------------------------------------------------
void ofApp::drawDebug() {

    //videos[0]->getFrameTex()->draw(0, 0);
    video.getFrameTex()->draw(0, 0);

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
    gui.add(enable_shape_data.set("enable shape data", true));
    gui.add(enable_palette_preview.set("enable palette preview", false));
    gui.add(enable_poly_graphics.set("enable poly graphics", false));
    gui.add(enable_texture_masking.set("enable cut textures", false));
    gui.add(curr_palette.set("current palette", 0, 0, palettes.getNPalettes() - 1));

    gui.add(video_input.gui);
    gui.add(shape_detector.gui);
    gui.add(graphics.gui);
    gui.add(tex_utils.gui);
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
        graphics.setPalette(palettes.getPaletteEdited(curr_palette, bg_c_index));
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

