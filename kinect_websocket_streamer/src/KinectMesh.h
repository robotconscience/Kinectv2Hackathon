#pragma once

#include "ofMain.h"

class KinectMesh : public ofThread {
public:

	~KinectMesh(){
		waitForThread(true);
	}

	void setup(int w, int h, ofxKinectCommonBridge & _kinect ){
		width = w;
		height = h;
		kinect = &_kinect;
		meshStep = 4.0;
		buildMesh();
		startThread();
	}

	ofMesh getMeshCutoff(){
		return getMeshCutoff(zmin, zmax);
	}

	ofMesh getMeshCutoff( float minVal, float maxVal ){
		ofMesh ret;
		for ( auto & v : meshDraw.getVertices() ){
			if ( v.z >= minVal && v.z <= maxVal ){
				ret.addVertex(v);
			}
		}
		
		return ret;
	}

	void threadedFunction(){
		while(isThreadRunning()){
			if ( kinect->isFrameNewDepth() ){
				vector<ofPoint> pts; 
				for(int y = 0; y < height; y+=meshStep){
					for(int x = 0; x < width; x+=meshStep){
						pts.push_back(ofPoint(x,y));
					}
				}
				int i=0;
				static ofVec3f flip(1000,-1000,-1000);
				for ( auto & v : mesh.getVertices() ){
					v.set( kinect->mapDepthToSkeleton( pts[i] ));
					mesh.setVertex( i, v * flip );
					if ( v.z > -500 ){
						mesh.setColor(i, ofFloatColor(0,0,0));
					} else {
						mesh.setColor(i, ofFloatColor(1));
					}
					i++;
				}
				bNeedToUpdate = true;
			}
			yield();
		}
	}

	void draw(){
		zmin = -600;//ofMap(sin(ofGetElapsedTimeMillis() * .001), -1., 1.0, -800, -500);
		zmax = zmin + 100;
		if ( bNeedToUpdate ){
			meshDraw = ofMesh(mesh);
			bNeedToUpdate = false;
		}
		ofPushMatrix();
		ofTranslate(ofGetWidth()/2.0, ofGetHeight()/2.0);
		ofScale(4.0,4.0,4.0);
		getMeshCutoff(zmin, zmax).drawVertices();
		ofPopMatrix();
		bWantToUpdate = true;
	}

protected:
	bool bNeedToUpdate, bWantToUpdate;
	ofxKinectCommonBridge * kinect;
	float width, height;
	float zmin, zmax;

	float meshStep;

	void buildMesh(){
        float z;
        float rowCount = width / meshStep;
		float columnCount = height / meshStep;
		for(int y = 0; y < height; y+=meshStep){
			for(int x = 0; x < width; x+=meshStep){
                z = 0;
                    
                // vbo
                // add colour, vertex and texture coord
                mesh.addColor(ofFloatColor(1,1,1,1));
                mesh.addVertex(ofVec3f(x, y, z));
                    
                //mesh.addTexCoord(ofVec2f( col/(columnCount-1), row/(rowCount-1)) );
            }
        }
            
        int n = 0;
        int colSteps = columnCount * 2.0;
        int rowSteps = rowCount - 1;
        vector<ofIndexType> indices;
            
        for ( int r = 0; r < rowSteps; r++ ) {
            for ( int c = 0; c < colSteps; c++ ) {
                int t = c + r * colSteps;
                if ( c == colSteps - 1 ) {
                    indices.push_back( n );
                } else {
                    indices.push_back( n );
                    ( t%2 == 0 ) ? n += columnCount : (r%2 == 0) ? n -= (columnCount-1) : n -= (columnCount+1);
                }  
            }  
        }
        mesh.addIndices(indices);
		mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
	}

	ofMesh mesh, meshDraw;
};