#pragma once

#include "ofMain.h"
#include "ofxUI.h"
#include "ofxFXObject.h"
#include "ofxBokeh.h"

#include "Phyllotaxis.h"

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
    void exit();
    
    void reset();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    
    ofxUISuperCanvas *gui;
    
    //ofxGrayScott gray;
    ofxBokeh    bokeh;
    Phyllotaxis phylo;
    ofxFXObject color;
    
    float f, k;
};
