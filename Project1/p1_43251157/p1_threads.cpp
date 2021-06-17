#include "p1_threads.h"

// This file includes functions that actually perform the
// computation. You can create a thread with these function
// or directly call this function

vector<int> threadPerVector(vector<struct student> file, int numThread) {
    vector<int> result;
    int len = file.size();
    int dataPerSeg = len / numThread;
    for(int i = 0; i < numThread; i++) {
        result.push_back(dataPerSeg);
    }
    int remainder = len % numThread;
    for(int i = 0; i < remainder; i++) {
        result[i] += 1;
    }
    return result;
}

vector<struct threadInfo> rangePerThread(vector<int> thread) {
    vector<struct threadInfo> result;
    int upperLimit = -1;
    for(int i = 0; i < thread.size(); i++) {
        struct threadInfo curr;
        curr.start = upperLimit + 1;
        curr.end = upperLimit + thread[i];
        upperLimit += thread[i];
        result.push_back(curr);
    }
    return result;
}

vector<struct threadInfo> getRangePerThread(vector<struct student> file, int numThread) {
    vector<int> numElementPerThread = threadPerVector(file, numThread);
    return rangePerThread(numElementPerThread);
}

void mergeSort(int start, int end) {
    if(start == end) {
        return;
    }
    int mid = start + (end - start) / 2;
    mergeSort(start, mid);
    mergeSort(mid + 1, end);
    merge(start, mid, end);
}

void merge(int start, int mid, int end) {
    vector<struct student> left;
    vector<struct student> right;
    
    int leftStart = threadRange[start].start;
    int leftEnd = threadRange[mid].end;
    int rightStart = threadRange[mid + 1].start;
    int rightEnd = threadRange[end].end;
    
    int leftLen = leftEnd - leftStart + 1;
    int rightLen = rightEnd - rightStart + 1;
    
    for(int i = 0; i < leftLen; i++) {
        struct student curr;
        curr.id = globalFile[i + leftStart].id;
        curr.grade = globalFile[i + leftStart].grade;
        curr.strGrade = globalFile[i + leftStart].strGrade;
        left.push_back(curr);
        
    }
    
    for(int i = 0; i < rightLen; i++) {
        struct student curr;
        curr.id = globalFile[i + rightStart].id;
        curr.grade = globalFile[i + rightStart].grade;
        curr.strGrade = globalFile[i + rightStart].strGrade;
        right.push_back(curr);
    }
    
    int leftPointer = 0;
    int rightPointer = 0;
    int filePointer = leftStart;
    
    while(leftPointer < leftLen && rightPointer < rightLen) {
        double leftGrade = left[leftPointer].grade;
        double rightGrade = right[rightPointer].grade;
        if(leftGrade > rightGrade) {
            globalFile[filePointer].id = left[leftPointer].id;
            globalFile[filePointer].grade = leftGrade;
            globalFile[filePointer].strGrade = left[leftPointer].strGrade;
            leftPointer++;
        } else {
            globalFile[filePointer].id = right[rightPointer].id;
            globalFile[filePointer].grade = rightGrade;
            globalFile[filePointer].strGrade = right[rightPointer].strGrade;
            rightPointer++;
        }
        filePointer++;
    }
    
    while(leftPointer < leftLen) {
        globalFile[filePointer].id = left[leftPointer].id;
        globalFile[filePointer].grade = left[leftPointer].grade;
        globalFile[filePointer].strGrade = left[leftPointer].strGrade;
        leftPointer++;
        filePointer++;
    }
    
    while(rightPointer < rightLen) {
        globalFile[filePointer].id = right[rightPointer].id;
        globalFile[filePointer].grade = right[rightPointer].grade;
        globalFile[filePointer].strGrade = right[rightPointer].strGrade;
        rightPointer++;
        filePointer++;
    }
}

bool compareStudent(struct student s1, struct student s2) {
    return s1.grade > s2.grade;
}

void *runner(void *param) {
    struct threadInfo *curr = (struct threadInfo*) param;
    sort(globalFile.begin() + curr -> start, globalFile.begin() + (curr -> end) + 1, compareStudent);
    pthread_exit(0);
}
