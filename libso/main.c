#include<dlfcn.h>
#include<signal.h>
#include<stdarg.h>
#include<sys/unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<math.h>
#include<errno.h>

void recvSignal(int sig)
{
    printf("received signal %d !!!\n",sig);
}

int main(int argc, char* argv[])
{
    signal(SIGSEGV, recvSignal);
    void *handle;
    char *error;
    int (*libmain)(char*, char*);

    if (strcmp(argv[1], "-f") != 0)
    {
        printf("the input parameter is error, the first parameter should be -f\n");
        return;
    }
    if ((strcmp(argv[3], "-p") != 0) && (strcmp(argv[3], "-o") != 0))
    {
        printf("the input parameter is error, the three parameter should be -p or -o\n");
        return;
    }
    if((strcmp(argv[3], "-p") == 0) && (argv[4] != NULL))
    {
        printf("the input parameter is error, the four parameter should be null\n");
        return;
    }
    if((strcmp(argv[3], "-o") == 0) && (argv[4] == NULL))
    {
        printf("the input parameter is error, the four parameter should not be null\n");
        return;
    }

    handle = dlopen("./lib.so", RTLD_LAZY);
    if(!handle)
    {
        fputs(dlerror(),stderr);
        exit(1);
    }
    libmain = dlsym(handle, "main");
    if((error = dlerror()) != NULL)
    {
        fputs(error, stderr);
        exit(1);
    }
    (*libmain)(argv[2], argv[4]);
    dlclose(handle);
    return 0;
}
