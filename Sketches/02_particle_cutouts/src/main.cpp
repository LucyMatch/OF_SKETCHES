#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	ofGLWindowSettings settings;

	//primary window
	settings.setSize(1000, 1000);
	settings.setPosition(glm::vec2(400, 50));
	shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(settings);

	//gui / settings window
	settings.setSize(500, 1000);
	settings.setPosition(glm::vec2(50, 50));
	shared_ptr<ofAppBaseWindow> guiWindow = ofCreateWindow(settings);
	guiWindow->setVerticalSync(false);

	shared_ptr<ofApp> mainApp(new ofApp);
	mainApp->initGui();

	ofAddListener(guiWindow->events().draw, mainApp.get(), &ofApp::drawGui);

	ofRunApp(mainWindow, mainApp);
	ofRunMainLoop();

}
