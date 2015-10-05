#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowShape(1280, 800);
    ofSetVerticalSync(true);
    
    // setup projector window
    projector.setup("projector", ofGetScreenWidth(), 0, 1280, 800, true);
    
    // load masking shader to separate out contours
    maskShader.setupShaderFromSource(GL_FRAGMENT_SHADER, maskFragmentShader);
    maskShader.linkProgram();
    fboMask.allocate(512, 424, GL_RGBA);
    fboPixels.allocate(512, 424, 1);

    // start kinectV2
    kinect.open(true, true, 0);   // kinect1.open(true, true, 0, 2); // GeForce on MacBookPro Retina
    kinect.start();
    kinect.setEnableFlipBuffer(true);
    
    // load calibration file
    kpt.loadCalibration("calibration.xml");
    
    // setup the gui
    gui.setup("parameters");
    gui.add(nearThreshold.set("nearThresh", 230, 0, 255));
    gui.add(farThreshold.set("farThresh", 10, 0, 255));
    gui.add(minArea.set("minArea", 1000, 0, 5000));
    gui.add(maxArea.set("maxArea", 70000, 15000, 150000));
    gui.add(threshold.set("threshold", 15, 1, 100));
    gui.add(persistence.set("persistence", 15, 1, 100));
    gui.add(maxDistance.set("maxDistance", 32, 1, 100));
    
    // colors for differentiating found contours
    blobColors[0] = ofColor(255, 0, 0);
    blobColors[1] = ofColor(0, 255, 0);
    blobColors[2] = ofColor(0, 0, 255);
    blobColors[3] = ofColor(255, 255, 0);
    blobColors[4] = ofColor(255, 0, 255);
    blobColors[5] = ofColor(0, 255, 255);
    blobColors[6] = ofColor(255, 127, 0);
    blobColors[7] = ofColor(127, 0, 255);
    blobColors[8] = ofColor(0, 255, 127);
    blobColors[9] = ofColor(127, 255, 0);
    blobColors[10]= ofColor(255, 0, 127);
    blobColors[11]= ofColor(0, 127, 255);
}

//--------------------------------------------------------------
void ofApp::update(){
    kinect.update();
    if (kinect.isFrameNew())
    {
        // get depth pixels
        depthTex.loadData(kinect.getDepthPixelsRef());
        depthTex.setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
        
        // thrshold the depth image and mask out everything outside of threshold range... this is ugly.
        // the maskShader needs to be used to get the depth texture pixels to
        // 0->255 range, so ofFbo -> ofPixels -> ofxCvColorImage -> ofxCvGrayscaleImage.
        // there must must be a better way to do this?
        fboMask.begin();
        ofClear(0, 0);
        maskShader.begin();
        depthTex.draw(0, 0, 512, 424);
        maskShader.end();
        fboMask.end();
        fboReader.readToPixels(fboMask, fboPixels);
        colorImage.setFromPixels(fboPixels);
        grayImage.setFromColorImage(colorImage);
        grayThreshNear = grayImage;
        grayThreshFar = grayImage;
        grayThreshNear.threshold(nearThreshold, true);
        grayThreshFar.threshold(farThreshold);
        cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), grayImage.getCvImage(), NULL);
        grayImage.flagImageChanged();
        
        // set contour tracker parameters
        contourFinder.setMinArea(minArea);
        contourFinder.setMaxArea(maxArea);
        contourFinder.setThreshold(threshold);
        contourFinder.getTracker().setPersistence(persistence);
        contourFinder.getTracker().setMaximumDistance(maxDistance);
        
        // look for contours
        contourFinder.findContours(grayImage);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    
    // draw the gui
    ofSetColor(255);
    fboMask.draw(0, 0);
    ofTranslate(512, 0);
    grayImage.draw(0, 0);
    contourFinder.draw();
    ofTranslate(-512, 0);

    // draw projected contours into projector window
    projector.begin();
    
    ofBackground(0);
    
    RectTracker& tracker = contourFinder.getTracker();
    
    for(int i = 0; i < contourFinder.size(); i++) {
        // get contour, label, center point, and age of contour
        vector<cv::Point> points = contourFinder.getContour(i);
        int label = contourFinder.getLabel(i);
        ofPoint center = toOf(contourFinder.getCenter(i));
        int age = tracker.getAge(label);
        
        // map contour using calibration and draw to main window
        ofBeginShape();
        ofFill();
        ofSetColor(blobColors[label % 12]);
        for (int j=0; j<points.size(); j++) {
            ofVec3f worldPoint = kinect.getWorldCoordinateAt(points[j].x, points[j].y);
            if (worldPoint.z == 0) continue;
            ofVec2f projectedPoint = kpt.getProjectedPoint(worldPoint);
            ofVertex(projector.getWidth() * projectedPoint.x, projector.getHeight() * projectedPoint.y);
        }
        ofEndShape();
    }
    
    projector.end();
    
    gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
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
