#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetFrameRate(1);

//    sigar.cpu();
//
//    sigar.fs();
//    sigar.mem();
    sigar.netif();
    sigar.netconn();
    sigar.pid();
    sigar.proc();
    sigar.uptime();
    
}

//--------------------------------------------------------------
void testApp::update(){
    //sigar.ps();
    
}

//--------------------------------------------------------------
void testApp::draw(){

    ofBackground(0);
    
    string monitor;

    
    ofSetColor(255);
    ofDrawBitmapString(monitor, 30, 30);
    
    
}
