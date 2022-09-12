#include "ofApp.h"
#include <chrono>


//--------------------------------------------------------------
void ofApp::setup()
{
    ofBackground(255, 255, 255);
    ofSetVerticalSync(true);
    frameByframe = false;

    // Uncomment this to show movies with alpha channels
    // fingerMovie.setPixelFormat(OF_PIXELS_RGBA);
    mainOutputSyphonServer.setName("Screen Output");
    tex.allocate(200, 100, GL_RGBA);

    fingerMovie.load("movies/onesecond.mp4");
    fingerMovie.setLoopState(OF_LOOP_NORMAL);
    fingerMovie.play();
    bpm = 60;
    int now = ofGetSystemTimeMillis();
    nextBeatMS = now + (60 * 1000 / bpm);
    ofLog(OF_LOG_NOTICE, "now is " + ofToString(now));

    ofLog(OF_LOG_NOTICE, "nextBeatMS is " + ofToString(nextBeatMS));
    receiver.setup(PORT);
}

//--------------------------------------------------------------
void ofApp::update()
{
    fingerMovie.update();
    while (receiver.hasWaitingMessages())
    {
        // get the next message
        ofxOscMessage m;
        receiver.getNextMessage(m);

        // check for mouse moved message
        if (m.getAddress() == "/beatape")
        {
            bpm = m.getArgAsFloat(0);
            nextBeatMS = m.getArgAsInt64(1);

            ofLog(OF_LOG_NOTICE, "got osc bpm " + ofToString(bpm, 2) + "type" + m.getArgTypeName(0));
            ofLog(OF_LOG_NOTICE, "got osc nextBeatMS " + ofToString(nextBeatMS, 2));
        }
    }
}

uint64_t timeSinceEpochMillisec() {
  using namespace std::chrono;
  return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

//--------------------------------------------------------------
void ofApp::draw()
{

    ofSetHexColor(0xFFFFFF);

    fingerMovie.draw(20, 20);
    tex = fingerMovie.getTexture();

    ofDrawBitmapString("frame: " + ofToString(fingerMovie.getCurrentFrame()) + "/" + ofToString(fingerMovie.getTotalNumFrames()), 20, 380);
    ofDrawBitmapString("duration: " + ofToString(fingerMovie.getPosition() * fingerMovie.getDuration(), 2) + "/" + ofToString(fingerMovie.getDuration(), 2), 20, 400);
    ofDrawBitmapString("speed: " + ofToString(fingerMovie.getSpeed(), 2), 20, 420);
    ofDrawBitmapString("bpm: " + ofToString(bpm, 2), 20, 440);
    ofDrawBitmapString("skew: " + ofToString(skew, 2), 20, 480);

    uint64_t now = timeSinceEpochMillisec();
    if (now >= nextBeatMS)
    {
        ofLog(OF_LOG_NOTICE, "beat ! bpm is  " + ofToString(bpm));

        float position = fingerMovie.getPosition() * fingerMovie.getDuration();

        float nextposition = floor(position + 1.0);
        skew = 1.0;
        if(nextposition > position){
            skew = 1.0 + (nextposition-position)/5.0;
        }
        if(nextposition < position){
            skew = 1.0 + (position-nextposition)/5.0;
        }

        if (nextposition > fingerMovie.getDuration())
        {
            nextposition - fingerMovie.getDuration();
        }

        // fingerMovie.setPosition(nextposition);

        fingerMovie.setSpeed((bpm / 60.0)*skew);

        nextBeatMS = now + (60.0 * 1000.0 / bpm);
    }

    if (fingerMovie.getIsMovieDone())
    {
        ofSetHexColor(0xFF0000);
        ofDrawBitmapString("end of movie", 20, 440);
    }

    mainOutputSyphonServer.publishTexture(&tex);
}
void ofApp::changeSpeedToBpm(int bpm)
{
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    switch (key)
    {
    case 'f':
        frameByframe = !frameByframe;
        fingerMovie.setPaused(frameByframe);
        break;
    case OF_KEY_LEFT:
        bpm -= 1;
        break;
    case OF_KEY_RIGHT:
        bpm += 1;
        break;
    case '0':
        fingerMovie.firstFrame();
        break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y)
{
    if (!frameByframe)
    {
        int width = ofGetWidth();
        float pct = (float)x / (float)width;
        float speed = (2 * pct - 1) * 5.0f;
    }
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
    if (!frameByframe)
    {
        int width = ofGetWidth();
        float pct = (float)x / (float)width;
        fingerMovie.setPosition(pct);
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
    if (!frameByframe)
    {
        fingerMovie.setPaused(true);
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
    if (!frameByframe)
    {
        fingerMovie.setPaused(false);
    }
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{
}
