#include "ofApp.h"
#include <iostream>

/*
Copyright Kevin Valerio
https://github.com/kevin-valerio/PhotoCv/
Please keep the credits
*/
 
void ofApp::setup() {
	
	ofSetBackgroundAuto(false);
	ofBackground(bgColor);

	//Init few variables 
	int marginLeft = 450;
	gaussLevel = 10;
	
	//Init all the needed buttons
	rectangleBtn = ofRectangle(marginLeft + 10, 10, 100, 50);
	pencilBtn = ofRectangle(rectangleBtn.x + rectangleBtn.width + 15, 10, 80, 50);
	penSwitcher = ofRectangle(pencilBtn.x + pencilBtn.width + 15, 10, 100, 50);
	cleanBtn = ofRectangle(penSwitcher.x + penSwitcher.width + 15, 10, 70, 50);
	contrastBtn = ofRectangle(cleanBtn.x + cleanBtn.width + 15, 10, 165, 50); 
	colorCorrectionBtn = ofRectangle(contrastBtn.x + contrastBtn.width + 15, 10, 105, 50);
	brightnessBtn = ofRectangle(colorCorrectionBtn.x + colorCorrectionBtn.width + 15, 10, 160, 50);
	foregroundBtn = ofRectangle(brightnessBtn.x + brightnessBtn.width + 15, 10, 160, 50);
	blurBtn = ofRectangle(foregroundBtn.x + foregroundBtn.width + 15, 10, 160, 50);
	
	//Constants being initialized
	BUTTONS = { rectangleBtn, pencilBtn, penSwitcher, cleanBtn, contrastBtn, colorCorrectionBtn, brightnessBtn, foregroundBtn, blurBtn };
	DRAWING_Y_SIZE = cleanBtn.getY() + cleanBtn.getWidth() + 15;

}

//Each pixels of the "image" matrix will change their value to cAlpha * value + cBeta
void ofApp::applyContrast() { 
	image = toCv(mainPic.getPixels());
	image.convertTo(image, -1, cAlpha, cBeta); 
}

//Setting the new setColor
void ofApp::update() {
	ofSetColor(mainColor);
}

//Extracting the foreground
void ofApp::extractForeground() {

	image = toCv(mainPic.getPixels());
	cvtColor(image, image, CV_RGBA2RGB);
	Rect rectangle(40, 90, image.cols - 80, image.rows - 170);
	Mat result, bgModel, fgModel, input_bgra;
	//Using the grab cut segmentation algorithm
	grabCut(image, result, rectangle, bgModel, fgModel, 1, GC_INIT_WITH_RECT); 	
	compare(result, GC_PR_FGD, result, CMP_EQ);
	//Setting a 4 dimensional matrix of the foreground
	Mat foreground(image.size(), CV_8UC4, Scalar(255, 255, 255, 1));
	image.copyTo(foreground, result);   
	
	//Removing all the white pixels (setting alpha = 0)
	cvtColor(foreground, input_bgra, CV_BGR2BGRA);
	for (int y = 0; y < input_bgra.rows; ++y)
		for (int x = 0; x < input_bgra.cols; ++x) {
			Vec4b& pixel = input_bgra.at<Vec4b>(y, x);
			if (pixel[0] < 10 && pixel[1] < 10 && pixel[2] < 10)
				pixel[3] = 0;			
		}	

	image = input_bgra;

	//Drawing the result
	drawMat(input_bgra, image.rows , DRAWING_Y_SIZE);

}

//Using gaussian bluring algorithm to blur each pixel
void ofApp::blurPic() {
	image = toCv(mainPic.getPixels());
	gaussLevel += 5;
	for (int i = 1; i < gaussLevel; i = i + 2) 
		GaussianBlur(image, image, Size(i, i), 0, 0);		
}

//Each pixels of the "image" matrix will change their value the same value plus cBrightness value
void ofApp::applyBrightness() {
	image = toCv(mainPic.getPixels());
	image.convertTo(image, -1, 1, cBrightness);
}

//Nothing to do, just set the new image matrix to the main picture
void ofApp::applyColorCorrection() {
	image = toCv(mainPic.getPixels());
}

void ofApp::draw() {
	
	ofFill();

	//Setting the new 
	ofSetColor(mainColor);

	//If user wants to draw a rectangle, drawing it
	if (btnType == 1 && firstState == 3) {
		ofDrawRectangle(xLocation, yLocation, width, height);
		firstState = 1;
	}	 

	//If user wants to erase, selecting the eraser
	if (btnType == 3 && firstState == 2) {
		ofSetColor(bgColor);
		ofDrawEllipse(mouseX, mouseY, 10 + size, 10 + size);
	}

	//If user wants to paint (drawing an ellipse)
	if (btnType == 2 && firstState == 2) {
		//Depending of the pen kind (pen or brush)
		if (penKind) {
			ofDrawEllipse(mouseX, mouseY, 8 + size, 8 + size);	
		}	 			
		else {
			ofDrawEllipse(mouseX + ofRandom(-10, 10), mouseY + ofRandom(-10, 10), 3 + size, 3 + size);
		}
	}

	//We draw the image matrix, if any filter has been applied (i.e Gaussian Blur, Color Correction, Brightness control...)
	drawMat(image, 0, DRAWING_Y_SIZE);

	ofNoFill();
	ofSetColor(255, 255, 255);
 
	//Drawing each buttons
	for (ofRectangle btn : BUTTONS) 
		ofDrawRectangle(btn);

	//Giving a name to each button
	ofDrawBitmapString("Rectangle", rectangleBtn.x + 5, rectangleBtn.y + rectangleBtn.height / 2);
	ofDrawBitmapString("Pencil", pencilBtn.x + 5, pencilBtn.y + pencilBtn.height / 2);
	ofDrawBitmapString("Switch pen", penSwitcher.x + 5, penSwitcher.y + penSwitcher.height / 2);
	ofDrawBitmapString("Clean", cleanBtn.x + 5, cleanBtn.y + cleanBtn.height / 2);
	ofDrawBitmapString("Contrasted picture", contrastBtn.x + 5, contrastBtn.y + contrastBtn.height / 2);
	ofDrawBitmapString("Brightned picture", brightnessBtn.x + 5, brightnessBtn.y + brightnessBtn.height / 2);
	ofDrawBitmapString("Color tuning", colorCorrectionBtn.x + 5, colorCorrectionBtn.y + colorCorrectionBtn.height / 2);
	ofDrawBitmapString("Extract foreground", foregroundBtn.x + 5, foregroundBtn.y + foregroundBtn.height / 2);
	ofDrawBitmapString("Blur effect (Gauss)", blurBtn.x + 5, blurBtn.y + blurBtn.height / 2);
	ofDrawBitmapString("R G Y W B : pen color ", contrastBtn.x - 5, 80);
	
}
 
//Event for a pressed key
void ofApp::keyPressed(int key) {

	//Setting the new color for each new key (Red, Green, Blue, Yellow, White)
	switch (key) {
	default:
		break;
	case 'r':
		mainColor.set(255, 0, 0);
		break;
	case 'g':
		mainColor.set(0, 255, 0);
		break;
	case 'b':
		mainColor.set(0, 0, 255);
		break;
	case 'y':
		mainColor.set(255, 255, 0);
		break;
	case 'w':
		mainColor.set(255, 255, 255);
		break;
	//Saving the picture (bugged feature)
	case 's':
		mainPic.grabScreen(0, 0, 1024, 600);
		mainPic.save("saved_Picture" + to_string(rand() % 100) + ".jpg");
		break;

	//Loading the picture.
	case 'l':
		if(!mainPic.load("myFile.jpg")) 
			mainPic.load("myFile.png");
		mainPic.draw(0, DRAWING_Y_SIZE);
		break;
	}
}
 
//Simple function to check a type and a state
bool ofApp::checkForType(int btnTypeValue, int firstStateValue) {
	return btnType == btnTypeValue && firstState == firstStateValue;
}

//Checking if a button is being clicked
bool ofApp::isBtnClicked(ofRectangle btn, int x, int y) {
	return 	(x >= btn.x && x <= btn.x + btn.width && y >= btn.y && y <= btn.y + btn.height);
}

//Event for a pressed mouse button
void ofApp::mousePressed(int x, int y, int button) {

	//Setting the variables and the triggered functions (i.e extractForeground(), applyColorCorrection()...) for each button
	if (isBtnClicked(foregroundBtn, x, y))
		extractForeground();

	if (isBtnClicked(colorCorrectionBtn, x, y))
		applyColorCorrection();

	if(isBtnClicked(blurBtn, x, y))
		blurPic();
 
	if (isBtnClicked(brightnessBtn, x, y)) {
		cBrightness += 5;
		applyBrightness();
	}

	if (isBtnClicked(pencilBtn, x, y)) {
		btnType = 2;
		firstState = 1;
	}
	   	 
	if (isBtnClicked(contrastBtn, x, y)) {
		cBeta += 10;
		cAlpha -= 0.1;
		applyContrast();
	}
	
	if (isBtnClicked(penSwitcher, x, y)) {
		penKind = !penKind;
		firstState = 0;
	} 

	if (isBtnClicked(cleanBtn, x, y)) {
		btnType = 3;
		firstState = 1;
	}
	 
	if (isBtnClicked(rectangleBtn, x, y)) {
		btnType = 1;
		firstState = 1;
	}
	  

	if (checkForType(1, 2)) {
		width = x - xLocation;
		height = y - yLocation;
		firstState = 3;
	}


	if (checkForType(1, 1)) {
		xLocation = x;
		yLocation = y;
		firstState = 2;
	}
	
	if (checkForType(2, 2)) {
		firstState = 1;
		secondState = 1;
	} 

	if (checkForType(2, 3)) {
		firstState = 1;
		secondState = 1;
	}

	if ((btnType == 2 || btnType == 3) && firstState == 1 && secondState == 0)
		firstState = 2; 

	secondState = 0;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
