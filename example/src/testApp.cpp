#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    mon = ofxActivityMonitor::Instance();
    
    
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){

    ofBackground(0);
    
//    ofxActivityMonitor::Instance()->test();
    
    string monitor = "";
    monitor += "-----------------------------------CPU------------------------------------";
    monitor += "\n";
    monitor += "      Number of Cores: ";
    monitor += ofToString(mon->getNumCPUCores());
    monitor += "\n";
    monitor += "    CPU Load Averages: ";
    monitor += ofToString(mon->getCPULoadAvg());
    monitor += "\n";
    monitor += "-----------------------------------RAM------------------------------------";
    monitor += "\n";
    monitor += "          Memory Used: ";
    monitor += ofToString(mon->getSystemMemoryUsed());
    monitor += "\n";
    monitor += "       Process Memory: ";
    monitor += ofToString(mon->getSystemMemoryUsedProcess());
    monitor += "\n";
    monitor += "         Total Memory: ";
    monitor += ofToString(mon->getSystemMemoryTotal());
    monitor += "\n";
    monitor += "-----------------------------------GPU------------------------------------";
    monitor += "\n";
    monitor += "     Total GPU Memory: ";
    monitor += ofToString(mon->getGPUMemoryTotal());
    monitor += "\n";
    monitor += "Total GPU Memory Used: ";
    monitor += ofToString(mon->getGPUMemoryUsed());
    monitor += "\n";
    monitor += "           GPU Vendor: ";
    monitor += mon->getGPUVendor();
    monitor += "\n";
    monitor += "         GPU Renderer: ";
    monitor += mon->getGPURenderer();
    monitor += "\n";
    monitor += "          GPU Version: ";
    monitor += mon->getGPUVersion();
    monitor += "\n";
    monitor += "GPU Shading Lang Ver.: ";
    monitor += mon->getGPUShadingLanguageVersion();
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

