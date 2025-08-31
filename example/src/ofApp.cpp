#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(30);
    
    aekey.open("test.txt");

    aekey.dumpTrackName();
    
    ofLog() << "Units per second: " << aekey.getUnitsPerSecond();
    ofLog() << "Source Height: " << aekey.getSourceHeight();
    ofLog() << "Source Width: " << aekey.getSourceWidth();

    const ofxAfterEffectsKeyframeParser::Track& t = aekey.getTrack("Transform/Rotation");

//    for (int i = 0 ; i <= aekey.getLastFrame(); i++)
//    {
//        cout << i << " " << t.getParam(i, "degrees") << endl;
//    }
    
//    track = const_cast<ofxAfterEffectsKeyframeParser::Track &>(t);
    
    track = t;
    
    ofSetRectMode(OF_RECTMODE_CENTER);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    float rotZ_deg = track.value().get().getParam(frame, "degrees");
//    float rotZ = ofDegToRad(rotZ_deg);
    
    ofDrawBitmapString("frame: " + ofToString(frame), 50, 50);
    ofDrawBitmapString("degrees: " + ofToString(rotZ_deg), 50, 80);

    ofTranslate(200, 200);
    ofRotateDeg(rotZ_deg);
    ofDrawRectangle(0, 0, 200, 10);

    if(frame < aekey.getLastFrame()){
        ++frame;
    }else{
        frame = 0;
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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
