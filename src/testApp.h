#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofURLFileLoader.h"
#include "ofxFX.h"
#include "ofxThreadedImageSaver.h"

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
		
		ofVideoGrabber cam;
		ofxCvColorImage cvImgColor;
		ofxCvGrayscaleImage cvImgGrayscale;
		ofxCvContourFinder contourFinder;
		vector<ofPolyline> polylines, smoothed, resampled;
		vector<ofRectangle> boundingBoxes;
		vector<ofPoint> closestPoints;
		vector<unsigned int> closestIndices;
		int camWidth, camHeight;
		float scaleWidth, scaleHeight;

		int threshold;

		void urlResponse(ofHttpResponse & response);

		ofImage img;
		bool loading;

		//----------------- visuals

		int width;
		int height;
	    bool toggle;
	    int counter;
	    ofxMask maskLayers;
		ofImage bigMask;
		void clearLayer(int i);

	    bool doVideoWrite;
	    ofImage saveScreen;
	    ofxThreadedImageSaver TIS;

};
