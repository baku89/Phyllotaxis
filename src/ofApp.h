#pragma once

#include "ofMain.h"
#include "ofxUI.h"
#include "ofxFXObject.h"
#include "ofxGaussianBlur.h"
#include "ofxBokeh.h"
#include "ofxLUT.h"
#include "ofxCvFloatImage.h"

#include "Phyllotaxis.h"

//#define WIDTH 512
//#define HEIGHT 512
//#define SIZE 16384
#define STEP 1024
#define FPS 6

#define DATA_DIR "/Volumes/MugiRAID1/Works/2015/01_fasciation/of/"

class ofApp : public ofBaseApp{
    
public:
    ofApp(vector<string> args);
    
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
    void saveFrame();
    
    string                  fileName;
    
    int                     width, height;
    
    int                     gen;
    float                   frameDiffThreshold;
    float                   frameDiffContourMin;
    float                   frameDiffContourMax;
    
    int                     frameElapsedTime;
    
    int                     cntFrame;
    
    bool                    bNextFrame;
    
    string                  exportName;
    
    stringstream            ss;
    
    ofVideoPlayer           source;
    
    ofxUISuperCanvas        *gui;
    ofxUILabel              *lblGen;
    ofxUILabel              *lblProgress;
    ofxUILabel              *lblDiffSeedCount;
    ofxUIMovingGraph        *mgDiffDetected;
    
    ofxGaussianBlur         blur;
    Phyllotaxis             phylo;
    ofxLUT                  lut;
    
    ofImage maskImg;
    ofImage exporter;
    
    ofFbo                   *frameFbo;
    ofPixels                framePixels;
    
    // cv
    ofxCvColorImage         cvColor;
    ofxCvGrayscaleImage     maskGray;
    ofxCvGrayscaleImage     *framePrev;
    ofxCvGrayscaleImage     *frameCnt;
    ofxCvGrayscaleImage     frameDiff;
    
    ofxCvContourFinder      contourFinder;
};
