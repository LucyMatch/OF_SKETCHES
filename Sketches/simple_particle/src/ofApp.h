#pragma once

#include "ofMain.h"
#include "ParticleManager.h"
#include "Forces.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

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

		ParticleManager p_man;
		vector<Forces> blob; // faking blob with forces base class - its a simple location holder for now

		//new polymorph version
		Repel repel;
		Attract attract;
		Friction friction;
		Drag drag;
		
};
