#include "ofApp.h"

#define WIDTH 256
#define HEIGHT 256

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofEnableSmoothing();
    ofSetFrameRate(60);
    
    //color.allocate(WIDTH, HEIGHT);
    bokeh.allocate(WIDTH, HEIGHT);
    phylo.allocate(WIDTH, HEIGHT);
    
    bokeh.setRadius(4);
    
    /*color.setCode(STRINGIFY(
                            uniform sampler2DRect tex0;
                            
                            void main() {
                                vec2 pos = gl_TexCoord[0].xy;
                                float intensity = texture2DRect(tex0, pos).r;
                                
                                gl_FragColor = vec4(intensity, intensity, intensity, 1.0);
                            }
                  ));*/
    
    ofSetWindowShape(1280, 720);
    
    
    // setup gui
    gui = new ofxUISuperCanvas("Parameters");
    
    gui->addFPS();
    gui->addSlider("mu", 0.0, 0.005, &phylo.mu)->setLabelPrecision(4);
    gui->addSlider("beta", 0.0, 10.0, &phylo.beta);
    
    gui->loadSettings("gui.xml");
    
    reset();
}

//--------------------------------------------------------------
void ofApp::reset() {
    
    phylo.begin();
    ofBackground(0);
    phylo.end();
    
    bokeh.begin();
    ofBackground(0);
    ofSetColor(255, 0, 0);
    ofCircle(WIDTH / 2, HEIGHT / 2, 3);
    bokeh.end();
}


//--------------------------------------------------------------
void ofApp::update(){
    
    bokeh.update();
    
    phylo << bokeh;
    
    for (int i = 0; i < 100; i++)
        phylo.update();
    
    bokeh.begin();
    ofBackground(0);
    bokeh.end();
}

//--------------------------------------------------------------
void ofApp::draw(){

    ofBackground(0, 0, 0);

    bokeh.draw();
    
    ofTranslate(WIDTH, 0);
    phylo.draw();
    ofTranslate(-WIDTH, 0);
    
    //color.draw();
}

//--------------------------------------------------------------
void ofApp::exit(){
    
    gui->saveSettings("gui.xml");
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    reset();
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

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
