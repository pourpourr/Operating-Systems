#include "ENC1.h"
#define KEY 1234

unsigned long hash( char *str);

int main(int argc, char *argv[]){

    int key=KEY;
    int shm_id= makeSpace(key);
    SMemory* mem= (SMemory*) shmat(shm_id, NULL, 0);

    int sema_id = getSemId(mem);
    union semun arg;
    arg.val=0;

    semctl(sema_id, 1, SETVAL, arg);
    struct sembuf* sop=malloc(sizeof(struct sembuf));
    int tempHash;
    char* backup= malloc(20*sizeof(char));

    //// P1 does stuff
    while(strcmp(mem->message,"TERM")!=0){
        mem->hashValue= hash(mem->message);
        strcpy(backup,mem->message);

        do{
            mem->badNews=0;
            strcpy(mem->message,backup);
            semUp(sop,2);         // SIGNAL CHAN
            semop(sema_id,sop,1);
            semDown(sop,1);         // ENC1 WAITS
            semop(sema_id,sop,1);
        }while(mem->badNews!=0);
        printf("%s\n",mem->message );
        tempHash= hash(mem->message);
        while (tempHash!= mem->hashValue){
            mem->badNews=1;
            semUp(sop,2);              //SIGNAL CHAN
            semop(sema_id,sop,1);
            semDown(sop,1);             // ENC1 WAITS
            semop(sema_id,sop,1);
            tempHash= hash(mem->message);
        }

        semUp(sop,0);
        semop(sema_id,sop,1);
        semDown(sop,1);
        semop(sema_id,sop,1);

    }
    free (sop);
    free(backup);


}
