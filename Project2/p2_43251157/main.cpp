#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/time.h>
#include <string>
#include <vector>
#include <unistd.h>
#include <pthread.h>
#include "types_p2.h"
#include "p2_threads.h"
#include "utils.h"

using namespace std;

pthread_cond_t  cond  = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
struct timeval t_global_start;
vector<Person> menQueue;
vector<Person> womenQueue;

int menInQueue;
int womenInQueue;
int totalQueue;

int menInRestroom;
int womenInRestroom;
int totalRestroom;

int menToCreate;
int womenToCreate;
int total;

int peopleToRestroom;

bool taskIsDone;

Restroom global_restroom;

int main(int argc, char** argv)
{
	// This is to set the global start time
//    gettimeofday(&t_global_start, NULL);
//	pthread_t       tid = 0;
//	int             status = 0;
//	int             work = 0;


	
	// Example code for sleep and class usage
//	Person p1;
//	p1.set_order(1);
//
//	usleep(MSEC(200));
//	p1.start();
//
//
//	usleep(MSEC(150));
//	p1.complete();
	///////////////////////////////////////////
    
    
    if(argc != 2) {
        printf("[ERROR] Expecting 1 argument, but got %d.\n", argc - 1);
        printf("[USAGE] p2_exec <number>\n");
    } else {
        menToCreate = atoi(argv[1]);
        womenToCreate = atoi(argv[1]);
        total = menToCreate + womenToCreate;
        
        menInQueue = 0;
        womenInQueue = 0;
        totalQueue = 0;

        menInRestroom = 0;
        womenInRestroom = 0;
        totalRestroom = 0;
        
        taskIsDone = false;
        
        global_restroom = Restroom();
    }
    
    gettimeofday(&t_global_start, NULL);

    pthread_t input;
    pthread_t manQueue;
    pthread_t womanQueue;
    pthread_create(&input, NULL, inputThread, NULL);
    pthread_create(&manQueue, NULL, man_wants_to_enter, NULL);
    pthread_create(&womanQueue, NULL, woman_wants_to_enter, NULL);
    
//	if(pthread_create(&queueThread, NULL, queueFunc, NULL)) {
//		fprintf(stderr, "Error creating thread\n");
//	}
//
//    if(pthread_create(&restroomThread, NULL, restroomFunc, NULL)) {
//        fprintf(stderr, "Error creating thread\n");
//    }
    
    while(total != peopleToRestroom) {
    
    }
    
    taskIsDone = true;
    pthread_join(input, NULL);
    pthread_join(manQueue, NULL);
    pthread_join(womanQueue, NULL);
    
    
//	usleep(MSEC(10));
//
//	for (int i=0; i<5; i++) {
//		printf("Wake up thread after (%d) seconds\n", (5-i));
//		usleep(MSEC(1000));
//	}
//
//	printf("Wake up thread\n");
//	status = pthread_cond_signal(&cond);
//
//
//
//	/* wait for the second thread to finish */
//	if(pthread_join(tid, NULL)) {
//		fprintf(stderr, "Error joining thread\n");
//	}

	return 0;


}

