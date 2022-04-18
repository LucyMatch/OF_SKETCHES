#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetVerticalSync(false);
    ofEnableSmoothing();
    ofEnableAlphaBlending();

    ofRectMode(CENTER);

    /*palettes.load();*/    //load in main.cpp - so we can load prior to initGui
    bg_c = *palettes.getColour(curr_palette, bg_c_index);

    main_canvas.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    foreground_canvas.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);

    video_input.setOutputDims(glm::vec2(ofGetWidth(), ofGetHeight()));
    //video_input.setDims(glm::vec2(1920 / 1.5, 1080 / 1.5));
    video_input.setDims(glm::vec2(1920, 1080));
    video_input.setup();
    //video_input.setMode(VideoHandler::VIDEO_LOCAL, "videos", false);
    //video_input.setFeed(0, false);
    //video_input.setup("videos", VideoHandler::VIDEO_LOCAL);

    // Setup tracker
    tracker.setup();

}

//--------------------------------------------------------------
void ofApp::update(){

    framerate();

    video_input.update();

    if (video_input.isFrameNew()) {
        //
        tracker.update(video_input.getFrameImg());
        auto t = tracker.getInstances();
        cut_man.update(t);
    }

}

//--------------------------------------------------------------
void ofApp::draw(){

        auto palette = palettes.getPaletteEdited(curr_palette, bg_c_index);
        int ci = 0;

        auto temp = video_input.getDims();

        ofPushStyle();
        foreground_canvas.begin();
                ofSetColor(255, 255, 255, foreground_alpha);
                ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

                ofSetColor(255, 255, 255, 255);
                

                if (enable_eye_grid) {

                        for (auto& f : cut_man.faces) {

                            for (int x = 0; x < eye_grid_res_w; x ++) {
                                    for (int y = 0; y < eye_grid_res_h; y++) {

                                        int index = (x % 2 != 0) ? 1 : 0;
                                        ofRectangle b = enable_cut_mod_grid ? f.eyes[index].getModifiedBounding() : f.eyes[index].getBounding();
                                        

                                        if (enable_sin_grid) {
                                            auto range = f.eyes[index].getPosRange();
                                            //left.translateX( ofMap(sin(x), -1, 1, -range, range) );
                                            b.translateX(ofMap(sin(ofGetElapsedTimef() + x), -1, 1, -range, range));
                                        }
                                        else if (enable_pos_grid) { 
                                            auto x_inc = b.getWidth() / eye_grid_res_w;
                                            auto y_inc = b.getHeight() / eye_grid_res_h;
                                            float translate_x = x_inc * x, translate_y = y_inc * y;
                                            b.translate(translate_x, translate_y);
                                        }

                                        //full size to resoltuion / grid size...
                                        if (enable_cell_grid) {
                                            auto c = b.getCenter();
                                            float new_x = c.x - ((ofGetWidth() / eye_grid_res_w)/2);
                                            float new_y = c.y - ((ofGetHeight() / eye_grid_res_h)/2);
                                            b.setX(new_x);
                                            b.setY(new_y);
                                        }

                                        auto pos = b.getTopLeft();
                                        int xPos = ofGetWidth() / eye_grid_res_w * x;
                                        int yPos = ofGetHeight() / eye_grid_res_h * y;

                                        ofSetColor(255, 255, 255, 255);
                                        video_input.getFrameTex()->drawSubsection(xPos, yPos, b.getWidth(), b.getHeight(), pos.x, pos.y);
                                    }
                                }

                        }

                }
                else {
                    //on the face
                    int variant_amount = 10;

                    for (auto& f : cut_man.faces) {

                        //    auto bounding = f.cut.getBounding();
                        //    auto pos = bounding.getTopLeft();
                        //just draw eyes for now
                        ofPushMatrix();
                        ofPushStyle();

                        ofTranslate((ofGetWidth() / 2) - (temp.x / 2), (ofGetHeight() / 2) - (temp.y / 2));

                        //draw bg variants
                        for (int i = 0; i < variant_amount; i++) {
                            for (auto& e : f.eyes) {
                                auto b = e.getModifiedBounding();
                                auto b_pos = b.getTopLeft();


                                ofSetColor(255, 255, 255, 255);

                                video_input.getFrameTex()->drawSubsection(b_pos.x, b_pos.y, b.getWidth(), b.getHeight(), b_pos.x, b_pos.y);

                                ofSetColor(palette[ci], 100);
                                //ofNoFill();
                                ofSetLineWidth(1);
                                ofDrawRectangle(b);

                                ci = ++ci % (int)palette.size();
                            } 
                        }

                        //then draw actual eyes
                        for (auto& e : f.eyes) {
                            auto b = e.getBounding();
                            auto b_pos = b.getTopLeft();

                            //ofSetColor(palette[ci]);
                            //ofNoFill();
                            //ofSetLineWidth(1);
                            //ofDrawRectangle(b);
                            ofSetColor(255, 255, 255, 255);
                            video_input.getFrameTex()->drawSubsection(b_pos.x, b_pos.y, b.getWidth(), b.getHeight(), b_pos.x, b_pos.y);
                            ci = ++ci % (int)palette.size();
                        }

                        ofPopStyle();
                        ofPopMatrix();

                    }
                }
        foreground_canvas.end();
        ofPopStyle();

        ofPushStyle();
        main_canvas.begin();
            ofEnableAlphaBlending();

            ofSetColor(bg_c);
            ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
            ofSetColor(255, 255, 255, 255);

            ofPushMatrix();
            ofTranslate((ofGetWidth() / 2) - (temp.x / 2), (ofGetHeight() / 2) - (temp.y / 2));
                
                if (enable_original_video)video_input.getFrameTex()->draw(0,0);
            ofPopMatrix();

            glEnable(GL_BLEND);
            glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);

            foreground_canvas.draw(0,0);
            

            ofDisableAlphaBlending();
        main_canvas.end();
        ofPopStyle();

        //render to screen
        ofPushMatrix();

            ofEnableAlphaBlending();

            ofSetColor(bg_c);
            ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
            ofSetColor(255, 255, 255, 255);

            auto new_x = (ofGetWidth() - (main_canvas.getWidth() * canvas_scale))/2;
            auto new_y = (ofGetHeight() - (main_canvas.getHeight() * canvas_scale))/2;
            ofTranslate(new_x, new_y);
            ofScale(canvas_scale, canvas_scale, 1);

            
            main_canvas.draw(0, 0);
        ofPopMatrix();


    if (enable_palette_preview) drawPalette();
    if (enable_debug) drawDebug();
    if (enable_info) drawInfo();

}

//--------------------------------------------------------------
void ofApp::drawDebug() {

    video_input.getFrameTex()->draw(0, 0);

    video_input.draw();

    // Draw tracker landmarks
    tracker.drawDebug();

    // Draw estimated 3d pose
    tracker.drawDebugPose();

    cut_man.drawDebug();

    cut_man.draw();

    //ofPushView(); 
    //ofPushStyle();
    //    for (auto& i : tracker.getInstances()) {
    //        auto matrix = i.getPoseMatrix();
    //        ofLoadMatrix(matrix);
    //        auto b = i.getBoundingBox();
    //        ofSetColor(255,0,0);
    //        ofDrawRectangle(b.position, b.width, b.height);
    //        auto quat = matrix.getRotate();
    //    }
    //ofPopStyle();
    //ofPopView();

}

//--------------------------------------------------------------
void ofApp::initGui() {
    gui.setup("P R I M A R Y");
    gui.add(bg_c.set("background", ofColor(247, 237, 226, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));
    gui.add(foreground_alpha.set("foreground_alpha", 255, 0, 255));
    gui.add(canvas_scale.set("canvas scale", 1.0, 0.1, 5));
    gui.add(enable_debug.set("enable debug", false));
    gui.add(enable_info.set("enable info", false));
    gui.add(enable_original_video.set("enable orig video", true));
    gui.add(enable_palette_preview.set("enable palette preview", false));
    gui.add(curr_palette.set("current palette", 0, 0, palettes.getNPalettes() - 1));
    gui.add(enable_eye_grid.set("enable_eye_grid", true));
    gui.add(eye_grid_res_w.set("grid resolution cols", 10, 1, 100));
    gui.add(eye_grid_res_h.set("grid resolution rows", 10, 1, 100));
    gui.add(enable_cut_mod_grid.set("enable mod cut", false));
    gui.add(enable_sin_grid.set("enable sin grid", true));
    gui.add(enable_cell_grid.set("enable cell grid", true));
    gui.add(enable_pos_grid.set("enable pos grid", true));

    gui.add(video_input.gui);
    gui.add(cut_man.gui);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
    case '[':
        video_input.prevFeed();
        break;
    case ']':
        video_input.nxtFeed();
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
    ofPushStyle();

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

    ofSetColor(255);
    ofDrawBitmapString(ss.str(), 10, 10 + 12);

    ofDisableAlphaBlending();

    ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    video_input.setOutputDims(glm::vec2(ofGetWidth(), ofGetHeight()));
    main_canvas.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    foreground_canvas.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
}
