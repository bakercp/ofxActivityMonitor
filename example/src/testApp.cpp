#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){

    ofBackground(0);
    
    ofxActivityMonitor::Instance()->test();
    
    string monitor = "";
    monitor += "-----------------------------------CPU------------------------------------";
    monitor += "\n";
    monitor += "      Number of Cores: ";
    monitor += ofToString(ofxActivityMonitor::Instance()->getNumCPUCores());
    monitor += "\n";
    monitor += "    CPU Load Averages: ";
    monitor += ofToString(ofxActivityMonitor::Instance()->getCPULoadAvg());
    monitor += "\n";
    monitor += "-----------------------------------RAM------------------------------------";
    monitor += "\n";
    monitor += "          Memory Used: ";
    monitor += ofToString(ofxActivityMonitor::Instance()->getSystemMemoryUsed());
    monitor += "\n";
    monitor += "       Process Memory: ";
    monitor += ofToString(ofxActivityMonitor::Instance()->getSystemMemoryUsedProcess());
    monitor += "\n";
    monitor += "         Total Memory: ";
    monitor += ofToString(ofxActivityMonitor::Instance()->getSystemMemoryTotal());
    monitor += "\n";
    monitor += "-----------------------------------GPU------------------------------------";
    monitor += "\n";
    monitor += "     Total GPU Memory: ";
    monitor += ofToString(ofxActivityMonitor::Instance()->getGPUMemoryTotal());
    monitor += "\n";
    monitor += "Total GPU Memory Used: ";
    monitor += ofToString(ofxActivityMonitor::Instance()->getGPUMemoryUsed());
    monitor += "\n";
    monitor += "           GPU Vendor: ";
    monitor += ofxActivityMonitor::Instance()->getGPUVendor();
    monitor += "\n";
    monitor += "         GPU Renderer: ";
    monitor += ofxActivityMonitor::Instance()->getGPURenderer();
    monitor += "\n";
    monitor += "          GPU Version: ";
    monitor += ofxActivityMonitor::Instance()->getGPUVersion();
    monitor += "\n";
    monitor += "GPU Shading Lang Ver.: ";
    monitor += ofxActivityMonitor::Instance()->getGPUShadingLanguageVersion();
    monitor += "\n";
    /*
    monitor += "       GPU Extensions: ";
    vector<string> extensions = ofxActivityMonitor::Instance()->getGPUExtensions();
    for(int i = 0; i < (int)extensions.size(); i++) {
        monitor += "                       ";
        monitor += ofxActivityMonitor::Instance()->getGPUVersion();
        monitor += "\n";
    }
     */
    


    
    ofSetColor(255);
    ofDrawBitmapString(monitor, 30, 30);
    
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}