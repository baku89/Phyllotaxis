#pragma once

#include "ofMain.h"
#include "ofxUI.h"
#include "ofxFXObject.h"
#include "ofxGaussianBlur.h"
#include "ofxCvFloatImage.h"

#include "Phyllotaxis.h"

#define WIDTH 128
#define HEIGHT 128
#define SIZE 16384
#define STEP 1

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
    
    int gen;
    stringstream ss;
    
    
    ofxUISuperCanvas *gui;
    ofxUILabel *lblGen;
    
    //ofxGrayScott gray;
    //ofxGaussianBlur blur;
    Phyllotaxis phylo;
    //ofxFXObject color;
    
    //float pixels[SIZE];
};
