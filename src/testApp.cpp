#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
	ofLog(OF_LOG_NOTICE, "test setup");
	threshold = 32;

	camWidth = 640;
	camHeight = 480;

	cam.initGrabber(640, 480);
	cvImgColor.allocate(camWidth, camHeight);
	cvImgGrayscale.allocate(camWidth, camHeight);

	ofEnableBlendMode(OF_BLENDMODE_ALPHA);

	mouseX = 100;
	loading = false;
	ofRegisterURLNotification(this);

	maskLayers.allocate(camWidth, camHeight);
}

//--------------------------------------------------------------
void testApp::urlResponse(ofHttpResponse & response) {
	if (response.status == 200 && response.request.name == "tsingy_forest") {
		img.loadImage(response.data);
		loading = false;
	} else {
		cout << response.status << " " << response.error << endl;
		if (response.status != -1)
			loading = false;
	}
}

//--------------------------------------------------------------
void testApp::update() {
	ofSetLogLevel(OF_LOG_VERBOSE);

	cam.update();
	if (cam.isFrameNew()) {
		cam.update();
		cvImgColor.setFromPixels(cam.getPixels(), camWidth, camHeight);
		cvImgGrayscale.setFromColorImage(cvImgColor);
//		cvImgGrayscale.threshold(threshold, ofGetKeyPressed());
		cvImgGrayscale.threshold(threshold, false);
		contourFinder.findContours(cvImgGrayscale, 64 * 64,
				camWidth * camHeight, 5, false, true);

		polylines.clear();
		smoothed.clear();
		resampled.clear();
		boundingBoxes.clear();
		closestPoints.clear();
		closestIndices.clear();

		ofPoint target;
		target.set(mouseX, mouseY);

		for (unsigned int i = 0; i < contourFinder.blobs.size(); i++) {
			ofPolyline cur;
			// add all the current vertices to cur polyline
			cur.addVertices(contourFinder.blobs[i].pts);
			cur.setClosed(true);

			// add the cur polyline to all these vector<ofPolyline>
			polylines.push_back(cur);
			smoothed.push_back(cur.getSmoothed(8));
			resampled.push_back(cur.getResampledByCount(100).getSmoothed(8));

			boundingBoxes.push_back(cur.getBoundingBox());

			unsigned int index;
			closestPoints.push_back(
					resampled.back().getClosestPoint(target, &index));
			closestIndices.push_back(index);
		}
	}
	scaleWidth = (float) ofGetWidth() / (float) camWidth;
	scaleHeight = (float) ofGetHeight() / (float) camHeight;

//    maskLayers.setTexture(allUserMasks.getTextureReference(), 0);
//    maskLayers.begin(1);
//    ofPushMatrix();
//    ofScale(toKinectWidth, toKinectHeight, toKinectWidth);
//    drawParticlePositions();
//    ofPopMatrix();
//    maskLayers.end(1);
//    maskLayers.update();
}

//--------------------------------------------------------------
void drawWithNormals(const ofPolyline& polyline, int zeroX, int zeroY,
		bool drawContours) {
	for (int i = 0; i < (int) polyline.size(); i++) {
		bool repeatNext = i == (int) polyline.size() - 1;

		const ofPoint& cur = polyline[i];
		const ofPoint& next = repeatNext ? polyline[0] : polyline[i + 1];

		float angle = atan2f(next.y - cur.y, next.x - cur.x) * RAD_TO_DEG;
		float distance = cur.distance(next);

		if (repeatNext) {
			ofSetColor(255, 0, 255);
		}
		glPushMatrix();
		glTranslatef(cur.x + zeroX, cur.y + zeroY, 0);
		ofRotate(angle);
//		ofLine(0, 0, 0, distance);
		ofLine(0, 0, distance, 0);
		ofLine(0, distance, distance, distance);
		if (drawContours) {
			for (int i = distance; i < distance * 3; i += 5) {
				ofLine(0, 0, i, 0);
				ofLine(0, i, i, i);
			}
		}
		glPopMatrix();
	}
}

//--------------------------------------------------------------
void testApp::draw() {
	ofBackground(0);

	ofSetColor(255);
//	glPushMatrix();
//	glScalef(scaleWidth, scaleHeight, 0);
	cvImgGrayscale.draw(0, 0);

	for (unsigned int i = 0; i < polylines.size(); i++) {
		ofNoFill();

		ofSetColor(255, 0, 0);
		drawWithNormals(polylines[i], 0, 0, true);

		ofSetColor(0, 255, 0);
		drawWithNormals(smoothed[i], 0, 0, false);

		ofSetColor(0, 0, 255);
		drawWithNormals(resampled[i], 0, 0, true);

//		ofSetColor(0, 255, 255);
//		ofRect(boundingBoxes[i]);

//		ofSetColor(255, 0, 0);
//		ofLine(closestPoints[i], ofPoint(mouseX, mouseY));
		ofSetColor(0, 0, 255);
		ofLine(resampled[i][closestIndices[i]], ofPoint(mouseX, mouseY));
	}
//	glPopMatrix();

	ofSetColor(255, 0, 0);
	ofDrawBitmapString(ofToString((int) ofGetFrameRate()) + " fps", 10, 20);
	ofDrawBitmapString("Click and drag to set a new threshold.", 10, 40);
//	ofDrawBitmapString("Hold down any key to invert the thresholding.", 10, 60);

//---------------------------------------

//	ofDrawBitmapString("hit spacebar to load image from web", 10, ofGetHeight()/2);
//	if(loading)
//		ofDrawBitmapString("loading...", 10, ofGetHeight()+20);
//	float divider = ofMap( mouseX, 0, ofGetWidth(), 1, 48, true );
//
//	if(img.bAllocated()){
//		for(int y = 0; y < img.getHeight(); y+= divider){
//			for(int x = 0; x < img.getWidth(); x+=divider){
//				ofColor c = img.getColor(x, y);
//
//				ofSetColor( c.r, c.g, c.b );
//				ofCircle( x, y, divider/2 );
//			}
//		}
//
//		ofSetColor(255);
//		img.draw(img.getWidth(), 0);
//	}

//	maskLayers.draw(0, 0, maskLayers.getWidth(), maskLayers.getHeight());

	if (doVideoWrite) {
//		saveScreen.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
//		TIS.saveThreaded(saveScreen);
//		doVideoWrite != doVideoWrite;
	}

}

//--------------------------------------------------------------
void testApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void testApp::keyReleased(int key) {
	switch (key) {
	case '`':
		doVideoWrite = !doVideoWrite;
		saveScreen.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
		saveScreen.saveImage("ttt.jpg");
		TIS.saveThreaded(saveScreen);
		break;
	default:
//	img.clear();
//	ofLoadURLAsync(
//			"http://images.wildmadagascar.org/pictures/bemaraha/tsingy_forest.JPG",
//			"tsingy_forest");
//	loading = true;
		break;
	}

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button) {
	threshold = x;

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo) {

}

void testApp::clearLayer(int i) {
	maskLayers.begin(i);
	ofBackground(0, 0, 0);
	maskLayers.end(i);
	maskLayers.update();
}
