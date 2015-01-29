#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    
    ofEnableSmoothing();
    ofSetFrameRate(FPS);
    
    ofSetWindowShape(1280, 720);
    
    gen = 0;

    phylo.allocate(WIDTH, HEIGHT);
    blur.allocate(WIDTH, HEIGHT);
    lut.allocate(WIDTH, HEIGHT);
    
    blur.setInternalFormat(GL_RGB32F);
    blur.setRadius(2);
    
    lut.setInternalFormat(GL_RGB32F);
    lut.loadLUT("sunflower.cube");
    
    exporter.allocate(WIDTH, HEIGHT, OF_IMAGE_COLOR);
    
    
    // setup gui
    gui = new ofxUISuperCanvas("Parameters");
    
    gui->addFPS();
    lblGen = gui->addLabel("GENERATION", "");
    gui->addSlider("mu", 0.0, 0.01, &phylo.mu)->setLabelPrecision(4);
    gui->addSlider("beta", 0.0, 10.0, &phylo.beta);
    
    gui->setTheme(OFX_UI_THEME_BERLIN);
    gui->loadSettings("gui.xml");
    
    
    // reset
    phylo.init();
}

//--------------------------------------------------------------
void ofApp::update(){
    
    for (int i = 0; i < STEP; i++) {
        
        phylo << blur;
    }
    
    lut << phylo;
    
    gen += STEP;
}

//--------------------------------------------------------------
void ofApp::draw(){

    ofBackground(32);
    
    ofPushMatrix();
    blur.draw();
    ofTranslate(WIDTH, 0);
    phylo.draw();
    ofTranslate(WIDTH, 0);
    lut.draw();
    ofPopMatrix();
    
    ss.str("");
    ss << "generation:" << gen;
    lblGen->setLabel(ss.str());
    
    ss.str("");
    ss << "../../mov/of_exported/sunflower" << setfill('0') << setw(6) << ofGetFrameNum() << ".bmp";
    exporter.grabScreen(WIDTH * 2, 0, WIDTH, HEIGHT);
    exporter.saveImage(ss.str());
}

//--------------------------------------------------------------
void ofApp::exit(){
    
    gui->saveSettings("gui.xml");
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if (key == ' ') {

        blur.begin();
        ofBackground(0);
        
        float angle, x, y;
        
        int count = 30;
        
        for (int i = 0; i < count; i++) {
            
            angle = TWO_PI * (float)i / (float)count;
            x = (cos(angle) + 1.0) * WIDTH / 2.0;
            y = (sin(angle) + 1.0) * HEIGHT / 2.0;
            ofCircle(x, y, 3);
            
        }
        
        blur.end();
        blur.update();
        
        phylo << blur;
        gen++;
        
        blur.begin();
        ofBackground(0);
        blur.end();
        blur.update();
    
    }
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
