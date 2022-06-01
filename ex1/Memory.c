#include "Memory.h"

void CopyString(SMemory* mem,char* str){
    int i;
    for(i=0; i<strlen(str); i++){
        mem->message[i]=str[i];
    }
    mem->message[i]='\0';
    mem->ChangeCounter=mem->ChangeCounter+2;

}


int getSemId(SMemory* mem){
    return mem->sem_id;
}
void setSemId(SMemory* mem,int id){
    mem->sem_id=id;
}

int makeSpace(int key){

    int shm_id;
    shm_id = shmget((key_t) key, sizeof(SMemory), IPC_CREAT | 0666);
    if (shm_id<0 ){
        printf("shmget error \n" );

    }
    else {
        printf("space made with id = %d\n", shm_id );
        return shm_id;
    }
}

unsigned long hash(char *str)
{
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}
