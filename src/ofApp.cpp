#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    
    ofEnableSmoothing();
    ofSetFrameRate(60);
    
    ofSetWindowShape(1280, 720);
    
    gen = 0;

    phylo.allocate(WIDTH, HEIGHT);
    
    
    // setup gui
    gui = new ofxUISuperCanvas("Parameters");
    
    gui->addFPS();
    lblGen = gui->addLabel("GENERATION", "");
    gui->addSlider("mu", 0.0, 0.005, &phylo.mu)->setLabelPrecision(4);
    gui->addSlider("beta", 0.0, 10.0, &phylo.beta);
    
    gui->setTheme(OFX_UI_THEME_BERLIN);
    gui->loadSettings("gui.xml");
}

//--------------------------------------------------------------
void ofApp::update(){
    
    for (int i = 0; i < STEP; i++) {
        phylo.update();
    }
    gen += STEP;
    
    
}

//--------------------------------------------------------------
void ofApp::draw(){

    ofBackground(32);

    phylo.draw();
    
    ss.str("");
    ss << "generation:" << gen;
    lblGen->setLabel(ss.str());
}

//--------------------------------------------------------------
void ofApp::exit(){
    
    gui->saveSettings("gui.xml");
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
    phylo.begin();
    ofSetColor(255, 255, 255);
    ofCircle(WIDTH / 2, HEIGHT / 2, 100);
    phylo.end();
    
    cout << "pressed";
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
