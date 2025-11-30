#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( )
{
	ofGLWindowSettings settings;
	settings.windowMode = OF_WINDOW;
	settings.setSize(1920, 1080);

	auto window = ofCreateWindow(settings);
	ofSetWindowPosition(0, 0);

	ofRunApp(window, make_shared<ofApp>());
	ofRunMainLoop();
}
