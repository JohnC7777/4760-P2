#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <time.h>
#include <sys/shm.h>
#include "config.h"

void lock(int);
void use_resource(int);
void unlock(int);
char *getPerror(char*);
void logMessage(char*, int, char*);


struct shmseg {
	int source;
	int tickets[NUM_OF_PROCS];
	int choosing[NUM_OF_PROCS];
};
struct shmseg *shmp;


int main (int argc, char *argv[]) {
	int procNum = atoi(argv[1]) + 1;
	int shmid = atoi(argv[2]);
	
	int randNumsLower = 1;
	int randNumsUpper = 5;
  int randNums;
	srand(time(0) * procNum); 
	
	shmp = shmat(shmid, NULL, 0);
	if (shmp == (void *) -1) {
		char *output = getPerror(argv[0]);
		perror(output);
		exit(1);
	}
	
	/* Set logfile name */
	char intToString[3];
	sprintf(intToString, "%d", procNum);
	char logfile[10] = "logfile.";
	strcat(logfile, intToString);
	
	
	                //Bakery's algorithm
	int i = 0;
	for (i = 0; i < 5; i++) {
		logMessage("Requested to join critical section by process number: ", procNum, logfile);
		lock(procNum - 1); 
		logMessage("Entered critical section by process number: ", procNum, logfile);
		randNums = (rand() % (randNumsUpper - randNumsLower + 1)) + randNumsLower; 
		sleep(randNums);
		use_resource(procNum - 1);
		logMessage("Wrote in 'cstest' file by process number: ", procNum, logfile);
		randNums = (rand() % (randNumsUpper - randNumsLower + 1)) + randNumsLower;
		sleep(randNums);
		logMessage("Exited critical section by process number: ", procNum, logfile);
		unlock(procNum - 1); /* Exit critical section */
	}
	
	
        return 0;
}

void lock(int procNum) {
	shmp->choosing[procNum] = 1;
	MEMBAR; 
	
	int max_ticket = 0;

	int i;
	for (i = 0; i < NUM_OF_PROCS; ++i) {
		int ticket = shmp->tickets[i];
		max_ticket = ticket > max_ticket ? ticket : max_ticket;
	}
	
	shmp->tickets[procNum] = max_ticket + 1;
	
	MEMBAR;
	shmp->choosing[procNum] = 0;
	MEMBAR;
	
	int other;
	for (other = 0; other < NUM_OF_PROCS; ++other) {
		
		while (shmp->choosing[other]) {
		}
		
		MEMBAR;
		
		while (shmp->tickets[other] != 0 && (shmp->tickets[other] < shmp->tickets[procNum] || (shmp->tickets[other] == shmp->tickets[procNum] && other < procNum))) {
		}
		
	}
}

// EXIT
void unlock(int procNum) {
	MEMBAR;
	shmp->tickets[procNum] = 0;
}


//CRITICAL SECTION
void use_resource(int procNum) {
	if (shmp->source != 0) {
		printf("Resource was acquired by %d, but is still in-use by %d!\n", procNum, shmp->source);
	}
	shmp->source = procNum;
	int realProcNum = procNum + 1;
	printf("%d using resource...\n", realProcNum);
	
	time_t rawtime;
	struct tm * timeinfo;
	FILE *fptr = fopen("cstest", "a");
	if (fptr == NULL) {
		printf("Error: unable to open cstest file.\n");
		exit(0);
	}
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	fprintf(fptr, "%d:%d:%d Queue %d File modified by process number %d\n", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, shmp->tickets[procNum], realProcNum);
	fclose(fptr);
	
	MEMBAR;
	
	shmp->source = 0;
}

char *getPerror(char *programName) {
	char* output = strdup(programName);
	strcat(output, ": Error");
	return output;
}

void logMessage(char *message, int procNum, char *fileName) {
	time_t rawtime;
        struct tm * timeinfo;
	time(&rawtime);
        timeinfo = localtime(&rawtime);
	FILE *fptr = fopen(fileName, "a");
        if (fptr == NULL) {
                printf("Error: unable to open log file.\n");
                exit(0);
        }
        fprintf(fptr, "%d:%d:%d %s%d\n", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, message, procNum);
        fclose(fptr);	
}
