#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    
    ofEnableSmoothing();
    ofSetFrameRate(FPS);
    
    ofSetWindowShape(1280, 720);
    ofSetCircleResolution(100);
    
    gen = 0;

    phylo.allocate(WIDTH, HEIGHT);
    blur.allocate(WIDTH, HEIGHT);
    lut.allocate(WIDTH, HEIGHT);
    
    blur.setInternalFormat(GL_RGB32F);
    blur.setRadius(0);
    
    lut.setInternalFormat(GL_RGB32F);
    lut.loadLUT("sunflower.cube");
    
    exporter.allocate(WIDTH, HEIGHT, OF_IMAGE_COLOR);
    
    
    // cv
    maskGray.allocate(WIDTH, HEIGHT);
    maskColor.allocate(WIDTH, HEIGHT);
    
    
    framePrev = new ofxCvGrayscaleImage();
    frameCnt  = new ofxCvGrayscaleImage();
    
    framePrev->allocate(WIDTH, HEIGHT);
    frameCnt->allocate(WIDTH, HEIGHT);
    frameDiff.allocate(WIDTH, HEIGHT);
    
    
    // setup gui
    gui = new ofxUISuperCanvas("Parameters");
    
    gui->addFPS();
    lblGen = gui->addLabel("GENERATION", "");
    gui->addSlider("mu", 0.0, 0.01, &phylo.mu)->setLabelPrecision(4);
    gui->addSlider("beta", 0.0, 10.0, &phylo.beta);
    
    gui->setTheme(OFX_UI_THEME_BERLIN);
    gui->loadSettings("gui.xml");
    
    // reset
    setInitialPattern();
}

//--------------------------------------------------------------
void ofApp::setInitialPattern() {
    
    gen = 0;
    
    // load image
    maskImg.loadImage("heart.png");
    maskColor.setFromPixels(maskImg.getPixelsRef());
    maskGray.setFromColorImage(maskColor);
    
    
    // calucate initial seed arrangement
    contourFinder.findContours(maskGray, 5, (WIDTH*HEIGHT), 10, false);
    
    
    // setup initial pattern
    blur.begin();
    
    ofBackground(0);
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    ofSetColor(0, 255, 0);
    maskImg.draw(0, 0);
    
    for (int i = 0; i < contourFinder.nBlobs; i++) {
        ofxCvBlob blob = contourFinder.blobs[i];
        
        ofPath path;
    
        path.setFilled(false);
        path.setStrokeWidth(3);
        path.setStrokeColor(ofColor(255, 255, 0));
        path.moveTo(blob.pts[0]);
        
        for (int j = 0; j < blob.nPts; j++) {
            
            path.lineTo(blob.pts[j]);
        }
        
        path.close();
        path.draw();
    }

    blur.end();
    blur.update();
    
    // apply and set initial pattern
    phylo.setTexture(blur.getTextureReference());
    phylo.init();
}

//--------------------------------------------------------------
void ofApp::update(){
    
    for (int i = 0; i < STEP; i++) {
        
        phylo << blur;
    }
    
    //lut << phylo;
    
    frameFbo = phylo.getBackBuffer();
    frameFbo->readToPixels(framePixels);
    maskColor.setFromPixels(framePixels.getPixels(), WIDTH, HEIGHT);
    
    frameCnt->setFromColorImage(maskColor);
    
    frameDiff.absDiff(*framePrev, *frameCnt);
    
    frameDiff.threshold(128);
    
    
    gen += STEP;
}

//--------------------------------------------------------------
void ofApp::draw(){

    ofBackground(32);
    
    ofPushMatrix();
    
    blur.draw();
    ofSetColor(255);
    ofTranslate(WIDTH, 0);
    phylo.draw();
    ofTranslate(WIDTH, 0);
    maskColor.draw(0, 0);
    //lut.draw();
    ofPopMatrix();
    
    ss.str("");
    ss << "generation:" << gen;
    lblGen->setLabel(ss.str());
    
    /*ss.str("");
    ss << "../../mov/of_exported/sunflower" << setfill('0') << setw(6) << ofGetFrameNum() << ".bmp";
    exporter.grabScreen(WIDTH * 2, 0, WIDTH, HEIGHT);
    exporter.saveImage(ss.str());*/
}

//--------------------------------------------------------------
void ofApp::exit(){
    
    gui->saveSettings("gui.xml");
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if (key == ' ') {
        
        setInitialPattern();
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
