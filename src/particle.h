#include "ofMain.h"

class particle
{
    public:
        particle();

        void setup();
        void update(float speed, float noise, ofVec2f tide);
        void draw(float sizeDot);
        void drawLines(vector<ofVec2f> blobPts, vector<particle> pts, float lineLength);
	    void repel(vector<ofVec2f>repelPt);

        ofVec2f pos;
        ofVec2f vel;
        ofVec2f frc;
        ofVec2f uniqueVal;
	    ofVec2f repelFrc;

        float drag;
        float size;
};