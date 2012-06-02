#pragma once

#include "ofMain.h"
#include "ofxActivityMonitor.h"

class testApp : public ofBaseApp {
	public:
		void setup();
		void update();
		void draw();

    ofxActivityMonitor* mon;

};
