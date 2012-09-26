#include "testApp.h"
#include "ofxSimpleGuiToo.h"

bool drawRingFrontside = 1;
bool drawRingBackside = 1;
bool drawSphere = 1;

float sceneRotX;
float ringRotX;

ofVideoGrabber vidGrabber;

//static int key_ring_map[] = { OF_KEY_LEFT, OF_KEY_DOWN, OF_KEY_RIGHT, 'a', 's', 'd' };
static int key_ring_map[] = { 'a', 's', 'd', OF_KEY_LEFT, OF_KEY_DOWN, OF_KEY_RIGHT };

static int colors[] = { 0xE00278, 0xE56105, 0xFBF002, 0x62F529, 0x0CF3CD, 0xF30CEA };


//--------------------------------------------------------------
void testApp::setup() {
	ofSetVerticalSync(true);
	ofBackgroundHex(0xfdefc2);
	ofSetLogLevel(OF_LOG_NOTICE);
	ofEnableAlphaBlending();
	ofEnableSmoothing();
	ofSetCircleResolution(31);
	ofEnableLighting();
	//glEnable(GL_DEPTH_TEST);
	//glEnable(GL_


	for (int i = 0; i < 511; i++) {
		keyDownTime[i] = 0;
	}

	gui.addTitle("scene \nsettings");
	gui.addSlider("Scene rotationX", sceneRotX, 0.0, 60.0);
	
	gui.addTitle("ring \nsettings").setNewColumn(true);
	gui.addToggle("Draw ring \nfrontside", drawRingFrontside);
    gui.addToggle("Draw ring \nbackside", drawRingBackside);
	gui.addToggle("Draw sphere", drawSphere);
	gui.addSlider("Ring rotationX", ringRotX, 0.0, 360.0);
	
	gui.addTitle("Camera Feedback").setNewColumn(true);
	gui.addContent("Camera feed", vidGrabber);


	gui.addFPSCounter();
    gui.loadFromXML();
	gui.show();


	light.enable();
	light.setPointLight();
	light.setScale(20);
	

	box2d.init();
	box2d.setGravity(0, 0);
	box2d.setFPS(30.0);
	
	float xx = 2*ofGetWindowWidth()/3;
	float yy = ofGetWindowHeight()/3;
	/*float yy=sin(ofGetElapsedTimef()*0.4)*150;
	float zz=cos(ofGetElapsedTimef()*0.4)*150;*/

	light.setPosition(xx,yy,500);
	//light.draw();
	
	vidGrabber.initGrabber(640, 480);
	fadeTimer = 0;

	sceneRotX = 31;
	ringRotX = 0;

	lastMillis = ofGetElapsedTimeMillis();
}

//--------------------------------------------------------------
void testApp::update() {
	
	vidGrabber.update();


	fadeTimer -= ofGetElapsedTimeMillis() - lastMillis;
	
	if (fadeTimer < 0)
		fadeTimer = 0;

	rotTimer -= ofGetElapsedTimeMillis() - lastMillis;
	if (rotTimer < 0){
		rotTimer = 0;
		ringRotX = 0;
	}

	if (rotTimer > 0) {
		ringRotX = 360 * (rotTimer/5000.0);
		//cout << "ringRotX: " << ringRotX << endl;
	}

	lastMillis = ofGetElapsedTimeMillis();

	box2d.update();	
	ofVec2f mouse(ofGetMouseX(), ofGetMouseY());
	float minDis = ofGetMousePressed() ? 300 : 200;

	for(int i=0; i<rings.size(); i++) {
		float dis = mouse.distance(rings[i].getPosition());
		//if(dis < minDis) circles[i].addRepulsionForce(mouse, 9);
		/*else*/ rings[i].addAttractionPoint(ofGetWindowWidth()/2,ofGetWindowHeight()/2, 4.0);
	}
}


//--------------------------------------------------------------
void testApp::draw() {
	//ofBackgroundGradient(ofColor(0,0,0,255),ofColor(47,0,47),OF_GRADIENT_CIRCULAR);
	
	ofBackgroundGradient(ofColor(255,255,255,255),ofColor(47,0,47),OF_GRADIENT_CIRCULAR);
	//ofBackgroundGradient(ofColor(47,0,47,255),ofColor(255,255,255),OF_GRADIENT_CIRCULAR);
	

	if (fadeTimer > 0){
		ofSetColor(255,255,255,255*(fadeTimer/5000.0));
		vidGrabber.draw(0,0,640,480);
	}

	ofPushMatrix();
	ofRotateX(sceneRotX);

	for(int i=0; i<rings.size(); i++) {
		rings[i].draw(drawRingFrontside, drawRingBackside, drawSphere, ringRotX);  // drawFrontside, drawBackside, isSphere, ringRot
	}
	
	// draw the ground
	box2d.drawGround();


	ofPopMatrix();
	
	
	string info = "";
	/*info += "Press [c] for circles\n";
	info += "Press [b] for blocks\n";
	*/
	info += "Total Bodies: "+ofToString(box2d.getBodyCount())+"\n";
	info += "Total Joints: "+ofToString(box2d.getJointCount())+"\n\n";
	info += "FPS: "+ofToString(ofGetFrameRate(), 1)+"\n";
	ofSetHexColor(0x444342);
	ofDrawBitmapString(info, 30, 30);

	gui.draw();
}



//--------------------------------------------------------------
void testApp::keyPressed(int key) {
	if (keyDownTime[key] == 0)
        keyDownTime[key] = ofGetElapsedTimeMillis();

	int millisElapsed = ofGetElapsedTimeMillis() - keyDownTime[key];
    

	// WHAT COLOR TO USE BASED ON KEY RING MAP
	int circle_index = -1;

	for (int i = 0; i < 6; i++){
		if (key == key_ring_map[i])
			circle_index = i;
	}

	cout << "circle index is: " << circle_index << endl;

	// HOW MANY KEYS AT ONCE?
	int numKeysDown = 0;
	for (int i = 0; i < 511; i++) {
		if (keyDownTime[i] != 0)
			numKeysDown++;
	}
	
	cout << numKeysDown << " keys down!" << endl;
	
	// ONE KEY, SIMPLE ADD COLOR CIRCLE
	if (numKeysDown == 1 && circle_index != -1) {
		CustomParticle p;
		p.setPhysics(1.0, 0.5, 0.3);
		p.setup(box2d.getWorld(), mouseX, mouseY, 23/*ofRandom(20, 60)*/);
		ofColor circle_color;
		circle_color.setHex(colors[circle_index]);
		p.setupTheCustomData(circle_color);
		rings.push_back(p);
	}

	// TWO KEY, SIMPLE ADD COLOR CIRCLE
	if (numKeysDown == 2 && circle_index != -1) {
		CustomParticle p;
		p.setPhysics(1.0, 0.5, 0.3);
		p.setup(box2d.getWorld(), mouseX, mouseY, 23/*ofRandom(20, 60)*/);
		int mixedColor = 0x000000;
		for (int i = 0; i < 6; i++)
			if (keyDownTime[key_ring_map[i]] != 0)
				mixedColor += colors[i];
		ofColor circle_color;
		circle_color.setHex(mixedColor);
		p.setupTheCustomData(circle_color);
		rings.push_back(p);
	}

	// FOUR KEY, BOOM, force from center
	if (numKeysDown == 4 && circle_index != -1) {
		rotTimer = 2000;
		for(int i=0; i<rings.size(); i++) {
			rings[i].addAttractionPoint(ofGetWindowWidth()/2,ofGetWindowHeight()/2, -750.0);
		}
	}

	// FIVE KEY, PICTURE
	if (numKeysDown == 5 && circle_index != -1) {
		fadeTimer = 5000;
	}

	// THREE KEY, ROT TIMER
	if (numKeysDown == 3 && circle_index != -1) {
		rotTimer = 2000;
		for(int i=0; i<rings.size(); i++) {
			rings[i].addAttractionPoint(ofGetWindowWidth()/2,ofGetWindowHeight()/2, -100.0);
		}
	}

	if(key == 's') {
		box2d.doSleep = 1;
	}

	if(key == 't') ofToggleFullscreen();

	if(key == 'g') 
		gui.toggleDraw();

	if(key == 'c')
		wreckShop();
}

//--------------------------------------------------------------
void testApp::keyReleased(int key) {
	cout << "keyReleased: " << key << endl;
	keyDownTime[key] = 0; //ofGetElapsedTimeMillis();

}

void testApp::wreckShop(){

 
        //erase all images

		for (int i=0; i<rings.size(); i++) {

            rings[i].destroy();

		}

		rings.erase(rings.begin(), rings.end());

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ) {
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button) {
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {
	
	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button) {
}

//--------------------------------------------------------------
void testApp::resized(int w, int h){
}

