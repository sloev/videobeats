#pragma once

#include "ofMain.h"
#include "ofxSyphon.h"
#include "ofxOsc.h"
#include "ofxGui.h"

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

		void ChangedOSCPort(string &port);
		void ChangedSyphonName(string &s);
		float bpm;
		float skew;
		uint64_t nextBeatMS;
		ofxOscReceiver receiver;



		ofVideoPlayer 		player;
		
		ofxSyphonServer mainOutputSyphonServer;
		
			ofxPanel gui;
	ofParameter<std::string> oscPort;
	ofParameter<bool> preview;
	ofParameter<std::string> syphonName;

};

