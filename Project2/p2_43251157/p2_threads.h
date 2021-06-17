#ifndef __P2_THREADS_H
#define __P2_THREADS_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/time.h>
#include <string>
#include <vector>
#include <unistd.h>
#include "types_p2.h"

#include "utils.h"
using namespace std;

extern struct timeval t_global_start;
//extern struct timeval currTime;
extern Restroom global_restroom;
extern vector<Person> queue;
extern vector<Person> menQueue;
extern vector<Person> womenQueue;

//void *threadfunc(void *parm);

void *man_wants_to_enter(void *parm);
void *woman_wants_to_enter(void *parm);
void *man_leaves(void *parm);
void *woman_leaves(void *parm);
void *signalFunc(void *parm);

void *inputThread(void *parm);
//void *queueThread(void *parm);

#endif
