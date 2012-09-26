#pragma once
#include "ofMain.h"
#include "ofxBox2d.h"

// A simple little Data class. This is were
// you can store anything you want.

class Data {
public:
	ofColor color;
};

// A Custom Particle extedning the box2d circle
class CustomParticle : public ofxBox2dCircle {
	
public:
	
	void setupTheCustomData(ofColor myColor) {
		
		// we are using a Data pointer because 
		// box2d needs to have a pointer not 
		// a referance
		setData(new Data());
		Data * theData = (Data*)getData();
		
		//theData->color.setHex(colors[(int)ofRandom(0, 6)]);
		theData->color = myColor;

		//printf("setting the custom data!\n");


		
	}
	
	void draw(bool drawFrontside = 1, bool drawBackside = 1, bool isSphere = 1, int spinRotation = 0) {
		Data* theData = (Data*)getData();
		if(theData) {
			
			// Evan though we know the data object lets just 
			// see how we can get the data out from box2d
			// you would use this when using a contact listener
			// or tapping into box2d's solver.
			
			float radius = getRadius();
			
			ofPushMatrix();
			ofTranslate(getPosition());
			ofRotateZ(getRotation());

			ofRotateY(spinRotation);


			if (isSphere){
				theData->color.a = 20;
				ofSetColor(theData->color);
				ofFill();
				//ofCircle(0, 0, radius);
				ofSphere(radius*2);
			}


			theData->color.a = 255;
			if (drawFrontside){
				ofSetColor(theData->color);

				ofNoFill();
				ofSetLineWidth(3);
				ofCircle(0, 0, radius*2);
			}
			
			if (drawBackside){
				ofSetColor(theData->color);
				ofNoFill();
				ofRotateY(180);
				ofTranslate(0,0,2);
				ofCircle(0, 0, radius*2);
			}

			ofSetColor(255);
			//ofDrawBitmapString(theData->name, -5, 5);
			ofPopMatrix();
		}
	}
		
		
};

// -------------------------------------------------

class testApp : public ofBaseApp {
	
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
	void resized(int w, int h);
	void wreckShop();
	
	int crossHairSpeed;
	int keyDownTime[511];

	ofxBox2d						box2d;			  //	the box2d world
	vector		<ofxBox2dCircle>	circles;		  //	default box2d circles
	vector		<ofxBox2dRect>		boxes;			  //	defalut box2d rects
	vector		<CustomParticle>	rings;			  //	ring particles
	ofLight		light;

	int fadeTimer;
	int rotTimer;
	int lastMillis;

};

