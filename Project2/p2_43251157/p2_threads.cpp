#include "p2_threads.h"
#include "utils.h"

extern pthread_cond_t  cond;
extern pthread_mutex_t mutex1;

extern int menToCreate;
extern int womenToCreate;
extern int total;

extern int menInQueue;
extern int womenInQueue;
extern int totalQueue;

extern int menInRestroom;
extern int womenInRestroom;
extern int totalRestroom;

extern bool taskIsDone;

extern int peopleToRestroom;
//void *threadfunc(void *parm)
//{
//
//	int status;
//	printf(" [Thread] Start\n");
//
//	printf(" [Thread] Locks\n");
//	status = pthread_mutex_lock(&mutex);
//
//    printf(" [Thread] Blocked\n");
//    status = pthread_cond_wait(&cond, &mutex);
//
//	printf(" [Thread] Starts again.\n");
//	for (int i=0; i<3; i++) {
//		printf(" [Thread] Complete thread after (%d) seconds\n", (3-i));
//		usleep(MSEC(1000));
//	}
//
//	printf(" [Thread] Unlocks\n");
//	status = pthread_mutex_unlock(&mutex);
//	printf(" [Thread] Complete\n");
//    return 0;

//}

void *inputThread(void *parm) {
    long timeElasped = -1;
    time_t t;
    srand((unsigned)time(&t));
    struct timeval currTime;
    
    for(int i = 0; i < total; i++) {
        
        int random = rand() % 2;
        Person current = Person();
        int gender = -1;
        string strGender = "";
        
        if(random == 0 && menToCreate > 0) {
            menToCreate--;
            gender = 0;
            strGender = "Man";
        } else if (random == 1 && womenToCreate > 0){
            womenToCreate--;
            gender = 1;
            strGender = "Woman";
        } else if (menToCreate == 0){
            womenToCreate--;
            gender = 1;
            strGender = "Woman";
        } else if (womenToCreate == 0) {
            menToCreate--;
            gender = 0;
            strGender = "Man";
        }
        current.set_gender(gender);
        current.set_strGender(strGender);
        current.set_time(3 + (rand() % ( 10 - 3 + 1 )));
        unsigned int randomTime = rand() % 5 + 1;
        gettimeofday(&currTime, NULL);
        timeElasped = get_elasped_time(t_global_start, currTime);
        pthread_mutex_lock(&mutex1);
        if(gender == 0) {
            cout << "[" << timeElasped << " ms][Input] A person (Man) goes into the queue" <<  endl;
            menInQueue++;
            totalQueue++;
            menQueue.push_back(current);
        } else if(gender == 1) {
            cout << "[" << timeElasped << " ms][Input] A person (Woman) goes into the queue" << endl;
            womenInQueue++;
            totalQueue++;
            womenQueue.push_back(current);
        }
        pthread_mutex_unlock(&mutex1);
        usleep(MSEC(randomTime));
    }
    return 0;
}

void *man_wants_to_enter(void *parm) {
    struct timeval currTime;
    while(!taskIsDone) {
        if(menQueue.size() != 0 && global_restroom.get_status() != 1) {
            global_restroom.set_status(2);
            Person man = menQueue.at(0);
            menQueue.erase(menQueue.begin());
            pthread_mutex_lock(&mutex1);
            menInRestroom++;
            totalRestroom++;
            menInQueue--;
            totalQueue--;
            pthread_mutex_unlock(&mutex1);
            gettimeofday(&currTime, NULL);
            long time = get_elasped_time(t_global_start, currTime);
            printf("[%ld ms][Queue] Send (Man) into the restroom (Stay %ld ms), Status: Total: %d (Men: %d, Women: %d)\n", time, man.get_time(), totalQueue, menInQueue, womenInQueue);
            printf("[%ld ms][Restroom] (Man) goes into the restroom, State is (MenPresent): Total: %d (Men: %d, Women: %d)\n", time, totalRestroom, menInRestroom, 0);
            pthread_t t;
            long a = man.get_time();
            pthread_create(&t, NULL, man_leaves, &a);
        }
    }
    return 0;
}

void *woman_wants_to_enter(void *parm) {
    struct timeval currTime;
    while(!taskIsDone) {
        if(womenQueue.size() != 0 && global_restroom.get_status() != 2) {
            global_restroom.set_status(1);
            Person woman = womenQueue.at(0);
            womenQueue.erase(womenQueue.begin());
            pthread_mutex_lock(&mutex1);
            womenInRestroom++;
            totalRestroom++;
            womenInQueue--;
            totalQueue--;
            pthread_mutex_unlock(&mutex1);
            gettimeofday(&currTime, NULL);
            long time = get_elasped_time(t_global_start, currTime);
            printf("[%ld ms][Queue] Send (Woman) into the restroom (Stay %ld ms), Status: Total: %d (Men: %d, Women: %d)\n", time, woman.get_time(), totalQueue, menInQueue, womenInQueue);
            printf("[%ld ms][Restroom] (Woman) goes into the restroom, State is (WomenPresent): Total: %d (Men: %d, Women: %d)\n", time, totalRestroom, 0, womenInRestroom);
            pthread_t t;
            long a = woman.get_time();
            pthread_create(&t, NULL, woman_leaves, &a);
        }
    }
    return 0;
}

void *man_leaves(void *parm) {
//    pthread_mutex_lock(&mutex1);
//    struct timeval now;
//    gettimeofday(&now, NULL);
//    cout << "Enter Man Leaves " << now.tv_sec * 1000 + now.tv_usec / 1000 << endl;
//    pthread_mutex_unlock(&mutex1);

    //Person *curr = (Person*)parm;
    usleep(MSEC((unsigned int)(*(long*)parm)));
    
    pthread_mutex_lock(&mutex1);
    struct timeval currTime;
    gettimeofday(&currTime, NULL);
    long time = (get_elasped_time(t_global_start, currTime));
    //cout << "End Man Leaves " << currTime.tv_sec * 1000 + currTime.tv_usec / 1000 << endl;
    totalRestroom--;
    menInRestroom--;
    if(totalRestroom == 0) {
        global_restroom.set_status(0);
        printf("[%ld ms][Restroom] (Man) left the restroom. Status is changed, Status is (empty): Total: 0 (Men: 0, Women: 0)\n", time);
    } else {
        printf("[%ld ms][Restroom] (Man) left the restroom. Status is changed, Status is (MenPresent): Total: %d (Men: %d, Women: %d)\n", time, totalRestroom, menInRestroom, 0);
    }
    peopleToRestroom++;
    
    pthread_mutex_unlock(&mutex1);
    return 0;
}

void *woman_leaves(void *parm) {
//    pthread_mutex_lock(&mutex1);
//    struct timeval now;
//    gettimeofday(&now, NULL);
//    cout << "Enter Woman Leaves " << now.tv_sec * 1000 + now.tv_usec / 1000 << endl;
//    pthread_mutex_unlock(&mutex1);
    
    //Person *curr = (Person*)parm;
    //usleep(MSEC((unsigned int)curr -> get_time()));
    usleep(MSEC((unsigned int)(*(long*)parm)));
    pthread_mutex_lock(&mutex1);
    struct timeval currTime;
    gettimeofday(&currTime, NULL);
    long time = get_elasped_time(t_global_start, currTime);
    //cout << "End Woman Leaves " << currTime.tv_sec * 1000 + currTime.tv_usec / 1000 << endl;
    totalRestroom--;
    womenInRestroom--;
    if(totalRestroom == 0) {
        global_restroom.set_status(0);
        printf("[%ld ms][Restroom] (Woman) left the restroom. Status is changed, Status is (empty): Total: 0 (Men: 0, Women: 0)\n", time);
    } else {
        printf("[%ld ms][Restroom] (Woman) left the restroom. Status is changed, Status is (WomenPresent): Total: %d (Men: %d, Women: %d)\n", time, totalRestroom, 0, womenInRestroom);
    }
    peopleToRestroom++;
    pthread_mutex_unlock(&mutex1);
    return 0;
}
