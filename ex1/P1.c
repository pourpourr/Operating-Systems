#include "P1.h"
#define KEY 1234

int main(int argc, char *argv[]){





    int key=KEY;

    int shm_id= makeSpace(key); // create share memory spce
    SMemory* mem= (SMemory*) shmat(shm_id, NULL, 0); //get a pointer to that space
    int sema_id=semget( (key_t) key, 3, (IPC_CREAT | 0660)); //create a semaphore
    if(sema_id<0){
        printf("Semaphore creation failed\n" );
        exit(5);
    }else {
        printf("semaphore created with sema_id = %d\n",sema_id );
        setSemId(mem,sema_id);
    }
    mem->posibol=atoi(argv[1]);
    mem->ChangeCounter=-1;
    union semun arg;
    arg.val=0;
    semctl(sema_id, 0, SETVAL, arg);  //P1's value =0
    struct sembuf* sop=malloc(sizeof(struct sembuf));


    printf("Give first message\n" );
    char* temp= malloc(20*sizeof(char));
    scanf("%s", temp);
    CopyString(mem,temp);
    while(strcmp(mem->message,"TERM")!=0){

        semUp(sop,1);   //ENC1 "wakes up"
        semop(sema_id,sop,1);
        semDown(sop,0);     //P1 "sleeps"
        semop(sema_id,sop,1);
        if((strcmp(mem->message,"TERM")==0)){
            break;
        }
        printf("P2 says: %s\n",mem->message );
        printf("Give new message\n" );
        scanf("%s",temp );
        CopyString(mem,temp);
    }


    free(temp);                     //freeing any memory allocated
    free (sop);
    shmctl(shm_id,0, IPC_RMID);
    semctl(sema_id,0,IPC_RMID,0);


}




































































/*
    int sema_id ;
    if(sema_id=semget( IPC_PRIVATE, 1, (IPC_CREAT | 0660))<0){
        printf("Semaphore creation failed\n" );
    }
*/

/*
    int semResult;
    union senum u;
    u.val=1;
    semResult=semctl(sema_id,0,SETVAL,u);
    char* temp= malloc(20*sizeof(char));



    sprintf(temp,"%d",semResult );
    printf("dasdsasad\n" );
    perror(temp);
    printf("dasdsasad\n" );
    free(temp);

    */






//    semctl(sema_id,0,IPC_RMID,0);









    /*
    key_t mem_key;
    char* message;
    char*
    message= ;
    printf("give message\n" );
    scanf("%s",message );
    printf("give posibol (0-100)\n" );
    scanf("%d",posibol );
    //char* argENC=message;

    char *programName = "./ENC1";
    char *argsForExec[] = {programName , message, posibol , NULL};
    int outcome=execvp(programName, argsForExec);

    printf("%d\n",outcome );
*/
