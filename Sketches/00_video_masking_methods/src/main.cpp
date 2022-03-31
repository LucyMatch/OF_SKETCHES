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

	//create pointer + init gui
	shared_ptr<ofApp> mainApp(new ofApp);
	mainApp->initGui();

	//add listener for gui 
	ofAddListener(guiWindow->events().draw, mainApp.get(), &ofApp::drawGui);

	//run app
	ofRunApp(mainWindow, mainApp);
	ofRunMainLoop();
}
