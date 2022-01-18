#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetFrameRate(60);
    ofSetVerticalSync(false);
    ofEnableSmoothing();
    ofEnableAlphaBlending();

    canvas_dims = glm::vec2(1920, 1080);

    // init gui
    initGui();

    //media man - test - simple video backgrounds
    Feed video_feed;
    video_feed.path = "videos";
    video_feed.media_type = mediaTypes::VIDEO;
    local_default_video_feed = media_man.createNewFeed(video_feed);

    ////media man - test 2nd feed - simple video backgrounds
    Feed _video_feed;
    _video_feed.path = "videos";
    _video_feed.media_type = mediaTypes::VIDEO;
    local_default_video_feed_2 = media_man.createNewFeed(_video_feed);

    //media man - testing image
    Feed img_feed;
    img_feed.path = "images/misc";
    img_feed.media_type = mediaTypes::IMAGE;
    img_feed.enable_slideshow = true;
    local_default_image_feed = media_man.createNewFeed(img_feed);

    Feed img_c_feed;
    img_c_feed.path = "images/eyes"; 
    img_c_feed.media_type = mediaTypes::IMAGE_COLLECTION;
    img_c_feed.collection_size = 5;
    local_image_collection = media_man.createNewFeed(img_c_feed);

    //media man - testing image collections stored in vector
//string img_dirs[] = { "clouds", "eyes", "rocks", "sky", "sky2" };
//for (int i = 0; i < sizeof(img_dirs); i++) {
//    Feed _feed;
//    _feed.path = "images/" + img_dirs[i];
//    _feed.media_type = mediaTypes::IMAGE_COLLECTION;
//    _feed.collection_size = 5;
//    //_feed.enable_slideshow = true;
//    local_image_feeds.push_back(media_man.createNewFeed(_feed));
//}

}

//--------------------------------------------------------------
void ofApp::update(){

    framerate();

    //update media man - to update frames
    media_man.update();

}

//--------------------------------------------------------------
void ofApp::draw(){

    ofSetColor(bg_c);
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

    float x = 0, y = 0;
    if (media_man.getFrameTexture(local_default_video_feed)->isAllocated()) {
        x = media_man.getFrameTexture(local_default_video_feed)->getWidth();
        y = media_man.getFrameTexture(local_default_video_feed)->getHeight();
    }

    if (local_default_video_feed != NULL)
        media_man.getFrameTexture(local_default_video_feed)->draw(0, 0);

    if(local_default_video_feed_2 != NULL )
        media_man.getFrameTexture(local_default_video_feed_2)->draw(x, 0);

    if (local_default_image_feed != NULL) {
        ofPushMatrix();
        ofTranslate(mouseX, mouseY);
        ofScale(0.25);
        media_man.getFrameTexture(local_default_image_feed)->draw(0, 0);
        ofPopMatrix();
    }


    int w = ofGetWidth() / local_image_collection->collection_size, h;
    int child_counter = 0;

    


    for (auto tex : media_man.getFrameTextures(local_image_collection)) {
        h = (tex->getHeight() / tex->getWidth()) * w;
        ofPushMatrix();
        ofTranslate(child_counter * w, y);
            tex->draw(0, 0, w, h);
        ofPopMatrix();
        child_counter++;
    }


    //int parent_counter = 0;

    //for (const auto& f : local_image_feeds) {

    //    int w = ofGetWidth() / f->collection_size, h;
    //    int child_counter = 0;

    //    for (auto tex : media_man.getFrameTextures(local_default_image_feed)) {
    //        h = (tex->getHeight() / tex->getWidth()) * w;
    //        ofPushMatrix();
    //        ofTranslate(child_counter * w, parent_counter * h);
    //        tex->draw(0, 0, w, h);
    //        ofPopMatrix();
    //        child_counter++;
    //    }
    //    parent_counter++;
    //}
    
    if (enable_debug) drawDebug();

    if (enable_info) drawInfo();

}

//--------------------------------------------------------------
void ofApp::drawDebug() {

    media_man.drawDebug();

}

//--------------------------------------------------------------
void ofApp::initGui() {

    gui.setup("P R I M A R Y");
    //48, 255, 297   //255, 70, 86
    gui.add(bg_c.set("background", ofColor(138, 184, 164, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));
    gui.add(enable_debug.set("enable debug", false));
    gui.add(enable_info.set("enable info", false));

}

//--------------------------------------------------------------
void ofApp::drawGui(ofEventArgs& args) {
    gui.draw();
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
    case']':
        media_man.nxtVideo(local_default_video_feed);
        break;
    case '[':
        media_man.prevVideo(local_default_video_feed);
        break;
    case'.':
        media_man.nxtVideo(local_default_video_feed_2);
        break;
    case ',':
        media_man.prevVideo(local_default_video_feed_2);
        break;
    case'\'':
        media_man.nxtImage(local_default_image_feed);
        break;
    case ';':
        media_man.nxtImage(local_default_image_feed);
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

    //@TODO: UPDATE THIS WITH RELEVANT INFO
    // 
    // ofToString formatting available in 0072+
    //ss << "          Video Feed: " << video.getVideoTitle() << std::endl;
    //ss << "          Export Interval: " << time_interval / 1000 << " /s" << std::endl;
    //string enabled = (enable_interval_export) ? "yes" : "no";
    //ss << "          Export Interval Active: " << enabled << std::endl;
    ss << "---------------------------------- " << std::endl;
    ss << " Key Controls " << std::endl;
    ss << " Next Local Video              :  '.' " << std::endl;
    ss << " Prev Local Video              :  ',' " << std::endl;
    //ss << " Single Export           :  'x' " << std::endl;
    //ss << " Toggle Interval Export  :  'z' " << std::endl;
    //ss << " Toggle This Info        :  'b' " << std::endl;
    //ss << " Spawn                   :  'p' " << std::endl;
    //ss << " Random Spawn            :  'o' " << std::endl;
    //ss << " Clear Particles         :  '-' " << std::endl;
    //ss << " Clear Fbos              :  '=' " << std::endl;


    ofSetColor(255);
    ofDrawBitmapString(ss.str(), 10, 10 + 12);

    ofDisableAlphaBlending();

    ofPopStyle();
    ofPopMatrix();
}

