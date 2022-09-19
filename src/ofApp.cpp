#include "ofApp.h"
#include <chrono>

const int width = 300;
const int height = 250;
//--------------------------------------------------------------
void ofApp::setup()
{
    ofSetBackgroundAuto(false);
    ofBackground(0, 0, 0);
    ofSetVerticalSync(true);
    ofSetWindowTitle("videobeats");

    oscPort.addListener(this, &ofApp::ChangedOSCPort);
    syphonName.addListener(this, &ofApp::ChangedSyphonName);

    // Uncomment this to show movies with alpha channels
    // player.setPixelFormat(OF_PIXELS_RGBA);
    mainOutputSyphonServer.setName(syphonName.get());

    bpm = 60;
    int now = ofGetSystemTimeMillis();
    nextBeatMS = now + (60 * 1000 / bpm);
  
    receiver.setup(PORT);
    gui.setup("videobeats");
    gui.add(oscPort.set("OSC port: ", "12345"));

    gui.add(syphonName.set("Syphon name: ", "videobeats"));
    ofSetFrameRate(30);
}

uint64_t timeSinceEpochMillisec()
{
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}
//--------------------------------------------------------------
void ofApp::update()
{
    if (player.isPlaying())
    {

        player.update();
        mainOutputSyphonServer.publishTexture(&player.getTexture());
    }
    uint64_t now = timeSinceEpochMillisec();
    if (now >= nextBeatMS)
    {
        if (player.isPlaying())
        {

            float position = player.getPosition() * player.getDuration();

            float nextposition = floor(position + 1.0);
            skew = 1.0;
            if (nextposition > position)
            {
                skew = 1.0 + (nextposition - position) / 5.0;
            }
            if (nextposition < position)
            {
                skew = 1.0 + (position - nextposition) / 5.0;
            }

            player.setSpeed((bpm / 60.0) * skew);
            gui.setName("videobeats - " + ofToString(bpm) + "bpm");
        }

        nextBeatMS = now + (60.0 * 1000.0 / bpm);
    }

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
        }
    }
}

void ofApp::ChangedOSCPort(string & port)
{
    receiver.setup(ofToInt(port));
}

void ofApp::ChangedSyphonName(string &s)
{
    mainOutputSyphonServer.setName(s);
    ofSetWindowTitle("videobeats - " + s);
}

//--------------------------------------------------------------
void ofApp::draw()
{
    if (ofGetFrameNum() % 6 == 0)
    {
        ofClear(0, 0, 0, 1);

        if (player.isPlaying())
        {
            player.draw(10, 10, width - 20, height - 20);
        }
    }
    gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
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
    if (dragInfo.files.size() > 0)
    {
        player.load(dragInfo.files[0]);
        player.setLoopState(OF_LOOP_NORMAL);
        player.play();
    }
}