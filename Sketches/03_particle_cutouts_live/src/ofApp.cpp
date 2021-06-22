#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetVerticalSync(false);
	ofEnableSmoothing();
	ofEnableAlphaBlending();

	main_draw.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
	p_draw.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);

	//init vid manager
	video.setDims(glm::vec2(1280, 720));
	video.setup(); //also here we would define - ip / webcam / local

	//init cut manager
	cut_man.setup();

	//init gui
	initGui();

}

//--------------------------------------------------------------
void ofApp::update() {

	framerate();

	//update video for new frame
	video.update();

	//live update cut manager w/ mouse position
	cut_man.update(mouseX, mouseY);

	//update cuts & ps
	for (auto& p : p_man) {
		if (video.cam.isFrameNew() && p.p.size() > 0) {
			ofTexture tmp_tex;
			tmp_tex = cut_man.getCutTexture( p.getCut() , *(video.getFrameTex()));
			p.update(tmp_tex);
		}	
		if (enable_auto_spawn)p.spawn();
		if (p.p.size() > 0) {
			if (enable_varying_gravity)p.applyVaryingGravity(v_gravity_min, v_gravity_max, v_gravity_direction);
			p.update();
		}
	}

	

}

//--------------------------------------------------------------
void ofApp::draw() {

	p_draw.begin();
		ofPushStyle();
			//@TODO: come up with other ways of controlling this apha
			//		 using animate?
			ofSetColor(pman_c, particle_fbo_alpha);
			ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
			for (auto& p : p_man) { p.draw(); }
		ofPopStyle();
	p_draw.end();

	main_draw.begin();
		//blend so we only get particles + their trails
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		p_draw.draw(0,0);
	main_draw.end();

	ofSetColor(bg_c);
	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

	//draw "original" texture 
	if (enable_orig)video.draw();
	
	main_draw.draw(0, 0);

	if(enable_debug)drawDebug();

	ofEnableAlphaBlending();

}

//--------------------------------------------------------------
void ofApp::drawDebug() {

	cut_man.draw();

	for (auto& p : p_man)
		p.drawDebug();

}

//--------------------------------------------------------------
void ofApp::framerate() {
	std::stringstream strm;
	strm << "fps: " << ofGetFrameRate();
	ofSetWindowTitle(strm.str());
}

//--------------------------------------------------------------
void ofApp::initGui() {

	//@TODO: looks like this is getting called twice?
	//		 ive done stuff to make sure duplicate dont show up
	//		but you shoud look into it...

	gui.setup("P R I M A R Y");

	gui.add(bg_c.set("background", ofColor(255, 228, 246, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));
	gui.add(enable_debug.set("enable debug", false));
	gui.add(enable_orig.set("enable orig", true));
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
	
	//@TODO: 
	//these vals are all duplicated in gui ??
	//WHYYYYY
	//is init actually getting called twice???

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

	//seems to be working
	//@TODO: test limits + bounce - hard to tell if working
	p_man_gui.setup("P MEN");

}

//--------------------------------------------------------------
void ofApp::drawGui(ofEventArgs& args) {
	gui.draw();
	p_man_gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

	switch (key) {
	case 's':
		cut_man.exportCuts(video.getFrameTex());
		break;
	case '.':
		video.nxtFeed();
		break;
	case ',':
		video.prevFeed();
		break;
	case 'x':
		//clear saved cuts
		cut_man.clearCuts();
		p_man.clear();
		p_man_gui.clear();
		p_man_gui.setup("P MEN");
		break;
	case 'p':
		//spawn 
		for (auto& p : p_man)
			p.spawn();
		break;
	case 'o':
		//random spawn 
		for (auto& p : p_man)
			p.randomSpawn();
		break;
	case '-':
		//clear p's only 
		for (auto& p : p_man)
			p.clear();
		break;
	case '=':
		p_draw.begin();
			ofClear(0, 0, 0, 0);
		p_draw.end();		
		main_draw.begin();
			ofClear(0, 0, 0, 0);
		main_draw.end();
		break;
	case '1':
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
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

	CutParticleManager _cpm( *(cut_man.saveCut()) );
	p_man_gui.add(_cpm.gui); //createa gui panel for new manager
	p_man.push_back( _cpm );

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

