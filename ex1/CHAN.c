#include "CHAN.h"

#define KEY1 1234
#define KEY2 1925

char Rsymbols[] = { 'a','b','c','d','e','f','g','h','i','j','0','1','2','3','4','5','6','7','8','9' };

int main(){
    srand(time(NULL));
    int key1=KEY1;
    int key2=KEY2;
    int shm_id1= makeSpace(key1);
    int shm_id2= makeSpace(key2);
    SMemory* mem1= (SMemory*) shmat(shm_id1, NULL, 0);
    SMemory* mem2= (SMemory*) shmat(shm_id2, NULL, 0);

    int sema_id1 = getSemId(mem1);

    int sema_id2=semget( (key_t) key2, 3, (IPC_CREAT | 0660));
    if(sema_id2<0){
        printf("Semaphore creation failed\n" );
        exit(5);
    }else {
        printf("semaphore created with sema_id = %d\n",sema_id2 );
        setSemId(mem2,sema_id2);
    }
    setSemId(mem2,sema_id2 );
    mem2->posibol=mem1->posibol;
    mem2->ChangeCounter=0;
    mem2->badNews=0;
    union semun arg;
    arg.val=0;
    semctl(sema_id1, 2, SETVAL, arg);
    semctl(sema_id2, 0, SETVAL, arg);

    struct sembuf* sop=malloc(sizeof(struct sembuf));
    int i;
    while(1==1) {



        if ((mem1->ChangeCounter)>(mem2->ChangeCounter)){  // most recent message is form P1
            if(mem2->badNews==1) {
                mem1->badNews=1;
                semUp(sop,1);
                semop(sema_id1,sop,1);
                semDown(sop,2);
                semop(sema_id1,sop,1);
            }
            printf("P1 message before messageRandomize %s\n", mem1->message );

            if((strcmp(mem1->message,"TERM")!=0)){
                messageRandomize(mem1);
            }
            printf("P1 message after messageRandomize %s\n", mem1->message );

            strcpy(mem2->message,mem1->message);
            mem2->hashValue=mem1->hashValue;
            semUp(sop,1);
            semop(sema_id2,sop,1);

            if((strcmp(mem1->message,"TERM")==0)){
                break;
            }
            semDown(sop,0);
            semop(sema_id2,sop,1);

        }else{
            printf("P2 message before messageRandomize %s\n", mem2->message );

            if((strcmp(mem2->message,"TERM")!=0)){
                messageRandomize(mem2);
            }
            printf("P2 message after messageRandomize %s\n", mem2->message );

            if(mem1->badNews==1) {
                mem2->badNews=1;
                semUp(sop,1);
                semop(sema_id2,sop,1);
                semDown(sop,0);
                semop(sema_id2,sop,1);
            }
            strcpy(mem1->message,mem2->message);
            mem1->hashValue=mem2->hashValue;
            semUp(sop,1);
            semop(sema_id1,sop,1);
            if((strcmp(mem2->message,"TERM")==0)){
                break;
            }
            semDown(sop,2);
            semop(sema_id1,sop,1);


        }

    }


    free (sop);
    shmctl(shm_id2,0, IPC_RMID);
    semctl(sema_id2,0,IPC_RMID,0);
}

void messageRandomize(SMemory* mem ){

    int i;
    if((rand()%101 ) > mem->posibol ){
        for(i=0; i<strlen(mem->message); i++){
            mem->message[i]= Rsymbols[rand()%20];
        }

    }

}
