#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<wait.h>
#include<signal.h>
#include <sys/types.h>
#include <sys/wait.h>

void usage();

void error();

int main(int argc, char*argv[]) {
    int n = atoi(argv[1]);
    for(int i=0; i<n; ++i) {
        int ret = fork();
    }
}
