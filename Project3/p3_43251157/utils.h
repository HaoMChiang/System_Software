#ifndef __UTILS_H
#define __UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/time.h>
#include <string>
#include <vector>
#include <unistd.h>

#define MSEC(x) x*1000


long get_elasped_time(struct timeval& start, struct timeval& end);
long get_time_stamp(void);
void timed_wait_for_task_complition(int timeInMs);


#endif
