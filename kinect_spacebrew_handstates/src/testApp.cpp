#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){    
    // setup message queue
    
    font.loadFont("myriad.ttf", 20);
    
    ofBackground(0);
    ofSetFrameRate(60);
    bSendImage = false;

	// kinect setup
	kinect.initSensor();
	//kinect.initColorStream(true);
	kinect.initIRStream();
	kinect.initDepthStream(true);
	kinect.initBodyIndexStream();
	kinect.initSkeletonStream(true);

	//simple start
	kinect.start();
	ofDisableAlphaBlending(); //Kinect alpha channel is default 0;

	string host = "localhost";
	string name = "of-handstate";
	string description = "KinectV2 Hand States";
	spacebrew.connect( host, name, description );

	spacebrew.addPublish("state", Spacebrew::TYPE_RANGE);
}

//--------------------------------------------------------------
void testApp::update(){
	kinect.update();
}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(0, 255);

	kinect.drawAllSkeletons(ofVec2f(640,480));
	//if(kinect.getSkeletons().size() > 0) {
	//	ofLog() << kinect.getSkeletons().size() << ":" << kinect.getSkeletons().back().leftHandState;
	//}
	for (int i = 0; i < kinect.getSkeletons().size(); i++)
	{
		if (kinect.getSkeletons()[i].tracked)
		{
			//drawSkeleton(i, scale);
			ofLog() << kinect.getSkeletons().size() << ":" << kinect.getSkeletons()[i].leftHandState;
			spacebrew.sendRange("state", kinect.getSkeletons()[i].leftHandState);
		}
	}
}

void testApp::onMessage(Spacebrew::Message &m) {
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 
}