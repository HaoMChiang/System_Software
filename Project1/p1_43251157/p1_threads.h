#ifndef __P1_THREADS
#define __P1_THREADS

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <pthread.h>

#include "p1_process.h"
using namespace std;

vector<int> threadPerVector(vector<struct student> file, int numThread);

vector<struct threadInfo> rangePerThread(vector<int> thread);

vector<struct threadInfo> getRangePerThread(vector<struct student> file, int numThread);

void mergeSort(int start, int end);

void merge(int start, int mid, int end);

void *runner(void *param);

bool compareStudent(struct student s1, struct student s2);

#endif
