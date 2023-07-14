#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main(){
    char buff[64];
    int num;
    read(0, &num, sizeof(int));
    sprintf(buff,"%d",num);
    write(1,buff,strlen(buff));
}