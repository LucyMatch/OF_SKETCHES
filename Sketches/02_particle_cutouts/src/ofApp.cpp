#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	

	ofSetFrameRate(60);
	ofSetVerticalSync(false);
	ofSetCircleResolution(100);
	ofEnableSmoothing();
	ofEnableAlphaBlending();

	//ofSetBackgroundAuto(true);
	//ofBackground(255, 255, 255);

	main_draw.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);

	initImages();
	initCanvasGrid();
	initParticleMans();

	initGui();
}

//--------------------------------------------------------------
void ofApp::update(){

	if (enable_simple_spawn)
		spawn();

	//for static force ctrl
	if (enable_static_force)
		forceCtrl();

	framerate();

	for (auto& p : pman) { 
		if(enable_varying_gravity)p.applyVaryingGravity(v_gravity_min, v_gravity_max, v_gravity_direction);
		p.update(); 
		p.drawFbo(pman_bg_alpha);
	}

}

//--------------------------------------------------------------
void ofApp::draw(){

	main_draw.begin();
		ofPushStyle();
			if (enable_debug)drawDebug();
		ofPopStyle();

		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		for (auto& p : pman) { p.getFbo().draw(0, 0); }
	main_draw.end();

	ofPushStyle();
	ofEnableBlendMode(blends[b_mode_selector]);
		ofSetColor(background);
		ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
		main_draw.draw(0, 0);
	ofPopStyle();

	ofEnableAlphaBlending();
}

//--------------------------------------------------------------
/*
*   init the ImageHandlers
*	doing manually at this point so I can identify them
*/
//--------------------------------------------------------------
void ofApp::initImages() {
	rings = new ImageHandler("images/rings");
	rocks = new ImageHandler("images/rocks");
	eyes = new ImageHandler("images/eyes");
	mouthes = new ImageHandler("images/mouths");
	misc = new ImageHandler("images/misc");
	sky_only = new ImageHandler("images/sky_partial");
	//cloud_only = new ImageHandler("images/clouds_partial");
	//mixed_sky = new ImageHandler("images/mixed_sky");

	cloud_only = new ImageHandler("images/water");
	mixed_sky = new ImageHandler("images/bluem");
}

//--------------------------------------------------------------
/*
*	this creates a "grid" of "cells"
*	we can use this grid to create "canvas" cells
*	i.e. pass these to particle manager to make a particle grid per cell.... 
* 
*	@TODO: create a class for different grid utility functions 
	-> we will want different pman organisation approaches
*/
//--------------------------------------------------------------
void ofApp::initCanvasGrid() {
	cells.clear();

	//create "grid" coordinates
	//canvas
	auto width_margin = ofGetWidth() * canvas_margin;
	auto height_margin = ofGetHeight() * canvas_margin;
	auto w = ofGetWidth() - (2 * width_margin);
	auto h = ofGetHeight() - (2 * height_margin);
	canvas.set(width_margin, height_margin, w, h);
	//cells
	auto cell_w = ((1.0 / float(cols)) * float(w));
	auto cell_h = ((1.0 / float(rows)) * float(h));
	auto cell_w_m = cell_w * cell_margin;
	auto cell_h_m = cell_h * cell_margin;
	ofVec2f cell((cell_w - (2 * cell_w_m)), (cell_h - (2 * cell_h_m)));

	//save the cells
	for (int x = 0; x < cols; x++) {
		for (int y = 0; y < rows; y++) {
			auto x_step = (canvas.x + cell_w_m) + (x * cell_w);
			auto y_step = (canvas.y + cell_h_m) + (y * cell_h);
			ofRectangle c(x_step, y_step, cell.x, cell.y);
			cells.push_back(c);
			std::cout << "[ Canvas Man ] ADDED NEW CELL " << std::endl;
			std::cout << "[ Canvas Man ] " << x_step <<" - " << y_step << " - " << cell.x << " - " << cell.y << " - " <<std::endl;
		}
	}

}

//--------------------------------------------------------------
void ofApp::initParticleMans() {
	pman.clear();
	//@TODO: add in toggles / gui controlls for updating which img sets get passed to p man
	for (auto& c : cells) {
		ImageParticleManager p(mixed_sky->getImages());
		std::cout << "[ pman set up ] X : " << c.x <<" Y : "<<c.y << " W : " << c.width << " H : " << c.height << std::endl;
		p.setup(glm::vec4(c.x, c.y, c.width, c.height));
		pman.push_back(p);
	}
}

//--------------------------------------------------------------
void ofApp::spawn() {
	for (auto& p : pman) {
		p.simpleSpawn(pman_direction);
	}
}

//--------------------------------------------------------------
void ofApp::gridSpawn() {
	for (auto& p : pman) {
		p.gridSpawn(grid_spawn_w, grid_spawn_h);
	}
}

//--------------------------------------------------------------
void ofApp::forceCtrl(glm::vec2 loc) {

	bool update = false;
	if (loc != glm::vec2(-1, -1))
		update = true;

	if (enable_repel) {
		//update force
		if(update)repel.update(loc);

		for (auto& p : pman) {
			for (auto& _p : p.p) {
				if (repel.inRange(_p)) {
					_p.applyforce(repel.force(_p));
				}

			}
		}
	}

	if (enable_attract) {
		//update force
		if(update)attract.update(loc);

		for (auto& p : pman) {
			for (auto& _p : p.p) {
				if (attract.inRange(_p)) {
					_p.applyforce(attract.force(_p));
				}

			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::drawDebug(){
	//draws result from initCanvasGrid
	for (auto& pm : pman) pm.drawDebug();
	repel.display();
	attract.display();
}

//--------------------------------------------------------------
void ofApp::framerate(){
	std::stringstream strm;
	strm << "fps: " << ofGetFrameRate();
	ofSetWindowTitle(strm.str());
}

//--------------------------------------------------------------
void ofApp::initGui(){
	gui.setup("P R I M A R Y");
	gui.add(background.set("background", ofColor(255, 228, 246, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));
	gui.add(enable_debug.set("draw debug", true));
	gui.add(enable_simple_spawn.set("enable simple spawning", false));
	gui.add(enable_mouse_spawn.set("enable mouse spawning", false));
	gui.add(enable_mouse_force.set("enable mouse force", false));
	gui.add(enable_static_force.set("enable static force", false));
	gui.add(b_mode_selector.set("blend modes", 1, 0, blends.size()-1));
	
	//grid controls
	gui.add(canvas_margin.set("canvas margin", 0.2 , 0.0, 1.0));
	gui.add(cell_margin.set("cell margin", 0.12, 0.0, 1.0));
	gui.add(rows.set("cell rows", 1, 1, 20));
	gui.add(cols.set("cell cols", 1, 1, 20));
	
	//@TODO: - put these vals in ImageParticleManager ofPanel
	gui.add(grid_spawn_w.set("particle grid w", 4, 1, 100));
	gui.add(grid_spawn_h.set("particle grid h", 50, 1, 100));

	//for particle manager direction
	//eventually will want other things to play with
	gui.add(pman_direction.set("pman spawn direction", 0, 0, 3));
	gui.add(enable_varying_gravity.set("enable varying gravity", false));
	gui.add(v_gravity_direction.set("gravity direction", 0, 0, 3));
	gui.add(v_gravity_min.set("gravity min", 0.5, 0.5, 25));
	gui.add(v_gravity_max.set("gravity max", 10, 0.5, 25));


	particleGui.setup("P");
	particleGui.add(ImageParticle::pcolor.set("color", ofColor(0, 0, 0, 100), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));
	particleGui.add(ImageParticle::tcolor.set("trail color", ofColor(0, 0, 0, 100), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));
	particleGui.add(ImageParticle::b_mode_selector.set("blend Mode Selector", 1, 0, blends.size() - 1));

	particleGui.add(ImageParticle::r.set("radius", 10, 0, 1000));
	particleGui.add(ImageParticle::enable_uniform_size.set("uniform size", true));
	particleGui.add(ImageParticle::enable_true_size.set("true size", true));
	particleGui.add(ImageParticle::mass_base.set("mass base", 11.0, 0.0, 500.0));
	particleGui.add(ImageParticle::speed_limit.set("speed limit", 20.0, 0.0, 50.0));

	particleGui.add(ImageParticle::seek_on.set("seek on", true));
	particleGui.add(ImageParticle::seek_limit.set("seek limit", 50.0, 0.0, 50.0));
	particleGui.add(ImageParticle::arrive_on.set("arrive on", true));
	particleGui.add(ImageParticle::arrive_cap.set("arrive cap", 100, 0, 250));

	particleGui.add(ImageParticle::trail.set("trail on", true));
	particleGui.add(ImageParticle::enable_home_in_history.set("trail include home", false));
	particleGui.add(ImageParticle::trail_wgt.set("trail weight", 5.0, 0.0, 10.0));
	particleGui.add(ImageParticle::history_length.set("history length", 3, 0, 50));

	repelGui.setup("R E P E L");
	repelGui.add(enable_repel.set("repel enable", true));
	repelGui.add(repel.force_ctrl);

	attractGui.setup("A T T R A C T");
	attractGui.add(enable_attract.set("attract enable", false));
	attractGui.add(attract.force_ctrl);

	pmanGui.setup("I M G   1  P M A N");
	pmanGui.add(pman_bg_alpha.set("bg alpha", 25, 0, 255));

	pmanGui.add(ImageParticleManager::b_mode_selector.set("blend Mode Selector", 1, 0, blends.size() - 1));

	pmanGui.add(ImageParticleManager::debug_c.set("debug background", ofColor(171, 196, 237), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));
	pmanGui.add(ImageParticleManager::enable_limit.set("p limit enable", false));
	pmanGui.add(ImageParticleManager::limit.set("p limit", 100, 1, 5000));
	pmanGui.add(ImageParticleManager::enable_kill.set("enable kill", true));
	pmanGui.add(ImageParticleManager::enable_kill_on_leave_screen.set("enable kill on leave screen", true));
	pmanGui.add(ImageParticleManager::enable_bounce.set("enable bounce", false));
	pmanGui.add(ImageParticleManager::randomize_assets.set("randomize assets", true));

}

//--------------------------------------------------------------
void ofApp::drawGui(ofEventArgs& args){
	gui.draw();
	particleGui.draw();
	repelGui.draw();
	attractGui.draw();
	pmanGui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	switch (key) {
	case 'x':
		//reinit canvas' + grids
		initCanvasGrid();
		initParticleMans();
		break;
	case 'a':
		//re spawn grid
		gridSpawn();
		break;
	case 's':
		//spawn
		spawn();
		break;
	case 'd':
		//spawn exact number of images
		for (auto& p : pman) 
			p.imgSpawn();
		break;
	case '.':
		for (auto& p : pman) 
			p.nextImgIndex();
		break;
	case ',':
		for (auto& p : pman) 
			p.prevImgIndex();
		break;
	case '1':
		gui.saveToFile("1_gui.xml");
		pmanGui.saveToFile("1_pmangui.xml");
		particleGui.saveToFile("1_blckgui.xml");
		repelGui.saveToFile("1_rplgui.xml");
		attractGui.saveToFile("1_attractgui.xml");
		break;
	case '2':
		gui.saveToFile("2_gui.xml");
		pmanGui.saveToFile("2_pmangui.xml");
		particleGui.saveToFile("2_blckgui.xml");
		repelGui.saveToFile("2_rplgui.xml");
		attractGui.saveToFile("2_attractgui.xml");
		break;
	case '3':
		gui.saveToFile("3_gui.xml");
		pmanGui.saveToFile("3_pmangui.xml");
		particleGui.saveToFile("3_blckgui.xml");
		repelGui.saveToFile("3_rplgui.xml");
		attractGui.saveToFile("3_attractgui.xml");
		break;
	case '4':
		gui.saveToFile("4_gui.xml");
		pmanGui.saveToFile("4_pmangui.xml");
		particleGui.saveToFile("4_blckgui.xml");
		repelGui.saveToFile("4_rplgui.xml");
		attractGui.saveToFile("4_attractgui.xml");
		break;
	case 'q':
		gui.loadFromFile("1_gui.xml");
		pmanGui.loadFromFile("1_pmangui.xml");
		particleGui.loadFromFile("1_blckgui.xml");
		repelGui.loadFromFile("1_rplgui.xml");
		attractGui.loadFromFile("1_attractgui.xml");
		break;
	case 'w':
		gui.loadFromFile("2_gui.xml");
		pmanGui.loadFromFile("2_pmangui.xml");
		particleGui.loadFromFile("2_blckgui.xml");
		repelGui.loadFromFile("2_rplgui.xml");
		attractGui.loadFromFile("2_attractgui.xml");
		break;
	case 'e':
		gui.loadFromFile("3_gui.xml");
		pmanGui.loadFromFile("3_pmangui.xml");
		particleGui.loadFromFile("3_blckgui.xml");
		repelGui.loadFromFile("3_rplgui.xml");
		attractGui.loadFromFile("3_attractgui.xml");
		break;
	case 'r':
		gui.loadFromFile("4_gui.xml");
		pmanGui.loadFromFile("4_pmangui.xml");
		particleGui.loadFromFile("4_blckgui.xml");
		repelGui.loadFromFile("4_rplgui.xml");
		attractGui.loadFromFile("4_attractgui.xml");
		break;
	case '7':
		gui.saveToFile("001_gui.xml");
		pmanGui.saveToFile("001_pmangui.xml");
		particleGui.saveToFile("001_blckgui.xml");
		repelGui.saveToFile("001_rplgui.xml");
		attractGui.saveToFile("001_attractgui.xml");
		break;
	case '8':
		gui.saveToFile("002_gui.xml");
		pmanGui.saveToFile("002_pmangui.xml");
		particleGui.saveToFile("002_blckgui.xml");
		repelGui.saveToFile("002_rplgui.xml");
		attractGui.saveToFile("002_attractgui.xml");
		break;
	case '9':
		gui.saveToFile("003_gui.xml");
		pmanGui.saveToFile("003_pmangui.xml");
		particleGui.saveToFile("003_blckgui.xml");
		repelGui.saveToFile("003_rplgui.xml");
		attractGui.saveToFile("003_attractgui.xml");
		break;
	case '0':
		gui.saveToFile("004_gui.xml");
		pmanGui.saveToFile("004_pmangui.xml");
		particleGui.saveToFile("004_blckgui.xml");
		repelGui.saveToFile("004_rplgui.xml");
		attractGui.saveToFile("004_attractgui.xml");
		break;
	case 'u':
		gui.loadFromFile("001_gui.xml");
		pmanGui.loadFromFile("001_pmangui.xml");
		particleGui.loadFromFile("001_blckgui.xml");
		repelGui.loadFromFile("001_rplgui.xml");
		attractGui.loadFromFile("001_attractgui.xml");
		break;
	case 'i':
		gui.loadFromFile("002_gui.xml");
		pmanGui.loadFromFile("002_pmangui.xml");
		particleGui.loadFromFile("002_blckgui.xml");
		repelGui.loadFromFile("002_rplgui.xml");
		attractGui.loadFromFile("002_attractgui.xml");
		break;
	case 'o':
		gui.loadFromFile("003_gui.xml");
		pmanGui.loadFromFile("003_pmangui.xml");
		particleGui.loadFromFile("003_blckgui.xml");
		repelGui.loadFromFile("003_rplgui.xml");
		attractGui.loadFromFile("003_attractgui.xml");
		break;
	case 'p':
		gui.loadFromFile("004_gui.xml");
		pmanGui.loadFromFile("004_pmangui.xml");
		particleGui.loadFromFile("004_blckgui.xml");
		repelGui.loadFromFile("004_rplgui.xml");
		attractGui.loadFromFile("004_attractgui.xml");
		break;
	default:
		break;
	}

	if (key == OF_KEY_RETURN) {

		//we need to figure out how to get a true screen grab - not frame 
		//as our bg is transparent we get differing colours 
		//just drawing a white bg doesn't help either.. 
		glReadBuffer(GL_FRONT);
		ofSaveScreen("screenshot_" + ofToString(ofGetMonth()) + "_" + ofToString(ofGetDay()) + "_" + ofToString(ofGetYear()) + "_" + ofToString(ofGetHours()) + "_" + ofToString(ofGetMinutes()) + ofToString(ofGetSeconds()) + ".png");

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

	//mouse spawn
	if (enable_mouse_spawn) {
		//we could add in all pmen.... 
		for (auto &p : pman)
			p.spawn(ofVec2f(x, y));

		//or just first...
		//pman[0].spawn(ofVec2f(x, y));
	}

	//force ctrl
	if (enable_mouse_force || enable_static_force) {

		forceCtrl(glm::vec2(x, y));

		//simple force
		//im going to ignore the pman for now.. lazy ass coding

		//if (enable_repel) {
		//	//update force
		//	repel.update(ofVec2f(x, y));

		//	for (auto& p : pman) {
		//		for (auto& _p : p.p) {
		//			//repel for now
		//			if (repel.inRange(_p)) {
		//				_p.applyforce(repel.force(_p));
		//			}

		//		}
		//	}
		//}

		//if (enable_attract) {
		//	//update force
		//	attract.update(ofVec2f(x, y));

		//	for (auto& p : pman) {
		//		for (auto& _p : p.p) {
		//			//repel for now
		//			if (attract.inRange(_p)) {
		//				_p.applyforce(attract.force(_p));
		//			}

		//		}
		//	}
		//}
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

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

	main_draw.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
