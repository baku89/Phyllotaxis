#include "ofApp.h"

ofApp::ofApp(vector<string> args) {
    
    if (args.size() > 0) {
        fileName = args[0];
    } else {
        fileName = "";
    }
    
    if (args.size() == 2) {
        ofSetDataPathRoot(args[1]);
    } else {
        ofSetDataPathRoot(DATA_DIR);
    }
}

//--------------------------------------------------------------
void ofApp::setup(){
    
    //ofSetDataPathRoot(DATA_DIR);
    
    ofEnableSmoothing();
    ofSetFrameRate(FPS);
    
    ofSetWindowShape(1920, 1080);
    ofSetCircleResolution(100);
    
    cntFrame = -1;
    gen = 0;
    frameDiffThreshold = 1.0;
    frameDiffContourMin = 4.0;
    frameDiffContourMax = 16.0;
    
    // load video
    source.loadMovie((string)"original/" + fileName + ".mov");
    if (!source.isLoaded()) {
        ofExit();
    }
    
    width = source.width;
    height = source.height;
    
    cout << "width:" << width << " height:" << height << endl;
    
    phylo.allocate(width, height);
    blur.allocate(width, height);
    lut.allocate(width, height);
    
    blur.setInternalFormat(GL_RGB32F);
    blur.setRadius(0);
    
    lut.setInternalFormat(GL_RGB32F);
    lut.loadLUT("sunflower.cube");
    
    exporter.allocate(width, height, OF_IMAGE_COLOR);
    
    exportName = fileName;//ofGetTimestampString("%m-%d-%H-%M-%S");
    
    // cv
    maskGray.allocate(width, height);
    cvColor.allocate(width, height);
    
    
    framePrev = new ofxCvGrayscaleImage();
    frameCnt  = new ofxCvGrayscaleImage();
    
    framePrev->allocate(width, height);
    frameCnt->allocate(width, height);
    frameDiff.allocate(width, height);
    
    
    // setup gui
    vector<float> buffer;
    for (int i = 0; i < 256; i++) {
        buffer.push_back(0.0);
    }
    
    gui = new ofxUISuperCanvas("Parameters");
    
    gui->addFPS();
    lblGen = gui->addLabel("GENERATION", "");
    lblProgress = gui->addLabel("PROGRESS");
    gui->addSlider("mu", 0.0, 0.01, &phylo.mu)->setLabelPrecision(4);
    gui->addSlider("beta", 0.0, 10.0, &phylo.beta);
    lblDiffSeedCount = gui->addLabel("DIFF SEED COUNT");
    mgDiffDetected = gui->addMovingGraph("frame diff detected", buffer, 256, 0.0, 60);
    gui->addSlider("frame diff threshold", 0.0, 4.0, &frameDiffThreshold);
    gui->addSlider("frame diff contour min", 0.0, 32.0, &frameDiffContourMin);
    gui->addSlider("frame diff contour max", 0.0, 32.0, &frameDiffContourMax);
    
    gui->autoSizeToFitWidgets();
    
    gui->setTheme(OFX_UI_THEME_BERLIN);
    gui->loadSettings("gui.xml");
    
    // reset
    bNextFrame = true;
}

//--------------------------------------------------------------
void ofApp::setInitialPattern() {
    
    frameElapsedTime = ofGetUnixTime();
    
    gen = 0;
    
    // load image
    //maskImg.loadImage("heart.png");
    
    if (++cntFrame >= source.getTotalNumFrames()) {
        ofExit();
    }
    
    
    source.nextFrame();
    cvColor.setFromPixels(source.getPixelsRef());
    maskGray.setFromColorImage(cvColor);
    
    
    // calucate initial seed arrangement
    contourFinder.findContours(maskGray, 5, (width*height), 10, true);
    
    
    // setup initial pattern
    blur.begin();
    
    ofBackground(0);
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    ofSetColor(0, 255, 0);
    maskGray.draw(0, 0);
    
    ofSetColor(255, 255, 0);
    
    for (int i = 0; i < contourFinder.nBlobs; i++) {
        ofxCvBlob blob = contourFinder.blobs[i];
        
        ofPath path;
        
        path.setFilled(false);
        path.setStrokeWidth(6);
        path.setStrokeColor(ofColor(255, 255, 0));
        path.moveTo(blob.pts[0]);
        
        for (int j = 0; j < blob.nPts; j++) {
            
            path.lineTo(blob.pts[j]);
            ofCircle(blob.pts[j], 3);
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
void ofApp::saveFrame() {
    
    ss.str("");
    ss << "phylo/" << exportName << "/"
    << exportName << "_" << setfill('0') << setw(6) << cntFrame << ".bmp";
    
    
    int elapsed = ofGetUnixTime() - frameElapsedTime;
    
    cout << "saved: " << ss.str() << " (" << elapsed << "s)" << endl;
    
    frameFbo = phylo.getBackBuffer();
    frameFbo->readToPixels(framePixels);
    
    exporter.setFromPixels(framePixels.getPixels(), width, height, OF_IMAGE_COLOR);
    exporter.saveImage(ss.str());
}

//--------------------------------------------------------------
void ofApp::update(){
    
    if (bNextFrame) {
        setInitialPattern();
        bNextFrame = false;
    }
    
    
    for (int i = 0; i < STEP; i++) {
        
        phylo << blur;
    }
    
    //lut << phylo;
    
    frameFbo = phylo.getBackBuffer();
    frameFbo->readToPixels(framePixels);
    cvColor.setFromPixels(framePixels.getPixels(), width, height);
    
    frameCnt->setFromColorImage(cvColor);
    
    frameDiff.absDiff(*framePrev, *frameCnt);
    frameDiff.threshold(frameDiffThreshold);
    
    contourFinder.findContours(frameDiff,
                               pow(frameDiffContourMin, 2),
                               width * height,
                               60,
                               false);
    
    swap(framePrev, frameCnt);
    
    gen += STEP;
    
    
    // check if finished
    
    if (contourFinder.nBlobs == 0) {
        
        bNextFrame = true;
        
        saveFrame();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(32);
    
    ofPushMatrix();
    
    blur.draw();
    ofSetColor(255);
    ofTranslate(width, 0);
    phylo.draw();
    ofTranslate(width, 0);
    frameDiff.draw(0, 0);
    contourFinder.draw();
    //lut.draw();
    ofPopMatrix();
    
    ss.str("");
    ss << "generation:" << gen;
    lblGen->setLabel(ss.str());
    
    ss.str("");
    ss << "progress: " << cntFrame << "/" << source.getTotalNumFrames();
    lblProgress->setLabel(ss.str());
    
    ss.str("");
    ss << "diff seed count:" << contourFinder.nBlobs;
    lblDiffSeedCount->setLabel(ss.str());
    
    mgDiffDetected->addPoint((float)contourFinder.nBlobs);
}

//--------------------------------------------------------------
void ofApp::exit(){
    
    if (gui)
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
