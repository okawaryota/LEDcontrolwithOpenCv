#include "ofApp.h"
int pixels_LED;

//--------------------------------------------------------------
void ofApp::setup(){
    
    
    grabber.initGrabber(320,240);
    colorImage.allocate(320, 240);
    currFrame.allocate(320, 240);
    prevFrame.allocate(320, 240);
    diffImage.allocate(320, 240);
    
    ard.connect("/dev/tty.usbmodem1411", 57600);
	//ard.connect("/dev/ttyUSB0", 57600);
	
	// listen for EInitialized notification. this indicates that
	// the arduino is ready to receive commands and it is safe to
	// call setupArduino()
	ofAddListener(ard.EInitialized, this, &ofApp::setupArduino);
    
	bSetupArduino	= false;	// flag so we setup arduino when its ready, you don't need to touch this :)
    
    }

//--------------------------------------------------------------
void ofApp::update(){
    
    grabber.update();
    
    if (grabber.isFrameNew()){
        
        colorImage.setFromPixels(grabber.getPixelsRef());
        currFrame = colorImage;
        currFrame.flagImageChanged();
        
        diffImage.absDiff( currFrame, prevFrame );
        diffImage.threshold(mouseX/10);
        
        int nWhitePixels = diffImage.countNonZeroInRegion(0, 0, 320, 240);
        cout << nWhitePixels << endl;
        
        pixels_LED = nWhitePixels;
        
        prevFrame = currFrame;
    }
    
    updateArduino();

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    grabber.draw(0,0);
    currFrame.draw(320,0);
    prevFrame.draw(0,240);
    diffImage.draw(320,240);
}
//--------------------------------------------------------------
void ofApp::setupArduino(const int & version){
    
    ofRemoveListener(ard.EInitialized, this, &ofApp::setupArduino);
	ard.sendDigitalPinMode(11, ARD_PWM);		// on diecimelia: 11 pwm?*/
	bSetupArduino = true;
    
}
//--------------------------------------------------------------
void ofApp::updateArduino(){
    
    float pixelsMapped = ofMap(pixels_LED, 0, 320*240, 0,255);
	// update the arduino, get any data or messages.
	ard.update();
	
	// do not send anything until the arduino has been set up
	if (bSetupArduino) {
		ard.sendPwm(11, pixelsMapped);   // pwm...
        
        //アナログピンモードからセンサーの値をインポートするのが次のステップ！
        
	}
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
