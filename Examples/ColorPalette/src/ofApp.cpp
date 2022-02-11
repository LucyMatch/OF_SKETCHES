#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetVerticalSync(false);
    ofEnableSmoothing();
    ofEnableAlphaBlending();

    pals.load();
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

    ofSetColor(*pals.getColour( curr_palette, 0 ));
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

    if (enable_current) {
        auto p = pals.getPalette(curr_palette);
        auto size = ofGetWidth() / pals.getNColours(curr_palette);
        int count = 0;
        for (auto c : *p) {
            ofSetColor(c);
            ofDrawRectangle( size * count++, 0, size, size );
        }
    }

    for (auto s : shapes)
        s.draw();

    curr_shape.draw();

    if(enable_debug)
        pals.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
    case 'd' :
        enable_debug = !enable_debug;
        break;
    case 'c':
        enable_current = !enable_current;
        break;
    case '.':
        curr_palette = ++curr_palette % pals.getNPalettes();
        break;
    case ' ':
        shapes.clear();
        break;
    }
    std::cout << curr_palette << std::endl;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    //add to shape
    curr_shape.curveTo(x, y);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    //start new shape
    curr_shape.clear();
    curr_shape.setFillColor( *pals.getColour( curr_palette, curr_shape_colour ) );
    curr_shape.curveTo(x, y);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    //finish shape
    curr_shape.curveTo(curr_shape.getOutline()[0].getVertices()[0]);
    shapes.push_back(curr_shape);
    //update colour for next shape
    curr_shape_colour = ++curr_shape_colour % pals.getNColours(curr_palette);
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
