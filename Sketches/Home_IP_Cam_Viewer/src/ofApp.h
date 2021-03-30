#pragma once

#include "ofMain.h"
#include "IPVideoGrabber.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void loadCams();
		ofx::Video::IpVideoGrabberSettings& nextCam();
		void drawCamInfo(std::shared_ptr<ofx::Video::IPVideoGrabber> &g);

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		std::vector<std::shared_ptr<ofx::Video::IPVideoGrabber>> ip_grabbers;
		std::vector<ofx::Video::IpVideoGrabberSettings> ip_info;

		

		enum view_mode {SINGLE, GRID} curr_mode;

		int cam_count, rows, cols, curr_cam = 0;
		
};
