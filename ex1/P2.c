#include "P2.h"
#define KEY 1925

int main(){

    int key=KEY;
    int shm_id= makeSpace(key);
    SMemory* mem= (SMemory*) shmat(shm_id, NULL, 0);

    int sema_id = getSemId(mem);
    union semun arg;
    arg.val=0;
    semctl(sema_id, 2, SETVAL, arg);
    struct sembuf* sop=malloc(sizeof(struct sembuf));
    char* temp= malloc(20*sizeof(char));

    while(strcmp(mem->message,"TERM")!=0){
        printf("P1 says: %s\n",mem->message );
        printf("Give new message\n" );
        scanf("%s",temp );
        CopyString(mem,temp);
        semUp(sop,1);
        semop(sema_id,sop,1);
        if((strcmp(mem->message,"TERM")==0)){
            break;
        }
        semDown(sop,2);
        semop(sema_id,sop,1);




    }
    free(temp);
    free (sop);

}
