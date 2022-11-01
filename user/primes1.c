#include "../kernel/types.h"
#include "../kernel/stat.h"
#include "user.h"

void primes(int *pip);

int main(){
    int pip[2];
    int pid = 0;
    int nums[] = {2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35};

    pipe(pip);
    int t = 0;
    for(int i = 0 ; i < 34 ; i++){
        if(nums[i] == 0)
            continue;
        if(t == 0){
            t = nums[i];
            nums[i] = 0;
            continue;
        }
        if(nums[i] % t == 0 ){
            nums[i] = 0;
        }
    }
    printf("prime %d\n",t);

    primes(pip);

    write(pip[1],nums,sizeof(nums));
    close(pip[1]);
    close(pip[0]);

    wait((int*)0);
    
    exit(0);
}

void primes(int *pip){
    if(fork() == 0){
        int pid = getpid();
        int* nums = (int*)malloc(sizeof(int) * 34);
        int t = 0;
        if( 0 > read(pip[0],nums,sizeof(int) * 34)){
            exit(-1);
        }
        for(int i = 0 ; i < 34 ; i++){
            if(nums[i] == 0)
                continue;
            if(t == 0 && nums[i] != 0){
                t = nums[i];
                nums[i] = 0;
                continue;
            }
            if(nums[i] % t == 0 ){
                nums[i] = 0;
            }
        }
        if(nums[33] == 0 && t == 0){
            exit(0);
        }
        printf("prime %d\n",t);
        primes(pip);
        write(pip[1],nums,sizeof(int) * 34);
        close(pip[0]);
        close(pip[1]);
        
        wait(&pid);
        free(nums);
    }
}