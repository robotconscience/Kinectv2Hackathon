#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    // setup a server with default options on port 9092
    // - pass in true after port to set up with SSL
    //bool connected = server.setup( 9092 );
    
    // Uncomment this to set up a server with a protocol
    // Right now, clients created via libwebsockets that are connecting to servers
    // made via libwebsockets seem to want a protocol. Hopefully this gets fixed, 
    // but until now you have to do something like this:
    
    ofxLibwebsockets::ServerOptions options = ofxLibwebsockets::defaultServerOptions();
    options.port = 9093;
    options.protocol = "of-protocol";
    options.bBinaryProtocol = true;
    
    bool connected = server.setup( options );
    
    // this adds your app as a listener for the server
    server.addListener(this);
    
    // setup message queue
    
    font.loadFont("myriad.ttf", 20);
    messages.push_back("WebSocket server setup at "+ofToString( server.getPort() ) + ( server.usingSSL() ? " with SSL" : " without SSL") );
    
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
	kinect.setRawTextureUsesFloats(true);

	kinect.setDepthClipping(200,1000);

	//simple start
	kinect.start();
	ofDisableAlphaBlending(); //Kinect alpha channel is default 0;

	mesh.setup(512, 424, kinect);
}

//--------------------------------------------------------------
void testApp::update(){
	kinect.update();


}

void meshToBuffer( ofMesh & m, string & buff ){
	buff = ( ofToString(m.getNumVertices()) + ";" );
	for ( auto & v : m.getVertices() ){
		buff += ofToString(v.x) + ",";
		buff += ofToString(v.y) + ",";
		buff += ofToString(v.z) + ",";
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(0, 255);

    int x = font.getSize();
    int y = font.getSize()*2;
    ofSetColor(255);
    for (int i = messages.size() -1; i >= 0; i-- ){
        //font.drawString( messages[i], x, y );
        y += font.stringHeight( messages[i] ) + font.getSize();
    }
    if (currentImage.bAllocated()) currentImage.draw(0,0);
    ofDrawBitmapString("Drag an image onto the window to send!\nOpen your browser to localhost:9093 to receive", 20,20);
	
	string buff;
	ofMesh m = mesh.getMeshCutoff();
	meshToBuffer(m, buff);
	server.send( buff );
	
	kinect.draw(0,0);
	mesh.draw();

	//kinect v2 outputs depth/ir resolution of 512x424
	//kinect.drawDepth(0, ofGetHeight() - 424);
	//kinect.drawBodyIndex(512, ofGetHeight() - 424);
	//kinect.drawAllSkeletons(ofVec2f(640,480));
}

//--------------------------------------------------------------
void testApp::onConnect( ofxLibwebsockets::Event& args ){
    cout<<"on connected"<<endl;
}

//--------------------------------------------------------------
void testApp::onOpen( ofxLibwebsockets::Event& args ){
    cout<<"new connection open"<<endl;
    messages.push_back("New connection from " + args.conn.getClientIP() );
    
    // send the latest image if there is one!
    if ( currentImage.bAllocated() ){

    }
}

//--------------------------------------------------------------
void testApp::onClose( ofxLibwebsockets::Event& args ){
    cout<<"on close"<<endl;
    messages.push_back("Connection closed");
}

//--------------------------------------------------------------
void testApp::onIdle( ofxLibwebsockets::Event& args ){
    cout<<"on idle"<<endl;
}

//--------------------------------------------------------------
void testApp::onMessage( ofxLibwebsockets::Event& args ){
    cout<<"got message "<<args.message<<endl;
    
    // trace out string messages or JSON messages!
    // args.json is null if badly formed or just not JOSN
    if ( !args.json.isNull() ){
        messages.push_back("New message: " + args.json.toStyledString() + " from " + args.conn.getClientName() );
    } else {
        messages.push_back("New message: " + args.message + " from " + args.conn.getClientName() );
    }
}

//--------------------------------------------------------------
void testApp::onBroadcast( ofxLibwebsockets::Event& args ){
    cout<<"got broadcast "<<args.message<<endl;    
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
    ofLaunchBrowser("http://localhost:9093");
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
    for (int i=0; i<dragInfo.files.size(); i++){
        string file = dragInfo.files[i];
        ofFile f(file); 
        string ex = f.getExtension();
        std::transform(ex.begin(), ex.end(),ex.begin(), ::toupper);
        
        if ( ex == "JPG" || ex == "JPEG" || ex == "PNG" || ex == "GIF" ){
            toLoad = file;
            bSendImage = true;
        }
    }
}