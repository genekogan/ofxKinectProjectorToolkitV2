#pragma once

#include "ofMain.h"
#include "ofxMultiKinectV2.h"
#include "ofxKinectProjectorToolkitV2.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"
#include "ofxSecondWindow.h"
#include "ofxGui.h"
#include "ofxFastFboReader.h"

// this must match the display resolution of your projector
#define PROJECTOR_RESOLUTION_X 1280
#define PROJECTOR_RESOLUTION_Y 800

using namespace ofxCv;
using namespace cv;


class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    ofxKinectProjectorToolkitV2 kpt;
    ofxMultiKinectV2 kinect;

    ofxSecondWindow projector;

    ofShader maskShader;
    ofTexture depthTex;
    ofxFastFboReader fboReader;
    ofPixels fboPixels;
    ofFbo fboMask;
    ofxCv::ContourFinder contourFinder;
    ofxCvColorImage colorImage;
    ofxCvGrayscaleImage grayImage;
    ofxCvGrayscaleImage grayThreshNear;
    ofxCvGrayscaleImage grayThreshFar;
    
    ofParameter<float> nearThreshold;
    ofParameter<float> farThreshold;
    ofParameter<float> minArea;
    ofParameter<float> maxArea;
    ofParameter<float> threshold;
    ofParameter<float> persistence;
    ofParameter<float> maxDistance;
    
    ofxPanel gui;
    
    ofColor blobColors[12];
};
