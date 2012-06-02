#include "ofxActivityMonitor.h"

int getcputime(void)        
{ 
    //struct timeval tim;        
    struct rusage ru;
    
    getrusage(RUSAGE_SELF, &ru);        
    //tim=ru.ru_utime;
    long t = ru.ru_ixrss;
    
    //double t=(double)tim.tv_sec + (double)tim.tv_usec / 1000000.0;        
    
    //tim=ru.ru_stime;        
    
    //t+=(double)tim.tv_sec + (double)tim.tv_usec / 1000000.0;        
    
    return t;
}


void process_mem_usage(double& vm_usage, double& resident_set)
{
    using std::ios_base;
    using std::ifstream;
    using std::string;
    
    vm_usage     = 0.0;
    resident_set = 0.0;
    
    // 'file' stat seems to give the most reliable results
    //
    ifstream stat_stream("/proc/self/stat",ios_base::in);
    
    // dummy vars for leading entries in stat that we don't care about
    //
    string pid, comm, state, ppid, pgrp, session, tty_nr;
    string tpgid, flags, minflt, cminflt, majflt, cmajflt;
    string utime, stime, cutime, cstime, priority, nice;
    string O, itrealvalue, starttime;
    
    // the two fields we want
    //
    unsigned long vsize;
    long rss;
    
    stat_stream >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr
    >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt
    >> utime >> stime >> cutime >> cstime >> priority >> nice
    >> O >> itrealvalue >> starttime >> vsize >> rss; // don't care about the rest
    
    stat_stream.close();
    
    long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
    vm_usage     = vsize / 1024.0;
    resident_set = rss * page_size_kb;
}


void ofxActivityMonitor::test()
{
//    using std::cout;
//    using std::endl;
//    
//    double vm = -1;
//    double rss = -1;
//    process_mem_usage(vm, rss);
//    cout << "VM: " << vm << "; RSS: " << rss << endl;

//#include <stdio.h>
//#include <proc/readproc.h>

//    struct proc_t usage;
//    look_up_our_self(&usage);
//    printf("usage: %lu\n", usage.vsize);
    
    short int i, n, a;
    /*double*/long t;
    
    n = 13;
    printf("The 7-times Table\n");
    
    for(i=1;i<n;i++)
    {
        a = (i * 7);
        printf("\n%d\t%d", i, a);
    }
    
    printf("\n");
    t = getcputime();
    printf("The total CPU time comsumed by the current process is: %lf (seconds)\n", t);
    //printf("%ld\n",t);


}



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
    ofAddListener(ofEvents().update,this,&ofxActivityMonitor::update);
    // we'll never unregister b/c this is a singleton
    
    
    numCPUCores             = calcNumCpuCores();
    
    
    memoryLastUpdate        = -INT_MAX;
    memoryUpdateInterval    = 10000;
    memoryTotal             = 0;
	memoryUsed              = 0;
	memoryUsedProcess       = 0;	
	
    
    memoryGPULastUpdate     = -INT_MAX;
    memoryGPUUpdateInterval = 10000;
    memoryTotalGPU          = 0;
    memoryUsedGPU           = 0;
    

    
    vendorGPU               = (char*)glGetString(GL_VENDOR);
    rendererGPU             = (char*)glGetString(GL_RENDERER);
    versionGPU              = (char*)glGetString(GL_VERSION);
    shadingLangVersionGPU   = (char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
    
    
    string s                = (char*)glGetString(GL_EXTENSIONS);
    
    cout << s << endl;
    
    // https://github.com/codykrieger/gfxCardStatus/blob/master/Classes/SystemInfo.m
    
    /*RegularExpression re(" ");
    re.split(s, 0, extensionsGPU);
     */


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



int ofxActivityMonitor::calcNumCpuCores() {
	
	int nCores = -1;
	
#ifdef TARGET_WIN32
    SYSTEM_INFO sysinfo;
    GetSystemInfo( &sysinfo );
    
    nCores = sysinfo.dwNumberOfProcessors;
#endif
	
#ifdef TARGET_LINUX
    nCores = sysconf( _SC_NPROCESSORS_ONLN );
#endif
	
#ifdef TARGET_OSX
    nCores = sysconf( _SC_NPROCESSORS_ONLN );
#endif
	
	return nCores;
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

//--------------------------------------------------------------
vector<float> ofxActivityMonitor::calcCPULoadAvg() {
    vector<float> results;
	
#ifdef TARGET_WIN32
#endif
	
#ifdef TARGET_LINUX
    double loadavg[3];
    getloadavg(loadavg, 3);
    for(int i = 0; i < 3; i++) results.push_back(loadavg[i]);
#endif
	
#ifdef TARGET_OSX
    double loadavg[3];
    getloadavg(loadavg, 3);
    for(int i = 0; i < 3; i++) results.push_back(loadavg[i]);
#endif
    return results;
}

//--------------------------------------------------------------
int ofxActivityMonitor::calcMemoryUsed(){
    
	int mb = 0;
	//int gb = 0;
    
#ifdef TARGET_WIN32	
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);
    DWORDLONG used_memory = memInfo.ullTotalPhys - memInfo.ullAvailPhys;
	mb = used_memory/1048576;	
#endif
	
#ifdef TARGET_LINUX
    struct sysinfo memInfo;
    sysinfo (&memInfo);
    long long used_memory = memInfo.totalram;
    used_memory *= memInfo.mem_unit;
	mb = used_memory/1048576;
#endif
	
#ifdef TARGET_OSX
	
	vm_size_t page_size;
	mach_port_t mach_port;
	mach_msg_type_number_t count;
	vm_statistics_data_t vm_stats;
	int64_t used_memory = 0;
	int64_t myFreeMemory = 0;
	
	mach_port = mach_host_self();
	count = sizeof(vm_stats) / sizeof(natural_t);
	if (KERN_SUCCESS == host_page_size(mach_port, &page_size) &&
		KERN_SUCCESS == host_statistics(mach_port, HOST_VM_INFO, 
										(host_info_t)&vm_stats, &count))
	{
		myFreeMemory = (int64_t)vm_stats.free_count * (int64_t)page_size;
		
		used_memory = ((int64_t)vm_stats.active_count + 
					   (int64_t)vm_stats.inactive_count + 
					   (int64_t)vm_stats.wire_count) *  (int64_t)page_size;
		mb = used_memory/1048576;
	}
	
#endif
	
	return mb;
	
}

//--------------------------------------------------------------
int ofxActivityMonitor::calcMemoryUsedProcess(){
	
	int mb = 0;
	//int gb = 0;
	
#ifdef TARGET_WIN32
    PROCESS_MEMORY_COUNTERS_EX pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
    SIZE_T virtualMemUsedByMe = pmc.PrivateUsage;
    SIZE_T used_memory = pmc.WorkingSetSize;
	mb = used_memory/1048576;	
#endif
	
#ifdef TARGET_LINUX
	FILE* file = fopen("/proc/self/status", "r");
	int result = -1;
	char line[128];
	while (fgets(line, 128, file) != NULL){
		if (strncmp(line, "VmRSS:", 6) == 0) result = parseLine(line);
		break;
	}
	fclose(file);
	mb = result/1024;
#endif
	
#ifdef TARGET_OSX
	struct task_basic_info t_info;
	mach_msg_type_number_t t_info_count = TASK_BASIC_INFO_COUNT;
	
	if (KERN_SUCCESS != task_info(mach_task_self(),
								  TASK_BASIC_INFO, (task_info_t)&t_info, 
								  &t_info_count))
	{
		return -1;
	}
	mb = t_info.resident_size/1048576;
#endif
	
	return mb;
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