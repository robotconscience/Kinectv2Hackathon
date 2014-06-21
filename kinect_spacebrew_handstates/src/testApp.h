#pragma once

#include "ofMain.h"

#include "ofxKinectCommonBridge.h"
//#include "ofxLibwebsockets.h"
#include "ofxSpacebrew.h"

#define NUM_MESSAGES 20 // how many past messages we want to keep

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		// KINECT stuff
		
		ofxKinectCommonBridge kinect;
		ofShader yuvRGBShader;
		ofPlanePrimitive plane;

        //ofxLibwebsockets::Server server;
		Spacebrew::Connection spacebrew;

        //queue of rec'd messages
        ofTrueTypeFont font;
        vector<string> messages;
    
        string  toLoad;
        bool    bSendImage;
        ofImage currentImage;


		void onMessage(Spacebrew::Message &m);
};
