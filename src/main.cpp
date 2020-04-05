#include "ofMain.h"
#include "ofApp.h"

int main( ){
	ofGLFWWindowSettings settings;
	settings.setGLVersion(4, 5);
	settings.setSize(1920, 1080);
	settings.windowMode = OF_WINDOW;
	ofCreateWindow(settings);
	ofRunApp(new ofApp());

}
