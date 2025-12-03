#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( )
{
	ofGLWindowSettings settings;
	settings.windowMode = OF_WINDOW;
	settings.setSize(1440, 1080); // 4:3 aspect ratio

	auto window = ofCreateWindow(settings);
	ofSetWindowPosition((1920 - 1440) / 2, 0);

	ofRunApp(window, make_shared<ofApp>());
	ofRunMainLoop();
}
