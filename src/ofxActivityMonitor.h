//
//  ofxActivityMonitor.h
//  ofxActivityMonitorExample
//
//  Created by Christopher Baker on 5/14/12.
//  Copyright (c) 2012 School of the Art Institute of Chicago. All rights reserved.
// 
//  Portions from Vincent R. on 07.11.11.
//  Copyright 2011 www.say-nono.com. All rights reserved.
//
//  further information from:
//  http://stackoverflow.com/questions/63166/how-to-determine-cpu-and-memory-consumption-from-inside-a-process
//
//

#pragma once


#include "ofMain.h"
#include "Poco/RegularExpression.h"
using Poco::RegularExpression;

#ifdef TARGET_WIN32
#include "windows.h"
#include "psapi.h"
#endif

#ifdef TARGET_LINUX
#include <proc/readproc.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "sys/times.h"
#include "sys/vtimes.h"
#endif

#ifdef TARGET_OSX
#include <sys/types.h>
#include <sys/sysctl.h>

#include <mach/vm_statistics.h>
#include <mach/mach_types.h> 
#include <mach/mach_init.h>
#include <mach/mach_host.h>
#include <mach/mach.h>

#endif

#include "sigar.h"


/*

#define GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX 0x9048
#define GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX 0x9049

GLint nTotalMemoryInKB = 0;
glGetIntegerv( GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX,
              &nTotalMemoryInKB );

GLint nCurAvailMemoryInKB = 0;
glGetIntegerv( GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX,
              &nCurAvailMemoryInKB );

 */




class ofxActivityMonitor {
  
public:
    static ofxActivityMonitor* Instance();
    
    void update(ofEventArgs& event);
    
    // CPU
    int  getNumCPUCores();
    vector<float> getCPULoadAvg();

    // RAM
    void setSystemMemoryUpdateInterval(int ms);
    int  getSystemMemoryUpdateInterval();
    int  getSystemMemoryTotal();
	int  getSystemMemoryUsed();
	int  getSystemMemoryUsedProcess();

    // GPU
    void setGPUUpdateInterval(int ms);
    int  getGPUUpdateInterval();
    int  getGPUMemoryTotal();
    int  getGPUMemoryUsed();
    
    string getGPUVendor();
    string getGPURenderer();
    string getGPUVersion();
    string getGPUShadingLanguageVersion();
    //vector<string> getGPUExtensions();
    
    //string getCtlInfo(int NAME);
    //string getSysConfInfo(int NAME);

    
private:
    // singleton
    ofxActivityMonitor();
    virtual ~ofxActivityMonitor();
    ofxActivityMonitor(ofxActivityMonitor const&){};
    ofxActivityMonitor& operator=(ofxActivityMonitor const&){};
    static ofxActivityMonitor* instance;

    // calculations
    
    bool getCpuInfo();
    bool getCpuList();
    bool getProcList();
    bool getFsList();
    bool getMem();
    bool getNetConn();
    bool getNetif();    
    
    bool getResLimit();
    bool getSwap();
    
    bool getSysInfo();
    
    
    bool version();
    bool uptime();
    
    bool pid();
    
    bool moreProc();
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    vector<float> calcCPULoadAvg();

    int calcMemoryTotal();
    int calcMemoryUsed();
    int calcMemoryUsedProcess();
    
    int calcMemoryTotalGPU();
    int calcMemoryUsedGPU();
    
    // vars
    int cpuLastUpdate;
    int cpuUpdateInterval;
    int numCPUCores;
    vector<float> cpuActivity;
    vector<float> cpuLoadAvg;
    
    int memoryLastUpdate;
    int memoryUpdateInterval;
    
	int memoryTotal;
	int memoryUsed;
	int memoryUsedProcess;	
	
    
    
    int memoryGPULastUpdate;
    int memoryGPUUpdateInterval;
    
    int numGPUS;

    int memoryTotalGPU;
    int memoryUsedGPU;
    
    string vendorGPU;
    string rendererGPU;
    string versionGPU;
    string shadingLangVersionGPU;
    
    vector<string> extensionsGPU;
    
    
    
    
    
    
    
    sigar_cpu_list_t cpulist;
    sigar_cpu_info_list_t cpuinfo;

    
    
    
    
    
    
    
    // the main sigar object
    sigar_t *sigar;
    

};