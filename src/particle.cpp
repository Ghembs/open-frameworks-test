#include "particle.h"

particle::particle()
{
    
}

void particle::setup()
{
    pos.x = ofRandomWidth();
    pos.y = ofRandomHeight();

    vel.x = ofRandom(-4, 4);
    vel.y = ofRandom(-4, 4);

    frc = ofVec2f(0, 0);

    uniqueVal.x = ofRandom(-1000, 1000);
    uniqueVal.y = ofRandom(-1000, 1000);

    size = ofRandom(3, 5);

    drag = ofRandom(0.97, 0.99);
}

void particle::update(float speed, float noise, ofVec2f tide)
{
    frc.x = ofSignedNoise(uniqueVal.x, ofGetElapsedTimeMillis());
    frc.y = ofSignedNoise(uniqueVal.y, ofGetElapsedTimeMillis());

    frc *= noise;

    vel *= drag;
    vel += frc;
    vel += tide * 0.01;
    vel -= repelFrc;
    pos += vel*speed;

    // RESET AT BORDER
    if (pos.x + vel.x > ofGetWidth()) {
        pos.x -= ofGetWidth();
    }

    if (pos.x + vel.x < 0) {
        pos.x += ofGetWidth();
    }

    if (pos.y + vel.y > ofGetHeight()) {
        pos.y -= ofGetHeight();
    }

    if (pos.y + vel.y < 0) {
        pos.y += ofGetHeight();
    }
}

void particle::draw(float sizeDot)
{
    ofDrawCircle(pos.x, pos.y, size * sizeDot);
}

void particle::drawLines(vector<ofVec2f> blobPts, vector<particle> pts, float lineLength)
{   
    for (int i = 0; i < blobPts.size(); i++){
        if (pos.distance(blobPts[i]) < 50){
            for (int j = 0; j < pts.size(); j++){
                if (pos.distance(pts[j].pos) < lineLength){
                    ofSetColor(255, 0, 0);
                    ofSetLineWidth(1);
                    ofDrawLine(pos, pts[j].pos);
                    ofSetColor(255, 255, 255, 255);
                }
            }
        }
    }
}

void particle::repel(vector<ofVec2f> repelPt)
{
	repelFrc.set(0, 0);

	for (int i = 0; i < repelPt.size(); i++) {
	
		float dist = pos.distance(repelPt[i]);

		if (dist < 200) {
			ofVec2f newRepelPt;
			newRepelPt = repelPt[i] - pos;
			repelFrc += newRepelPt * 0.00001;
		}
	}
}