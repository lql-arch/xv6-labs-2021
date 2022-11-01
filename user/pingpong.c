#include "../kernel/types.h"
#include "../kernel/stat.h"
#include "user.h"

int main(int argc,char* argv[]){
    int p[2];
    pipe(p);
    int pid = 0;

    if((pid = fork()) == 0){
        close(p[0]);
        if(write(p[1],"1\n",2) > 0){
            fprintf(1,"%d: received ping\n",getpid());
            close(p[1]);
            exit(0); 
        }else{
            exit(-1);
        }
    }else{
        close(p[1]);
        char str[10];
        if(read(p[0],str,2) > 0){
            wait(&pid);
            fprintf(1,"%d: received pong\n",pid);
            close(p[0]);
        }else{
            exit(-1);
        }
    }
    exit(0);
}