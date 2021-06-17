#ifndef __P1_PROCESS
#define __P1_PROCESS

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <pthread.h>
#include <unistd.h>

#include "p1_threads.h"
#include <vector>
#include <cmath>
#include <algorithm>
using namespace std;

extern vector<struct student> globalFile;
extern vector<struct threadInfo> threadRange;

struct student {
    string id;
    double grade;
    string strGrade;
};

struct threadInfo {
    int start;
    int end;
};

vector<struct student> readFile(string fileName);

void get_statistics(std::string class_name[], int num_processes, int num_threads, int fileSize);

void create_process(string class_name[], int num_processes, int num_threads, int fileSize);

void writeFile(string fileName);

vector<double> get_stats(double sum);

double get_mean(double sum);

double get_std(double mean);

double get_median();

bool checkResult();

bool check(string file);

#endif
