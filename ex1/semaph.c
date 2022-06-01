#include "semaph.h"

void semUp(struct sembuf* sop , int num){
    sop->sem_num=num;
    sop->sem_op=1;
    sop->sem_flg=0;
}

void semDown(struct sembuf* sop , int num){
    sop->sem_num=num;
    sop->sem_op=-1;
    sop->sem_flg=0;
}
