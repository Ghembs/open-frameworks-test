#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    camW = 320;
    camH = 240;

    cam.setup(camW, camH);

    colorImg.allocate(camW, camH);
    grayImg.allocate(camW, camH);
    grayBg.allocate(camW, camH);
    grayDiff.allocate(camW, camH);

    gui.setup();
    gui.add(overallSpeed.setup("Speed", 1, 0, 3));
    gui.add(noiseAmount.setup("Noise", 1, 0, 3));
    gui.add(trail.setup("Trail", 20, 0, 45));
    gui.add(lineLength.setup("Line", 50, 0, 150));
    gui.add(particles.setup("Particles", 150, 10, 600));
	gui.add(bLearnBackground.setup("capture bg", false));
	gui.add(threshold.setup("threshold", 80, 0, 255));
    gui.add(tide.setup("Tide", ofVec2f(0, 0), ofVec2f(-10, -10), ofVec2f(10, 10)));
    gui.add(colors.setup(ofParameter<ofColor> ("Color", 255, 0, 255)));

    ofSetBackgroundAuto(false);

    p.assign(particles, particle());

    for (int i = 0; i < p.size(); i++){
        p[i].setup();
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    ofSetFrameRate(60);

    bool bNewFrame = false;

    cam.update();

    bNewFrame = cam.isFrameNew();

    if (bNewFrame){
        colorImg.setFromPixels(cam.getPixels());
        grayImg = colorImg;

        if (bLearnBackground){
            grayBg = grayImg;
            bLearnBackground = false;
        }

        grayDiff.absDiff(grayBg, grayImg);
        grayDiff.threshold(threshold);

        contourFinder.findContours(grayDiff, 20, (camW * camH) * 0.9, 10, false);
    }
    
    vector<ofVec2f> blobPts;

    
    for (int i = 0; i < contourFinder.nBlobs; i++){
        for (int j = 0; j < contourFinder.blobs[i].pts.size(); j++){
            ofVec2f point;
            point.x = contourFinder.blobs[i].pts[j].x * 6;
            point.y = contourFinder.blobs[i].pts[j].y * 6;
            ofSetColor(0, 255, 0, 75);
            ofDrawCircle(point, 1);
            ofSetColor(255, 255, 255, 255);
            blobPts.push_back(point);
        }
    }

    if (p.size() != particles){
        p.assign(particles, particle());

        for (int i = 0; i < p.size(); i++){
            p[i].setup();
        }
    }

    
    for (int i = 0; i < p.size(); i++){
        //p[i].repel(blobPts);
        p[i].update(overallSpeed, noiseAmount, ofVec2f(tide->x, tide->y));
        //p[i].drawLines(blobPts, p, lineLength);
    }

    vector<ofVec2f> innerPts = findInnerPoints(p, blobPts);

    drawLines(innerPts);

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(0,0,0,trail);
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

    //ofSetColor(255, 255, 255, 255);
    ofSetColor(colors);

    for (int i = 0; i < p.size(); i++){
        p[i].draw(1);
        //p[i].drawLines(blobPts, p, lineLength);
    }

    // draw the incoming, the grayscale, the bg and the thresholded difference
	ofSetHexColor(0xffffff);
	//colorImg.draw(20, 20);
	//grayImg.draw(20+camW, 20);
	//grayBg.draw(20, 20+camH);
	//grayDiff.draw(20 + camW, 20 + camH);

	// then draw the contours:

	// ofFill();
	// ofSetHexColor(0x333333);
	// //ofDrawRectangle(20, 540, camW, camH);
	// ofSetHexColor(0xffffff);

	//contourFinder.draw(20 + camW, 20 + camH);

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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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

vector<ofVec2f> ofApp::findInnerPoints(vector<particle> pts, vector<ofVec2f> blobPts){
    vector<ofVec2f> innerPts;
    for (int i = 0; i < blobPts.size(); i++){
        int j = 0;
        auto itr = pts.begin();
        while (j < pts.size()){
            if (blobPts[i].distance(pts[j].pos) < 50){
                innerPts.push_back(pts[j].pos);
                pts.erase(itr+j);
            } else {
                j++;
            }
        }
    }

    return innerPts;
}

void ofApp::drawLines(vector<ofVec2f> blobPts)
{
    for (int i = 0; i < blobPts.size(); i++){
        for (int j = i + 1; j < blobPts.size(); j++){
            if (blobPts[i].distance(blobPts[j]) < lineLength){
                ofSetColor(255, 0, 0);
                ofSetLineWidth(1);
                ofDrawLine(blobPts[i], blobPts[j]);
                ofSetColor(255, 255, 255, 255);
            }
        }
    }
}
