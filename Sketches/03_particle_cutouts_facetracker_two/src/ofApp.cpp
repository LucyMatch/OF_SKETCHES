#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetVerticalSync(false);
    ofEnableSmoothing();
    ofEnableAlphaBlending();

    main_draw.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    p_draw.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);

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
    // 
    //@TODO: not in love with this approach
    //it may make sense for either another manager
        //to house (cut_man)landmark manager + particle men
        //or (cut_man)landmarkmanager to own particle men.... 

    if (video.cam.isFrameNew()) {

        tracker.update(video.cam); 
        auto t = tracker.getInstances();

        cut_man.update(t);

        if (cut_man.faces.size() > 0) {
            //we know we have faces!
            //for every "face" we should have pmen per feature
            for (int i = 0; i < cut_man.faces.size(); i++) {
                for (int j = 0; j < cut_man.faces[i].size(); j++) {
                    //looping through "landmarks" /features of faces from cut_man
                    //index we want for the pmen would be i + ( j *  cut_man.faces.size())
                    auto index = i + ( j * cut_man.faces.size() );
                    if (index < p_men.size()) {
                        //pman_exists!
                        //update cut ( shape )
                        p_men[index].update(cut_man.faces[i][j].cut);
                        //update texture
                        ofTexture tmp_tex;
                        tmp_tex = cut_man.getCutTexture( cut_man.faces[i][j].cut, *(video.getFrameTex()));
                        p_men[index].update( tmp_tex );
                        //update position
                        if (enable_auto_spawn)p_men[index].spawn();
                        if (enable_varying_gravity)p_men[index].applyVaryingGravity(v_gravity_min, v_gravity_max, v_gravity_direction);
                        p_men[index].update();
                    }
                    else {
                        //we must create a new pman!
                        CutParticleManager _cpm(cut_man.faces[i][j].cut);
                        _cpm.gui.setName(ofToString(index));
                        p_man_gui.add(_cpm.gui); //create a gui panel for new manager
                        p_men.push_back(_cpm);
                    }

                }
            }
            checkPmen();
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){

    //---------------------
    ////draw to particle fbo
    //p_draw.begin();
    //ofPushStyle();
    ////@TODO: come up with other ways of controlling this apha
    ////		 using animate?
    //    ofSetColor(pman_c, particle_fbo_alpha);
    //    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    //    for (auto& p : p_men) { p.draw(); }
    //ofPopStyle();
    //p_draw.end();
    //---------------------

    //---------------------
    // @TODO: test the bledning for trails is correct
    // @TODO: again test if this is actually how we want to draw... / tidy up!
    // draw to main fbo

    //ofPushStyle();
    //main_draw.begin();

    //    if (!enable_trails) {
    //        ofSetColor(bg_c);
    //        ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

    //        //draw "uncut" texture 
    //        if (enable_orig)
    //            video.draw();
    //    }


    //    //blend so we only get particles + their trails
    //    if (enable_trails) {
    //        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    //        p_draw.draw(0, 0);
    //    }

    //main_draw.end();
    //ofPopStyle();

    //if (enable_trails) {
    //    ofSetColor(bg_c);
    //    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

    //    //draw "original" texture 
    //    if (enable_orig)video.draw();
    //}
    //else {
    //    main_draw.draw(0, 0);
    //}

    //if (enable_plain_draw)for (auto& p : p_men) { p.draw(); }

    ofPushStyle();
    p_draw.begin();
        for (auto& p : p_men) { p.draw(); }
    p_draw.end();
    ofPopStyle();
    
    ofSetColor(bg_c);
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());


    if (enable_trails) {
        ofPushStyle();
        //draw "original" texture 
        if (enable_orig)video.draw();

        ofSetColor(pman_c, particle_fbo_alpha);
        p_draw.draw(0, 0);
        ofPopStyle();
    }


    if (enable_plain_draw)for (auto& p : p_men) { p.draw(); }



    ofEnableAlphaBlending();

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
    
}

//--------------------------------------------------------------
// removed unwanted pmen 
// this happens when u have multi faces - then the additional faces leave
//--------------------------------------------------------------
void ofApp::checkPmen() {
    if (cut_man.faces.size() > 0 && p_men.size() > (cut_man.faces.size() * cut_man.faces[0].size())) {
        auto amt = cut_man.faces.size() * cut_man.faces[0].size();          //amt we want
        p_men.erase(p_men.begin() + amt, p_men.begin() + p_men.size() );    //erase unwanted
        updatePMenGui( amt );                                               //remove unwanted gui panels
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

    //@TODO: tidy this - its a copy & paste job atm

    gui.setup("P R I M A R Y");

    gui.add(bg_c.set("background", ofColor(255, 228, 246, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));
    gui.add(enable_debug.set("enable debug", false));
    gui.add(enable_orig.set("enable orig", true));
    //gui.add(blend_mode.set("blend mode", 0, 0, 5));
    gui.add(time_interval.set("export interval", 1000, 100, 50000));

    gui.add(enable_trails.set("enable trails", true));
    //gui.add(enable_blend_min.set("enable blend min", false));
    //gui.add(enable_blend_max.set("enable blend max", false));
    gui.add(enable_plain_draw.set("enable plain draw", true));
    //gui.add(b_mode_selector.set("blend modes", 1, 0, blends.size() - 1));
    gui.add(enable_auto_spawn.set("enable auto spawning", false));
    gui.add(enable_varying_gravity.set("enable varying gravity", false));
    gui.add(v_gravity_direction.set("gravity direction", 0, 0, 3));
    gui.add(v_gravity_min.set("gravity min", 0.5, 0.5, 25));
    gui.add(v_gravity_max.set("gravity max", 10, 0.5, 25));

    gui.add(video.gui);
    gui.add(cut_man.gui);

    gui.add(particle_fbo_alpha.set("particle fbo alpha ctrl", 0, 0, 255));
    gui.add(pman_c.set("pman fbo colour", ofColor(255, 255, 255), ofColor(0, 0, 0), ofColor(255, 255, 255)));

    particleGui.clear();
    particleGui.setName("P");
    particleGui.add(CutParticle::pcolor.set("color", ofColor(0, 0, 0, 100), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));
    particleGui.add(CutParticle::tcolor.set("trail color", ofColor(0, 0, 0, 100), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));
    //particleGui.add(CutParticle::b_mode_selector.set("blend Mode Selector", 1, 0, blends.size() - 1));

    particleGui.add(CutParticle::r.set("radius", 10, 0, 1000));
    particleGui.add(CutParticle::enable_true_size.set("true size", true));
    particleGui.add(CutParticle::enable_random_size.set("random size", false));
    particleGui.add(CutParticle::enable_continous_resizing.set("continous resize", false));
    particleGui.add(CutParticle::size_min.set("size min", 11, 10, 500));
    particleGui.add(CutParticle::size_max.set("size max", 100, 0, 500));
    particleGui.add(CutParticle::mass_base.set("mass base", 11.0, 0.0, 500.0));
    particleGui.add(CutParticle::speed_limit.set("speed limit", 20.0, 0.0, 50.0));

    //particleGui.add(CutParticle::seek_on.set("seek on", true));
    //particleGui.add(CutParticle::seek_limit.set("seek limit", 50.0, 0.0, 50.0));
    //particleGui.add(CutParticle::arrive_on.set("arrive on", true));
    //particleGui.add(CutParticle::arrive_cap.set("arrive cap", 100, 0, 250));

    particleGui.add(CutParticle::trail.set("trail on", true));
    particleGui.add(CutParticle::enable_home_in_history.set("trail include home", false));
    particleGui.add(CutParticle::trail_wgt.set("trail weight", 5.0, 0.0, 10.0));
    particleGui.add(CutParticle::history_length.set("history length", 3, 0, 50));

    //repelGui.setup("R E P E L");
    //repelGui.add(enable_repel.set("repel enable", true));
    //repelGui.add(repel.force_ctrl);

        //attractGui.setup("A T T R A C T");
    //attractGui.add(enable_attract.set("attract enable", false));
    //attractGui.add(attract.force_ctrl);

    gui.add(particleGui);
    //gui.add(repelGui);
    //gui.add(attractGui);

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
void ofApp::updatePMenGui( int amt ) {

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
    case 'p':
        //spawn 
        for (auto& p : p_men)
            p.spawn();
        break;
    case 'o':
        //random spawn 
        for (auto& p : p_men)
            p.randomSpawn();
        break;
    case '-':
        //clear p's only 
        for (auto& p : p_men)
            p.clear();
        break;
    case '=':
        //@TODO: wait is this actually a thing?
        p_draw.begin();
        ofClear(0, 0, 0, 0);
        p_draw.end();
        main_draw.begin();
        ofClear(0, 0, 0, 0);
        main_draw.end();
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