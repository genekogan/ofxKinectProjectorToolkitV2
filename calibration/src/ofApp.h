#pragma once

#include "ofMain.h"
#include "ofxMultiKinectV2.h"
#include "ofxKinectProjectorToolkitV2.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"
#include "ofxSecondWindow.h"


// this must match the display resolution of your projector
#define PROJECTOR_RESOLUTION_X 1280
#define PROJECTOR_RESOLUTION_Y 800


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

    void drawChessboard(int x, int y, int chessboardSize);
    void drawTestingPoint(ofVec2f projectedPoint);
    void addPointPair();
    
    ofxKinectProjectorToolkitV2 kpt;
    ofxMultiKinectV2 kinect;
    ofxSecondWindow secondWindow;

    ofShader depthShader;
    ofTexture colorTex;
    ofTexture depthTex;
    GpuRegistration gr;
    ofFbo fboChessboard;
    ofxCvColorImage rgbImage;
    cv::Mat cvRgbImage;
    ofPixels pix;
    ofFbo fbo;

    vector<ofVec2f> currentProjectorPoints;
    vector<cv::Point2f> cvPoints;
    vector<ofVec3f> pairsKinect;
    vector<ofVec2f> pairsProjector;
    
    string resultMessage;
    ofColor resultMessageColor;
    ofVec2f testPoint;
    
    int chessboardSize;
    int chessboardX;
    int chessboardY;
    bool searching;
    bool testing;
    bool saved;
};
