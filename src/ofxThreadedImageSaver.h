#ifndef OFXTHREADEDIMAGESAVER_H
#define OFXTHREADEDIMAGESAVER_H


#include "ofMain.h"
#include "ofThread.h"

class ofxThreadedImageSaver : public ofThread, public ofImage
{
public:
    string fileName;
    int counter;
    ofImage myImage;

    ofxThreadedImageSaver() {
        counter=0;
    }

    void threadedFunction() {
        if(lock()) {
            myImage.saveImage(fileName);
            unlock();
        } else {
            printf("ofxThreadedImageSaver - cannot save %s cos I'm locked", fileName.c_str());
        }
        stopThread();
    }
    void saveThreaded(ofImage imageToSave) {
        char fileName[255];
        sprintf(fileName, "snapshot_%0.3i.png", counter);
        counter++;
        this->fileName = fileName;
        this->myImage = imageToSave;
        startThread(false, false);
    }
};

#endif // OFXTHREADEDIMAGESAVER_H
