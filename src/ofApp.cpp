#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0, 0, 0);
    ofEnableAlphaBlending();
    ofSetFrameRate(30);
    tetris.setup(0, 0, 30);
}

//--------------------------------------------------------------
void ofApp::update(){
    tetris.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    tetris.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    tetris.keyInput(key);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}