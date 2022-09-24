#include <sys/wait.h>
#include <stdio.h>
#include <pthread.h>
#include <stdatomic.h>
#include <signal.h>
#include <unistd.h>
#include <sched.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <sys/resource.h>

int main(int argc, char** argv) {
    int childPid;
    struct rusage resource={0};
    
    childPid = fork();
    if (childPid > 0) {    //parent
        wait4(-1, NULL, 0, &resource);
        printf("voluntary context switches %ld\n", resource.ru_nvcsw);
        printf("involuntary context switches %ld\n", resource.ru_nivcsw);
        printf("total context switches %ld\n", resource.ru_nivcsw+resource.ru_nvcsw);
        printf("soft page faults %ld\n", resource.ru_minflt);
        printf("hard page faults %ld\n", resource.ru_majflt);
        printf("stime %ld\n", resource.ru_stime.tv_sec);
        printf("utime %ld\n", resource.ru_utime.tv_sec);
    } else {
        execvp(argv[1], &argv[1]);
    }
}