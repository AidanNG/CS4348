
#include <sys/utsname.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>


int main(void){
    struct utsname sys;
    printf("Operating system name: %s\n",sys.sysname);
    printf("Node name: %s\n",sys.nodename);
    printf("Operating system release: %s\n",sys.release);
    printf("Operating system version: %s\n",sys.version);
    printf("Hardware identifier: %s\n",sys.machine);
    return 0;
}
