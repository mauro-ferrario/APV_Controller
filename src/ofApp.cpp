#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
  ofSetFrameRate(60);
  ofBackground(0);
  controller.init();
  ofSetLogLevel(OF_LOG_ERROR);
}

//--------------------------------------------------------------
void ofApp::update(){
  controller.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
  controller.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

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
  controller.windowResized(w,h);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
