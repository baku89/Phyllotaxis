#pragma once

#include "ofMain.h"
#include "ofxUI.h"
#include "ofxFXObject.h"
#include "ofxGaussianBlur.h"
#include "ofxBokeh.h"
#include "ofxLUT.h"
#include "ofxCvFloatImage.h"

#include "Phyllotaxis.h"

#define WIDTH 256
#define HEIGHT 256
#define SIZE 16384
#define STEP 512
#define FPS 24

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
    void exit();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    void setInitialPattern();
    
    int                     gen;
    stringstream            ss;
    
    ofxUISuperCanvas        *gui;
    ofxUILabel              *lblGen;
    
    ofxGaussianBlur         blur;
    Phyllotaxis             phylo;
    ofxLUT                  lut;
    
    ofImage maskImg;
    ofImage exporter;
    
    ofFbo                   *frameFbo;
    ofPixels                framePixels;
    
    // cv
    ofxCvColorImage         maskColor;
    ofxCvGrayscaleImage     maskGray;
    ofxCvGrayscaleImage     *framePrev;
    ofxCvGrayscaleImage     *frameCnt;
    ofxCvGrayscaleImage     frameDiff;
    
    ofxCvContourFinder      contourFinder;
};
