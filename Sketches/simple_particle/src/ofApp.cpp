#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

   // ofBackground(227, 155, 170);
    ofSetBackgroundAuto(false);
    ofEnableAlphaBlending();
    ofSetFrameRate(60);


    //! init particle man
    //  dims vec4( x_min, x_max, y_min, y_max);

    p_man.setup( glm::vec4(0, ofGetWidth(), 0, 29) );

}

//--------------------------------------------------------------
void ofApp::update(){

    p_man.applyVaryingGravity(-0.1, 0.0);

    //force container
    ofVec2f force;

    for (int i = 0; i < blob.size(); i++) {
        for (int j = 0; j < p_man.p.size(); j++) {
            //@TODO: add this function to blob class in final sketch
            if (blob[i].inRange(&p_man.p[j])) {

                //if in range of the blob
                //give repel a location and radius ( i.e. blob pos & r )
                repel.update(blob[i].location, blob[i].radius);
                force = repel.force(&p_man.p[j]);

                //drag.update(blob[i].location, blob[i].radius);
                //force = drag.force(&p_man.p[j]);

                //friction.update(blob[i].location, blob[i].radius);
                //force = friction.force(&p_man.p[j]);

                //attract.update(blob[i].location, blob[i].radius);
                //force = attract.force(&p_man.p[j]);

                p_man.p[j].applyforce(force);

            }
        }
    }

    p_man.applySeek();


    p_man.update();



}

//--------------------------------------------------------------
void ofApp::draw(){

    //we got a slight trail..

    ofSetColor(0,0,255,50);
    ofFill();
    ofRect(0, 0, ofGetWidth(), ofGetHeight());

    for (int i = 0; i < blob.size(); i++) {
        blob[i].display();
    }

    p_man.draw();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    //hit 0 to spawn
    if (key == '0') {
        p_man.simpleSpawn();
       // p_man.multiSpawn( 5 );
    }
    

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

    if (blob.size() > 0) {
        blob[blob.size() - 1].update(ofVec2f(x, y));
    }

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

    //on mouse clicked create a force object
    //simulating bloobs by making them force objects for now..
    Forces f;
    f.update(ofVec2f(x, y), 15);
    blob.push_back(f);

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
