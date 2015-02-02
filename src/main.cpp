#include "ofMain.h"
#include "ofApp.h"

string fileName;

//========================================================================
int main(int argc, const char * argv[]){
	ofSetupOpenGL(1024,768,OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
    
    vector<string> args;
    
    for (int i = 0; i < argc - 1; i++) {
        cout << ofToString(argv[i]) << endl;
        if (ofToString(argv[i]) == "--arguments") {
            
            for (i++; i < argc; i++) {
                args.push_back(ofToString(argv[i]));
            }
        }
    }
    
	ofRunApp(new ofApp(args));

}
