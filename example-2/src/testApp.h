#pragma once

#include "ofMain.h"
#include "ofxSigar.h"

class testApp : public ofBaseApp {
	public:
		void setup();
		void update();
		void draw();
		

    ofxSigar sigar;
//    
//    process
//    
//    sigar_proc_mem_t
//    sigar_proc_time_t
//    sigar_proc_cred_name_t
//    sigar_proc_cred_t
//    sigar_proc_cpu_t
//    sigar_thread_cpu_t
//    sigar_file_system_type_e
//    sigar_file_system_t
//    sigar_proc_args_t
//    sigar_proc_env_t
//    sigar_proc_exe_t
    
//    sigar_net_interface_stat_t
    
    
    
//    SIGAR includes support for Linux, FreeBSD, Windows, Solaris, AIX, HP-UX and Mac OSX across a variety of versions and architectures. Users of the SIGAR API are given portable access to inventory and monitoring data including:
//        
//        System memory, swap, cpu, load average, uptime, logins
//        Per-process memory, cpu, credential info, state, arguments, environment, open files
//        File system detection and metrics
//        Network interface detection, configuration information and metrics
//        Network route and connection tables

    
};
