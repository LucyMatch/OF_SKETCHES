#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetVerticalSync(false);
    ofEnableSmoothing();
    ofEnableAlphaBlending();

    canvas_dims = glm::vec2(1920, 1080);

    main_draw.allocate(canvas_dims.x, canvas_dims.y, GL_RGBA);
    cuts_draw.allocate(canvas_dims.x, canvas_dims.y, GL_RGBA);
    p_draw.allocate(canvas_dims.x, canvas_dims.y, GL_RGBA);

    // init vid manager
    video.setDims(canvas_dims);

    video.setup();
    video.setOutputDims(glm::vec2(ofGetWidth(), ofGetHeight()));

    // init gui
    initGui();
    
    // Setup tracker
    tracker.setup();

    //media man - test - simple video backgrounds
    Feed video_feed;
    video_feed.path = "videos";
    video_feed.media_type = mediaTypes::VIDEO;
    bg_feed = media_man.createNewFeed(video_feed);

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

    if (video.isFrameNew()) {

        tracker.update(video.getFrameImg());
        auto t = tracker.getInstances();
        cut_man.update(t);

        //update the elements using cuts ie. particle manager / graphic manager
        updateCutElements();

    }

    //update media man - to update frames
    media_man.update();
}

//--------------------------------------------------------------
void ofApp::draw(){

    if (enable_cuts_draw) {
        //draw cuts / elemnts on seperate fbo so we can blend
        ofPushStyle();
        cuts_draw.begin();
            ofSetColor(255, 255, 255, cuts_draw_alpha);
            ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
            ofSetColor(cuts_color);
            cut_man.draw(video.getFrameTex());
        cuts_draw.end();
        ofPopStyle();
    }

    if (enable_p_draw) {
        //draw particles / elemnts on seperate fbo so we can blend
        ofPushStyle();
        p_draw.begin();
            ofSetColor(255, 255, 255, p_draw_alpha);
            ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
            for (auto& p : p_men) { p.draw(); }
        p_draw.end();
        ofPopStyle();
    }

    // draw to main fbo
    ofPushStyle();
        main_draw.begin();
            ofEnableAlphaBlending();

            ofSetColor(bg_c);
            ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

            //draw "uncut" texture 
            if (enable_orig) video.draw();

            //TESTING DRAWING MEDIA MAN AS BG
            //NOT WORKING
            if (enable_bg_feed && bg_feed != NULL) {
                ofSetColor(bg_feed_c);
                ofDrawRectangle(0, 0, 250, 250);
                media_man.getFrameTexture(bg_feed)->draw(0, 0);
            }

            if (enable_trails) {
                glEnable(GL_BLEND);
                glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
            }

            if (enable_p_draw) {
                ofSetColor(p_color);
                if (enable_trails) {
                    p_draw.draw(0, 0);
                }
                else {
                    for (auto& p : p_men) { p.draw(); }
                }
            }

            if (enable_cuts_draw) {
                ofSetColor(cuts_draw_color);
                if (enable_trails) {
                    cuts_draw.draw(0, 0);
                }
                else {
                    cut_man.draw(video.getFrameTex());
                }
            }

            ofDisableAlphaBlending();
        main_draw.end();
    ofPopStyle();

    //render to screen
    main_draw.draw(0, 0);

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

    for (auto& p : p_men)
        p.drawDebug();

    media_man.drawDebug();

}

//--------------------------------------------------------------
void ofApp::updateCutElements() {

    if (cut_man.faces.size() > 0) {
        //we know we have faces!
        //for each face we have a set of landmarks ( features ) 
        //we create an element manager per landmark
        for (int i = 0; i < cut_man.faces.size(); i++) {
            for (int j = 0; j < cut_man.faces[i].size(); j++) {
                //looping through landmarks of faces
                //to translate index into a 1d array to access to elements
                // --- i + ( j * size() )
                auto index = i + (j * cut_man.faces.size());
                
                //update pmen
                updatePmen ( index, i,  j );

                //update graphics ( todo )

            }
        }

        checkPmen();
        //checkGraphics();
    }

}

//--------------------------------------------------------------
void ofApp::updatePmen(int index, int i, int j) {

    if (index < p_men.size()) {
        //pman_exists!
        //update cut ( shape )
        p_men[index].update(cut_man.faces[i][j].cut);
        //update texture
        ofTexture tmp_tex;
        tmp_tex = cut_man.getCutTexture(cut_man.faces[i][j].cut, *(video.getFrameTex()));
        p_men[index].update(tmp_tex);
        //update 
        p_men[index].update();
        //update enabled - so we dont draw p if cut has been disabled
        p_men[index].enabled = cut_man.faces[i][j].enabled;
    }
    else {
        //we must create a new pman!
        CutParticleManager _cpm(cut_man.faces[i][j].cut);
        _cpm.gui.setName(ofToString(index));
        p_man_gui.add(_cpm.gui); //create a gui panel for new manager
        p_men.push_back(_cpm);
    }

}

//--------------------------------------------------------------
void ofApp::checkPmen() {
    if (cut_man.faces.size() > 0 && p_men.size() > (cut_man.faces.size() * cut_man.faces[0].size())) {
        auto amt = cut_man.faces.size() * cut_man.faces[0].size();          //amt we want
        p_men.erase(p_men.begin() + amt, p_men.begin() + p_men.size());    //erase unwanted
        updatePMenGui(amt);                                               //remove unwanted gui panels
    }
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
    gui.add(cuts_draw_alpha.set("cuts layer alpha", 255, 0, 255));
    gui.add(p_draw_alpha.set("p layer alpha", 255, 0, 255));
    gui.add(cuts_color.set("cuts colour", ofColor(255, 255, 255, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));
    gui.add(p_color.set("p colour", ofColor(255, 255, 255, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));
    gui.add(cuts_draw_color.set("cuts draw colour", ofColor(255, 255, 255, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));
    gui.add(enable_debug.set("enable debug", false));
    gui.add(enable_orig.set("enable orig", true));
    gui.add(enable_cuts_draw.set("enable cuts draw", false));
    gui.add(enable_p_draw.set("enable particle draw", false));
    gui.add(enable_trails.set("enable trails", true));
    gui.add(time_interval.set("export interval", 1000, 100, 50000));

    media_man_gui.clear();
    media_man_gui.setName("FEEDS");
    media_man_gui.add(enable_bg_feed.set("enable bg video feed", true));
    media_man_gui.add(bg_feed_c.set("background feed", ofColor(255, 0, 0, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));

    gui.add(media_man_gui);

    gui.add(video.gui);
    gui.add(cut_man.gui);

    particleGui.clear();
    particleGui.setName("P");
    particleGui.add(CutParticle::pcolor.set("color", ofColor(0, 0, 0, 100), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));
    particleGui.add(CutParticle::tcolor.set("trail color", ofColor(0, 0, 0, 100), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));

    particleGui.add(CutParticle::r.set("radius", 10, 0, 1000));
    particleGui.add(CutParticle::enable_true_size.set("true size", true));
    particleGui.add(CutParticle::enable_random_size.set("random size", false));
    particleGui.add(CutParticle::enable_continous_resizing.set("continous resize", false));
    particleGui.add(CutParticle::size_min.set("size min", 11, 10, 500));
    particleGui.add(CutParticle::size_max.set("size max", 100, 0, 500));
    particleGui.add(CutParticle::mass_base.set("mass base", 11.0, 0.0, 500.0));
    particleGui.add(CutParticle::speed_limit.set("speed limit", 20.0, 0.0, 50.0));
    particleGui.add(CutParticle::trail.set("trail on", true));
    particleGui.add(CutParticle::enable_home_in_history.set("trail include home", false));
    particleGui.add(CutParticle::trail_wgt.set("trail weight", 5.0, 0.0, 10.0));
    particleGui.add(CutParticle::history_length.set("history length", 3, 0, 50));

    gui.add(particleGui);

    p_man_gui.setup("P MEN");

}

//--------------------------------------------------------------
void ofApp::drawGui(ofEventArgs& args) {
    gui.draw();
    p_man_gui.draw();
}

//--------------------------------------------------------------
// where amt is the amount of panels we SHOULD have 
// this function removed unwanted p men guis
//--------------------------------------------------------------
void ofApp::updatePMenGui(int amt) {

    vector<ofxBaseGui*> gs;
    //back up ofParamGroups we want to keep
    for (auto g : p_man_gui.getControlNames()) {
        auto index = ofToInt(g);
        if (index < amt)gs.push_back(p_man_gui.getControl(g));  //might need to not make this a pointer if clearning???
    }
    //delete them all
    p_man_gui.clear();
    //add back the ones we want 
    for (auto g : gs)
        p_man_gui.add(g);
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
    case '[':
        //spawn 
        for (auto& p : p_men)
            p.spawn();
        break;
    case ']':
        //random spawn 
        for (auto& p : p_men)
            p.randomSpawn();
        break;
    case '-':
        //clear p's only 
        for (auto& p : p_men)
            p.clear();
        break;
    case 'a':
        for (auto& p : p_men)
            p.v_gravity_direction = 0;
        break;
    case 's':
        for (auto& p : p_men)
            p.v_gravity_direction = 1;
        break;
    case 'd':
        for (auto& p : p_men)
            p.v_gravity_direction = 2;
        break;
    case 'w':
        for (auto& p : p_men)
            p.v_gravity_direction = 3;
        break;
    case '0':
        //@TODO: once gui tidied up - update this to actually save all
        gui.saveToFile("1_gui.xml");
        p_man_gui.saveToFile("1_pmen_gui.xml");
        break;
    case '9':
        gui.saveToFile("2_gui.xml");
        p_man_gui.saveToFile("2_pmen_gui.xml");
        break;
    case '8':
        gui.saveToFile("3_gui.xml");
        p_man_gui.saveToFile("3_pmen_gui.xml");
        break;
    case '7':
        gui.saveToFile("4_gui.xml");
        p_man_gui.saveToFile("4_pmen_gui.xml");
        break;
    case 'p':
        gui.loadFromFile("1_gui.xml");
        p_man_gui.loadFromFile("1_pmen_gui.xml");
        break;
    case 'o':
        gui.loadFromFile("2_gui.xml");
        p_man_gui.loadFromFile("2_pmen_gui.xml");
        break;
    case 'i':
        gui.loadFromFile("3_gui.xml");
        p_man_gui.loadFromFile("3_pmen_gui.xml");
        break;
    case 'u':
        gui.loadFromFile("4_gui.xml");
        p_man_gui.loadFromFile("4_pmen_gui.xml");
        break;
    case'1':
        media_man.nxtVideo(bg_feed);
        break;
    case '2':
        media_man.prevVideo(bg_feed);
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