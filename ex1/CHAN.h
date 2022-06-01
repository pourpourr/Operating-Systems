#ifndef __CHAN__
#define __CHAN__

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "Memory.h"
#include "semaph.h"

void messageRandomize(SMemory* mem );

#endif
