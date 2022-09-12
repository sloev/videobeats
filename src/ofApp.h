#pragma once

#include "ofMain.h"
#include "ofxSyphon.h"
#include "ofxOsc.h"

#define PORT 12345

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
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);		

		void changeSpeedToBpm(int bpm);		
		float bpm;
		float skew;
		uint64_t nextBeatMS;
		ofxOscReceiver receiver;



		ofVideoPlayer 		fingerMovie;
		bool                frameByframe;

		ofTexture tex;
		
		ofxSyphonServer mainOutputSyphonServer;
};

