#include "ofMain.h"
#include "ofApp.h"
#define TETRIS_SIZE 30

//========================================================================
int main( ){

	ofSetupOpenGL(12 * TETRIS_SIZE,25 * TETRIS_SIZE, OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new ofApp());

}
