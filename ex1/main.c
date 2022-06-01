#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "P1.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

int main(){
    int shm_id;
    shm_id = shmget((key_t)getpid(), 87*sizeof(char), IPC_CREAT | 0666);
    if (shm_id<0 ){
        printf("shmget error \n" );
        exit(1);
    }
    printf("%d\n",shm_id );
    char* message=  shmat (shm_id, (char *) 0, 0);

    message="dsddddddddddddd555";
    printf("%lu xmm\n",sizeof(message) );
    //message="df";
    char *args[] = {message};
    execl("./P1", args);
    printf("%s xmmm\n",message );

    return 0;

}
