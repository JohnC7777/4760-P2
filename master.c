#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include "config.h"

int maxTime;
int slaves;
int shmAllocated;
int shmid;
void *shmp;
char *programName;
int activeProcesses;
const int SHM_KEY = 777;
const int SHM_SIZE = 1024;
const int SHM_PERM = 0666;


char *getOutputPerror();
void childTermHandler(int);
void ctrlCHandler(int);

int main (int argc, char *argv[]) {
	signal(SIGINT, ctrlCHandler);
	signal(SIGCHLD, childTermHandler);
	programName = argv[0];
	maxTime = 100;
	slaves = 0;
	activeProcesses = 0;
	int opt;
	void *shmp;
	int shmAllocated=0;
	

   //***GETOPT***
while((opt = getopt(argc, argv, "hn:t:")) != -1){
	switch(opt){
			
	case'h':
	printf("Usage:\nchain [-h] [-n nprocs] [-t ss maxTime] \nnprocs Number of processes \nmaxTime maximum time in seconds (default 100 seconds) after which the process should terminate itself if not completed. \n");
	return 0;
	break;

	case'n':
	slaves = atoi(optarg);
	if(slaves>20){
		printf("Slaves cannot be more than 20\n");
		slaves = 0;
		exit(0);
	}
	break;
		      
	case't':
	maxTime = atoi(optarg);
      }
   }
	shmid = shmget(SHM_KEY, SHM_SIZE, SHM_PERM|IPC_CREAT);
	if (shmid == -1) {
		char *output = getOutputPerror();
		perror(output);
		return 1;
	}
	
	shmp = shmat(shmid, NULL, 0);
	if (shmp == (void *) -1) {
		char *output = getOutputPerror();
		perror(output);
		return 1;
	}  
 
   return 0;
}
void childTermHandler(int s) {
	activeProcesses--;
	if (activeProcesses < 1) {
		printf("All children have terminated. Now exiting program...\n");
		logTermination("all children terminated");
		endProgramHandler(1);
	}		
}
void ctrlCHandler(int s) {
	logTermination("ctrl+C");
	endProgramHandler(1);
}
char *getOutputPerror () {
	char* output = strdup(programName);
	strcat(output, ": Error");
	return output;
}
