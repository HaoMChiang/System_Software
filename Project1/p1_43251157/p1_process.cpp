#include "p1_process.h"

vector<struct student> readFile(string fileName) {
    ifstream ip("input/" + fileName + ".csv");
    if(!ip.is_open()) cout << "Cannot Open File" << endl;
    vector<struct student> result;
    string id;
    string score;
    bool first = true;
    while(ip.good()) {
        std::getline(ip, id, ',');
        std::getline(ip, score, '\n');
        if(first) {
            first = false;
        } else {
            struct student curr;
            curr.id = id;
            curr.strGrade = score;
            curr.grade = strtod(score.c_str(), NULL);
            if(curr.id != "") {
                result.push_back(curr);
            }
        }
    }
    ip.close();
    //cout << "Size of " << fileName << ": " << result.size() << endl;
    return result;
}

double get_mean(double sum) {
    return sum / globalFile.size();
}

double get_std(double mean) {
    double sd = 0;
    
    for(int i = 0; i < globalFile.size(); i++) {
        double grade = globalFile[i].grade;
        sd += pow(grade - mean, 2);
    }
    
    return sqrt(sd/globalFile.size());
}

double get_median() {
    int len = globalFile.size();
    if(len % 2 == 1) {
        return globalFile[len / 2].grade;
    } else {
        return (globalFile[len / 2].grade + globalFile[len / 2 - 1].grade) / 2;
    }
}

vector<double> get_stats(double sum) {
    double mean = get_mean(sum);
    double median = get_median();
    double std = get_std(mean);
    vector<double> stats;
    stats.push_back(mean);
    stats.push_back(median);
    stats.push_back(std);
    return stats;
}

void writeFile(string fileName) {
    //cout << "Size of " << fileName << ": " << globalFile.size() << endl;
    double sum = 0;
    ofstream file;
    file.open("output/" + fileName + "_sorted.csv");
    file << "Rank,Student ID,Grade" << endl;
    for(int i = 0; i < globalFile.size(); i++) {
        sum += globalFile[i].grade;
        file << i + 1 << "," << globalFile[i].id << "," << globalFile[i].strGrade << endl;
    }
    file.close();
    vector<double> stat = get_stats(sum);
    ofstream file2;
    file2.open("output/" + fileName + "_stats.csv");
    file2 << "Average,Median,Std. Dev" << endl;
    file2 << round( stat[0] * 1000.0 ) / 1000.0 << "," << round( stat[1] * 1000.0 ) / 1000.0 << "," << round( stat[2] * 1000.0 ) / 1000.0 << endl;
    file2.close();
    //cout << "Size of " << fileName << ": " << globalFile.size() << endl;
}

void get_statistics(std::string class_name[], int num_processes, int num_threads, int fileSize) {

	// You need to create multiple processes here (each process
	// handles at least one file.)
	
	
	// Each process should use the sort function which you have defined  		
	// in the p1_threads.cpp for multithread sorting of the data. 
	
	// Your code should be able to get the statistics and generate
	// the required outputs for any number of child processes and threads.
    
    //
    create_process(class_name, num_processes, num_threads, fileSize);
}

void create_process(std::string class_name[], int num_processes, int num_threads, int fileSize) {
    
    int parent = getpid();
    int filePerProcess[num_processes];
    
    for(int i = 0; i < num_processes; i++) {
        filePerProcess[i] = 0;
    }
    
    int sizeOfFile = fileSize;
    int index = 0;
    
    while(sizeOfFile > 0) {
        filePerProcess[index % num_processes] += 1;
        index += 1;
        sizeOfFile -= 1;
    }

    int upperRange = fileSize;
    int lowerRange = fileSize;
    int prevLowerRange = fileSize;
    
    for(int i = 0; i < num_processes; i++) {
        lowerRange = prevLowerRange - filePerProcess[i];
        upperRange = prevLowerRange - 1;
        prevLowerRange = lowerRange;
        if(fork() == 0) {
            printf("Child process is created. (pid: %d)\n", getpid());
            break;
        }
    }
    
    if(getpid() !=  parent) {
        for(int j = upperRange; j >= lowerRange; j--) {
            
            globalFile= readFile(class_name[j]);
            threadRange = getRangePerThread(globalFile, num_threads);
            
            pthread_t threads[num_threads];
            for(int k = 0; k < num_threads; k++) {
                int threadNum = k;
                pthread_create(&threads[k], NULL, runner, &threadRange[k]);
            }
        
            for(int c = 0; c < num_threads; c++) {
                pthread_join(threads[c], NULL);
            }
            mergeSort(0, threadRange.size() - 1);
            writeFile(class_name[j]);
//            if(check(class_name[j])) {
//                cout << "Result is correct" << endl;
//            } else {
//                cout << "Result is not correct" << endl;
//            }
        }
        printf("Child process is terminated. (pid: %d)\n", getpid());
        exit(0);
    }
    
    for(int i = 0; i < num_processes; i++) {
        wait(NULL);
    }
}

bool checkResult() {
    double first = globalFile[0].grade;
    for(int i = 1; i < globalFile.size(); i++) {
        if(globalFile[i].grade <= first) {
            first = globalFile[i].grade;
        } else {
            return false;
        }
    }
    return true;
}

bool check(string file) {
    ifstream ip("output/" + file + "_sorted.csv");
    if(!ip.is_open()) cout << "Cannot Open File" << endl;
    string id;
    string score;
    string rank;
    double num = 1000000.0;
    bool first = true;
    while(ip.good()) {
        std::getline(ip, rank, ',');
        std::getline(ip, id, ',');
        std::getline(ip, score, '\n');
        if(first) {
            first = false;
        } else {
            double grade = strtod(score.c_str(), NULL);
            if(grade < num) {
                num = grade;
            } else if(grade == num) {
                num = grade;
                cout << rank << " " << file << endl;
            } else {
                cout << grade << endl;
                ip.close();
                return false;
            }
        }
    }
    ip.close();
    return true;
}

