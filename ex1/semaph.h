#ifndef __SEMAPH__
#define __SEMAPH__


#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/types.h>




#if ! defined(__FreeBSD__) && ! defined(__OpenBSD__) && \
                ! defined(__sgi) && ! defined(__APPLE__)
                /* Some implementations already declare this union */

union semun {                   /* Used in calls to semctl() */
    int                 val;
    struct semid_ds *   buf;
    unsigned short *    array;
#if defined(__linux__)
    struct seminfo *    __buf;
#endif
};

#endif

void semDown(struct sembuf* sop , int num);
void semUp(struct sembuf* sop , int num);

#endif



//int sem_id;
