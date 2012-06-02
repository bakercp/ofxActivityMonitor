#include "ofxActivityMonitor.h"



//--------------------------------------------------------------
ofxActivityMonitor* ofxActivityMonitor::instance = NULL;  

//--------------------------------------------------------------
ofxActivityMonitor* ofxActivityMonitor::Instance()
{
    if (!instance) {  // Only allow one instance of class to be generated.
        instance = new ofxActivityMonitor();
    }
    
    return instance;
}

//--------------------------------------------------------------
ofxActivityMonitor::ofxActivityMonitor(){
    sigar_open(&sigar);

//    getProcList();  
//    getFsList();
  
//    getMem();
  
//    getNetConn();
  
//    getNetif();
  
//    getResLimit();
//    getSwap();
//    getSysInfo();
  
//    uptime();
//    version();

//    pid();
  
    moreProc();
    
    //numCPUCores             = calcNumCpuCores();
    
    
    memoryLastUpdate        = -INT_MAX;
    memoryUpdateInterval    = 10000;
    memoryTotal             = 0;
	memoryUsed              = 0;
	memoryUsedProcess       = 0;	
	
    
    
    
    
    
    
    
    
    
    
//    
//    memoryGPULastUpdate     = -INT_MAX;
//    memoryGPUUpdateInterval = 10000;
//    memoryTotalGPU          = 0;
//    memoryUsedGPU           = 0;
//    
//
//    
//    vendorGPU               = (char*)glGetString(GL_VENDOR);
//    rendererGPU             = (char*)glGetString(GL_RENDERER);
//    versionGPU              = (char*)glGetString(GL_VERSION);
//    shadingLangVersionGPU   = (char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
//    
//    
//    string s                = (char*)glGetString(GL_EXTENSIONS);
//    
//    cout << s << endl;
//    
//    // https://github.com/codykrieger/gfxCardStatus/blob/master/Classes/SystemInfo.m
//    
//    /*RegularExpression re(" ");
//    re.split(s, 0, extensionsGPU);
//     */
//
    
    ofAddListener(ofEvents().update,this,&ofxActivityMonitor::update);
}

//--------------------------------------------------------------
ofxActivityMonitor::~ofxActivityMonitor() {
    // unregister
    ofRemoveListener(ofEvents().update,this,&ofxActivityMonitor::update);
    // dispose of sigar instance
    sigar_close(sigar);
}


//--------------------------------------------------------------
void ofxActivityMonitor::update(ofEventArgs& event) {
    
    int now = ofGetElapsedTimeMillis();
    
    // cpu
    if(now > (cpuLastUpdate + cpuUpdateInterval)) {
        cpuLoadAvg    = calcCPULoadAvg();
        cpuLastUpdate = now;
    }
    
    // sys memory
    if(now > (memoryLastUpdate + memoryUpdateInterval)) {
        memoryTotal       = calcMemoryTotal();
        memoryUsed        = calcMemoryUsed();
        memoryUsedProcess = calcMemoryUsedProcess();		
        memoryLastUpdate  = now;
    }
    
    // gpu
    if(now > (memoryGPULastUpdate + memoryGPUUpdateInterval)) {
        memoryTotalGPU    = calcMemoryTotalGPU();
        memoryUsedGPU     = calcMemoryUsedGPU();
        
        memoryGPULastUpdate = now;
    }
}

//--------------------------------------------------------------
int ofxActivityMonitor::getNumCPUCores() {
    return numCPUCores;
}

//--------------------------------------------------------------
vector<float> ofxActivityMonitor::getCPULoadAvg() {
    return cpuLoadAvg;
}

//--------------------------------------------------------------
void ofxActivityMonitor::setSystemMemoryUpdateInterval(int ms) {
    memoryUpdateInterval = MAX(0,ms);
}

//--------------------------------------------------------------
int  ofxActivityMonitor::getSystemMemoryUpdateInterval() {
    return memoryUpdateInterval;
}


//--------------------------------------------------------------
int ofxActivityMonitor::getSystemMemoryTotal(){
    return memoryTotal;
}

//--------------------------------------------------------------
int ofxActivityMonitor::getSystemMemoryUsed(){
    return memoryUsed;
}

//--------------------------------------------------------------
int ofxActivityMonitor::getSystemMemoryUsedProcess(){
    return memoryUsedProcess;
}

//--------------------------------------------------------------
void ofxActivityMonitor::setGPUUpdateInterval(int ms) {
    memoryGPUUpdateInterval = MAX(0,ms);
}

//--------------------------------------------------------------
int  ofxActivityMonitor::getGPUUpdateInterval() {
    return memoryGPUUpdateInterval;
}

//--------------------------------------------------------------
int ofxActivityMonitor::getGPUMemoryTotal(){
    return memoryTotalGPU;
}

//--------------------------------------------------------------
int ofxActivityMonitor::getGPUMemoryUsed(){
    return memoryUsedGPU;
}

//--------------------------------------------------------------
string ofxActivityMonitor::getGPUVendor() {
    return vendorGPU;
}

//--------------------------------------------------------------
string ofxActivityMonitor::getGPURenderer() {
    return rendererGPU;
}

//--------------------------------------------------------------
string ofxActivityMonitor::getGPUVersion() {
    return versionGPU;
}

//--------------------------------------------------------------
string ofxActivityMonitor::getGPUShadingLanguageVersion() {
    return shadingLangVersionGPU;
}

//--------------------------------------------------------------
//string getCtlInfo(int TOP_LEVEL, ) {
    //https://developer.apple.com/library/mac/#documentation/Darwin/Reference/ManPages/man3/sysctl.3.html
/*
#ifdef TARGET_OSX
	int64_t physical_memory;
	int mib[2];
	mib[0] = CTL_HW;
	mib[1] = HW_MEMSIZE;
	size_t length = sizeof(int64_t);
	sysctl(mib, 2, &physical_memory, &length, NULL, 0);
	mb = physical_memory/1048576;
	//gb = physical_memory/1073741824;	
#endif
 */

//}

//--------------------------------------------------------------
//string getSysConfInfo(int NAME) {
//#ifdef TARGET_OSX
//    nCores = sysconf( _SC_NPROCESSORS_ONLN );
//#endif
    
//}


//--------------------------------------------------------------
//vector<string> ofxActivityMonitor::getGPUExtensions() {
//    return extensionsGPU;
//}

bool ofxActivityMonitor::getCpuInfo() {
    int status, i;
    sigar_cpu_info_list_t cpuinfo;
	
    status = sigar_cpu_info_list_get(sigar, &cpuinfo);
    
    if (status != SIGAR_OK) {
        string errString = "ofxActivityMonitor::";
        errString += "getCpuInfo() ";
        errString += "cpu_list error ";
        errString += ofToString(status) + " ";
        errString += sigar_strerror(sigar, status);
        ofLog(OF_LOG_ERROR,errString);
        return false;
    }
    
    
    for (i = 0; i < cpuinfo.number; i++) {
		sigar_cpu_info_t info = cpuinfo.data[i];
		cout << info.vendor;
		cout << "|" << info.model;
        cout << "|" << info.mhz;
        cout << "|" << info.cache_size;
        cout << "|" << info.cores_per_socket;
        cout << "|" << info.mhz_max;
        cout << "|" << info.mhz_min;
        cout << "|" << info.model;
        cout << "|" << info.total_cores;
        cout << "|" << info.total_sockets;
        cout << endl;
	}
    
	sigar_cpu_info_list_destroy(sigar, &cpuinfo);
    return true;
}

bool ofxActivityMonitor::getCpuList() {
    int status, i;
    sigar_cpu_list_t cpulist;

    status = sigar_cpu_list_get(sigar, &cpulist);
    
    
    if (status != SIGAR_OK) {
        string errString = "ofxActivityMonitor::";
        errString += "getCpuInfo() ";
        errString += "cpu_list error ";
        errString += ofToString(status) + " ";
        errString += sigar_strerror(sigar, status);
        ofLog(OF_LOG_ERROR,errString);
        return false;
    }
    
    
    for (i=0; i<cpulist.number; i++) {
        sigar_cpu_t cpu = cpulist.data[i];
        cout << cpu.total;
        cout << "|" << cpu.irq;
        cout << "|" << cpu.nice;
        cout << "|" << cpu.soft_irq;
        cout << "|" << cpu.stolen;
        cout << "|" << cpu.sys;
        cout << "|" << cpu.user;
        cout << "|" << cpu.wait;
        cout << endl;
    }
    
    sigar_cpu_list_destroy(sigar, &cpulist);
    return true;
}

bool ofxActivityMonitor::getProcList() {
    int status, i;
    sigar_proc_list_t proclist;
    
    
    status = sigar_proc_list_get(sigar, &proclist);
    
    if (status != SIGAR_OK) {
        printf("proc_list error: %d (%s)\n",
               status, sigar_strerror(sigar, status));
        return false;
    }
    
    for (i=0; i<proclist.number; i++) {
        sigar_pid_t pid = proclist.data[i];
        sigar_proc_state_t pstate;
        sigar_proc_time_t ptime;
        
        status = sigar_proc_state_get(sigar, pid, &pstate);
        if (status != SIGAR_OK) {
            cout << "error getting proc state ";
            printf("error: %d (%s) proc_state(%d)\n",
                   status, sigar_strerror(sigar, status), pid);
            continue;
        }
        
        status = sigar_proc_time_get(sigar, pid, &ptime);
        if (status != SIGAR_OK) {
            cout << "Error getting proc time." << endl;
            printf("error: %d (%s) proc_time(%d)\n",
                   status, sigar_strerror(sigar, status), pid);
            continue;
        }
        
        cout << pid;
        cout << "|" << pstate.name;
        cout << "|" << pstate.nice;
        cout << "|" << pstate.ppid;
        cout << "|" << pstate.priority;
        cout << "|" << pstate.processor;
        cout << "|" << pstate.state;
        cout << "|" << pstate.threads;
        cout << "|" << pstate.tty;
        cout << endl;
        
        
    }
    
    sigar_proc_list_destroy(sigar, &proclist);
    
    return true;

}

bool ofxActivityMonitor::getFsList() {
    int status, i;
    sigar_file_system_list_t fslist;
    
    status = sigar_file_system_list_get(sigar, &fslist);

    if(status != SIGAR_OK) {
        cout << "problem loading file system stuff." << endl;
        return false;
    }
    
    cout << "# filesystems=" << fslist.number << endl;
    
	for (i = 0; i < fslist.number; i++) {
		sigar_file_system_t fs = fslist.data[i];
		sigar_file_system_usage_t fsusage;
		sigar_disk_usage_t diskusage;
		int ret;
        
        
        cout << "FileSystem";
        cout << "|" << fs.dir_name;
        cout << "|" << fs.dev_name;
        cout << "|" << fs.type_name;
        cout << "|" << fs.sys_type_name;
        cout << "|" << fs.type;
        cout << endl;

        ret = sigar_file_system_ping(sigar, &fs);
        
        
        
		if (ret != SIGAR_OK) {
            cout << "\t\t\t there was a problem pinging." << endl;
		}
        
        ret = sigar_file_system_usage_get(sigar, fs.dir_name, &fsusage);
        
        if(ret != SIGAR_OK) {
            cout << "sigar_file_system_usage_get failure." << endl;
        } else {
            
            cout << "\t\tUSAGE";
            cout << "|" << fsusage.total;
            cout << "|" << fsusage.free;
            cout << "|" << fsusage.used;
            cout << "|" << fsusage.avail;
            cout << "|" << fsusage.files;
            cout << "|" << fsusage.use_percent;
            cout << endl;
        }
        
        ret = sigar_disk_usage_get(sigar, fs.dev_name, &diskusage);
        
        if(ret != SIGAR_OK) {
            cout << "Error with sigar_disk_usage_get" << endl;
        } else {
            
            
            cout << "\t\t\tDISK USAGE";
            cout << "|" << diskusage.reads;
            cout << "|" << diskusage.writes;
            cout << "|" << diskusage.read_bytes;
            cout << "|" << diskusage.write_bytes;
            cout << "|" << diskusage.rtime;
            cout << "|" << diskusage.time;
            cout << "|" << diskusage.wtime;
            cout << "|" << diskusage.snaptime;
            cout << "|" << diskusage.service_time;
            cout << "|" << diskusage.queue;
            cout << "|" << diskusage.wtime;
            cout << "|" << diskusage.wtime;
            cout << endl;
        }
    }
    
	sigar_file_system_list_destroy(sigar, &fslist);
    
    return true;
}

bool ofxActivityMonitor::getMem() {
    sigar_mem_t mem;
	int status;
    
	assert(SIGAR_OK == (status = sigar_mem_get(sigar, &mem)));
    
    cout << "memory" << endl;
    cout << "\tram=" << mem.ram << endl;
    cout << "\ttotal=" << mem.total << endl;;
    cout << "\tused=" << mem.used << endl;;
    cout << "\tfree=" << mem.free << endl;;
    cout << "\tactual_free=" << mem.actual_free << endl;;
    cout << "\tactual_used=" << mem.actual_used << endl;;
    cout << "\tused_percent=" << mem.used_percent << endl;
    cout << "\tfree_percent=" << mem.free_percent << endl;
}

bool ofxActivityMonitor::getNetConn() {
    sigar_net_connection_list_t connlist;
	size_t i;
	int ret;
    
    ret = sigar_net_connection_list_get(sigar, &connlist, 
                                        SIGAR_NETCONN_SERVER | SIGAR_NETCONN_CLIENT |
                                        SIGAR_NETCONN_TCP | SIGAR_NETCONN_UDP);
    
    cout << "# connections =" << connlist.number << endl;
    
		for (i = 0; i < connlist.number; i++) {
			sigar_net_connection_t con = connlist.data[i];
            
            // todo, all of this needs to be converted to better types -- perhaps poco types.
            cout << "\tlocal_port=" << con.local_port << endl;
            
            cout << "\t\t" << con.local_address.addr.in << endl;
            cout << "\t\t" << con.local_address.addr.in6 << endl;
            cout << "\t\t" << con.local_address.addr.mac << endl;
            cout << "\t\t" << con.local_address.family << endl;
            
            cout << "\t\t" << con.remote_address.addr.in << endl;
            cout << "\t\t" << con.remote_address.addr.in6 << endl;
            cout << "\t\t" << con.remote_address.addr.mac << endl;
            cout << "\t\t" << con.remote_address.family << endl;

            cout << "\t" << con.uid << endl;
            cout << "\t" << con.inode << endl;
            cout << "\t" << con.type << endl;
            cout << "\t" << con.state << endl;
            cout << "\t" << con.send_queue << endl;
            cout << "\t" << con.receive_queue << endl;


        }

    sigar_net_connection_list_destroy(sigar, &connlist);
        
}

bool ofxActivityMonitor::getNetif() {
    sigar_net_interface_list_t net_iflist;
	size_t i;
	int ret;
    
    ret = sigar_net_interface_list_get(sigar, &net_iflist);

    if(ret != SIGAR_OK) {
        cout << "well, something went wrong." << endl;
    }

	cout << "# interfaces = " << net_iflist.number << endl;
    
	for (i = 0; i < net_iflist.number; i++) {
		char *ifname = net_iflist.data[i];
		sigar_net_interface_stat_t   ifstat;
		sigar_net_interface_config_t config;
        
        ret = sigar_net_interface_stat_get(sigar, ifname, &ifstat);
        
        if(ret != SIGAR_OK) {
            cout << "problem with this one." << endl;
        }

        cout << "\t" << ifname << endl;
        cout << "\tRX=" << endl;
        cout << "\t" << ifstat.rx_packets << endl;
        cout << "\t" << ifstat.rx_bytes << endl;
        cout << "\t" << ifstat.rx_errors << endl;
        cout << "\t" << ifstat.rx_dropped<< endl;
        cout << "\t" << ifstat.rx_overruns<< endl;
        cout << "\t" << ifstat.rx_frame<< endl;
        cout << "\tTX=" << endl;
        cout << "\t" << ifstat.tx_bytes<< endl;
        cout << "\t" << ifstat.tx_errors<< endl;
        cout << "\t" << ifstat.tx_collisions<< endl;
        cout << "\t" << ifstat.tx_dropped<< endl;
        cout << "\t" << ifstat.tx_overruns<< endl;
        cout << "\t" << ifstat.tx_carrier<< endl;
        cout << "\t" << "other" << endl;
        cout << "\t" << ifstat.speed << endl;
        
        ret = sigar_net_interface_config_get(sigar, ifname, &config);
        
        if(ret != SIGAR_OK) {
            cout << "error loading config info" << endl;
        }
        
        cout << "\t" << config.name << endl;
        cout << "\t" << config.type << endl;
        cout << "\t" << config.description << endl;
        cout << "\t" << config.flags << endl;
        cout << "\t" << config.mtu << endl;
        cout << "\t" << config.metric << endl;


    }
    
    ret = sigar_net_interface_list_destroy(sigar, &net_iflist);
    if(ret != SIGAR_OK) {
        cout << "Error destroying some stuff." << endl;
    }
}


//--------------------------------------------------------------
int ofxActivityMonitor::calcMemoryTotal(){
	
	int mb = 0;
	//int gb = 0;
	
#ifdef TARGET_WIN32
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);
    DWORDLONG totalVirtualMem = memInfo.ullTotalPageFile;
	DWORDLONG physical_memory = memInfo.ullTotalPhys;
	mb = physical_memory/1048576;
#endif
	
#ifdef TARGET_LINUX
    struct sysinfo memInfo;
    sysinfo (&memInfo);
    long long physical_memory = memInfo.totalram;
    physical_memory *= memInfo.mem_unit;
	mb = physical_memory/1048576;	
#endif
	
#ifdef TARGET_OSX
	int64_t physical_memory;
	int mib[2];
	mib[0] = CTL_HW;
	mib[1] = HW_MEMSIZE;
	size_t length = sizeof(int64_t);
	sysctl(mib, 2, &physical_memory, &length, NULL, 0);
	mb = physical_memory/1048576;
	//gb = physical_memory/1073741824;	
#endif
	
	return mb;
}

bool ofxActivityMonitor::getResLimit() {
    sigar_resource_limit_t rlimit;
    int ret = sigar_resource_limit_get(sigar, &rlimit);

    if(ret!=SIGAR_OK) {
        cout << "probs getting resource limits." << endl;
    }
    
    cout << rlimit.core_cur << endl;
    cout << rlimit.core_max << endl;
    cout << rlimit.cpu_cur << endl;
    cout << rlimit.cpu_max << endl;
    cout << rlimit.data_cur << endl;
    cout << rlimit.data_max << endl;
    cout << rlimit.file_size_cur << endl;
    cout << rlimit.file_size_max << endl;
    cout << rlimit.memory_cur << endl;
    cout << rlimit.memory_max << endl;
    cout << rlimit.open_files_cur << endl;
    cout << rlimit.open_files_max << endl;
    cout << rlimit.pipe_size_cur << endl;
    cout << rlimit.pipe_size_max << endl;
    cout << rlimit.processes_cur << endl;
    cout << rlimit.processes_max << endl;
    cout << rlimit.stack_cur << endl;
    cout << rlimit.stack_max << endl;
    cout << rlimit.virtual_memory_cur << endl;
    cout << rlimit.virtual_memory_max << endl;
    

}


bool ofxActivityMonitor::getSwap() {
    sigar_swap_t swap;
    int ret = sigar_swap_get(sigar, &swap);
    
    if(ret!=SIGAR_OK) {
        cout << "swap prob." << endl;
    }
    
    cout << swap.total << endl;
    cout << swap.used << endl;
    cout << swap.free << endl;
    cout << swap.page_in << endl;
    cout << swap.page_out << endl;
    
}

bool ofxActivityMonitor::getSysInfo() {
    sigar_sys_info_t sysinfo;
    int ret = sigar_sys_info_get(sigar, &sysinfo);
    
    if(ret != SIGAR_OK) {
        cout << "uh." << endl;
    }
	cout << sysinfo.name << endl;
	cout << sysinfo.version<< endl;
	cout << sysinfo.arch<< endl;
	cout << sysinfo.machine<< endl;
	cout << sysinfo.description<< endl;
	cout << sysinfo.patch_level<< endl;
	cout << sysinfo.vendor<< endl;
	cout << sysinfo.vendor_version<< endl;
	cout << sysinfo.vendor_name<< endl;
	cout << sysinfo.vendor_code_name<< endl;

}

//--------------------------------------------------------------
vector<float> ofxActivityMonitor::calcCPULoadAvg() {
    int status, i;
    vector<float> results;

#ifdef TARGET_WIN32	
    ofLog(OF_LOG_WARNING,"ofxActivityMonitor::calcCPULoadAvg not implemented for Windows.");
#endif

	sigar_loadavg_t loadavg;
    
	status = sigar_loadavg_get(sigar, &loadavg);
    
    if (status != SIGAR_OK) {
        string errString = "ofxActivityMonitor::";
        errString += "getCpuInfo() ";
        errString += "cpu_list error ";
        errString += ofToString(status) + " ";
        errString += sigar_strerror(sigar, status);
        ofLog(OF_LOG_ERROR,errString);
        return results;
    }

    // otherwise load them up
    results.push_back(loadavg.loadavg[0]);
    results.push_back(loadavg.loadavg[1]);
    results.push_back(loadavg.loadavg[2]);
    
    return results;
}

//--------------------------------------------------------------
int ofxActivityMonitor::calcMemoryUsed(){
    
	
	
}

//--------------------------------------------------------------
int ofxActivityMonitor::calcMemoryUsedProcess(){
	

}

//--------------------------------------------------------------
int ofxActivityMonitor::calcMemoryTotalGPU() {
    /*
    GLuint uNoOfGPUs = wglGetGPUIDsAMD( 0, 0 );
    GLuint* uGPUIDs = new GLuint[uNoOfGPUs];
    wglGetGPUIDsAMD( uNoOfGPUs, uGPUIDs );
    
    GLuint uTotalMemoryInMB = 0;
    wglGetGPUInfoAMD( uGPUIDs[0],
                     WGL_GPU_RAM_AMD,
                     GL_UNSIGNED_INT,
                     sizeof( GLuint ),
                     &uTotalMemoryInMB );
     */
    
    // does not have these extensions?
    
    // http://feedback.wildfiregames.com/report/opengl/
    
    GLint nCurAvailMemoryInKB = -1;
    glGetIntegerv( GL_TEXTURE_FREE_MEMORY_ATI, &nCurAvailMemoryInKB );
    //cout << ">nTEX=>" << nCurAvailMemoryInKB << endl;
    glGetIntegerv( GL_VBO_FREE_MEMORY_ATI, &nCurAvailMemoryInKB );
    //cout << ">nVBO=>" << nCurAvailMemoryInKB << endl;
    glGetIntegerv( GL_RENDERBUFFER_FREE_MEMORY_ATI, &nCurAvailMemoryInKB );
    //cout << ">nRB=>" << nCurAvailMemoryInKB << endl;
    
    //GL_VBO_FREE_MEMORY_ATI
    //GL_RENDERBUFFER_FREE_MEMORY_ATI
//    http://www.geeks3d.com/20100531/programming-tips-how-to-know-the-graphics-memory-size-and-usage-in-opengl/
    
    return nCurAvailMemoryInKB;
    
}

//--------------------------------------------------------------
int ofxActivityMonitor::calcMemoryUsedGPU() {
    //http://feedback.wildfiregames.com/report/opengl/
}   




bool ofxActivityMonitor::uptime() {
    sigar_uptime_t uptime;
	assert(SIGAR_OK == sigar_uptime_get(sigar, &uptime));
	cout << "uptime=" << uptime.uptime << endl;
}

bool ofxActivityMonitor::version() {
    sigar_version_t *version;
	version = sigar_version_get();
	cout << version->major << endl;
	cout << version->minor << endl;
	cout << version->maint << endl;
	cout << version->build << endl;
    
}


bool ofxActivityMonitor::pid() {
        sigar_pid_t pid;
        
        pid = sigar_pid_get(sigar);
        
        cout << pid << "-pid" << endl;
    }

bool ofxActivityMonitor::moreProc() {
    sigar_proc_list_t proclist;
	size_t i;
    
	assert(SIGAR_OK == sigar_proc_list_get(sigar, &proclist));
    
    cout << "# procs=" << proclist.number << endl;
    
	for (i = 0; i < proclist.number; i++) {
		sigar_pid_t pid = proclist.data[i];
		sigar_proc_mem_t proc_mem;
		sigar_proc_time_t proc_time;
		sigar_proc_state_t proc_state;
		int ret;
        
		if (SIGAR_OK == (ret = sigar_proc_mem_get(sigar, pid, &proc_mem))) {
            
            cout << proc_mem.size << endl;
            cout << proc_mem.resident << endl;
            cout << proc_mem.share << endl;
            cout << proc_mem.minor_faults << endl;
            cout << proc_mem.major_faults << endl;
            cout << proc_mem.page_faults << endl;
        }
            
        
		if (SIGAR_OK == (ret = sigar_proc_time_get(sigar, pid, &proc_time))) {
            
            cout << proc_time.start_time << endl;
            cout << proc_time.user << endl;
            cout << proc_time.sys << endl;
            cout << proc_time.total << endl;

        }
        
		if (SIGAR_OK == sigar_proc_state_get(sigar, pid, &proc_state)) {
            
            cout << proc_state.name << endl;
            
            cout << proc_state.state << endl;
            cout << proc_state.ppid << endl;
            cout << proc_state.tty << endl;
            cout << proc_state.priority << endl;
            cout << proc_state.nice << endl;
            cout << proc_state.processor << endl;
            cout << proc_state.threads << endl;
        }
        
        
    }
    sigar_proc_list_destroy(sigar, &proclist);

}


