#include "PlatformSpecific.h"

#ifdef _WIN32
#include <windows.h>
int getNumThreads(){
    SYSTEM_INFO sysinfo;
    GetSystemInfo( &sysinfo );
    return sysinfo.dwNumberOfProcessors;
}

long getTimeSeed(){
    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);
    return ft.dwLowDateTime;
}

#endif // _WIN32

#ifdef linux
#include <unistd.h>
#include <time.h>

int getNumThreads(){
    return sysconf( _SC_NPROCESSORS_ONLN );
}

long getTimeSeed(){
    struct timespec* tp = new timespec;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, tp);
    long res = tp->tv_nsec;
    delete tp;
    return res;
}
#endif
