#pragma once

#include "ofMain.h"
#include <iostream>
#include "ofxGui.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"

using namespace cv;
using namespace ofxCv;

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();

	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	void applyContrast();
	void extractForeground();
	void blurPic();
	bool isBtnClicked(ofRectangle btn, int x, int y);
	bool checkForType(int btnTypeValue, int firstStateValue);
	void applyBrightness();
	void applyColorCorrection();

	ofColor mainColor;
	ofColor bgColor = ofColor(100, 150, 100);
	ofImage mainPic;
	Mat image;
	float cAlpha = 1;
	float cBrightness = 40;
	int gaussLevel, btnType, size, cBeta, width, height, xLocation, yLocation, firstState, DRAWING_Y_SIZE, secondState = 0;
	bool penKind, penShape = true;
	ofRectangle cleanBtn, pencilBtn, foregroundBtn, penSwitcher, rectangleBtn, contrastBtn, colorCorrectionBtn, brightnessBtn, blurBtn;
	vector<ofRectangle> BUTTONS;

};