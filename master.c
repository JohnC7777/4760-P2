#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main (int argc, char *argv[]) {
   //***VARIABLE DECLARATION***
   
   
   //***GETOPT***
   while((opt = getopt(argc, argv, "hp:c:s:i:")) != -1)
   {
      switch(opt)
      {
            case'h':
            printf("Usage:\nchain [-h] [-p nprocs] [-c nchars] [-s sleeptime] [-i niters] < textfile\nnprocs Number of processes [default 4]\nnchars Number of characters read into the buffer [default 80]\nsleeptime Time to sleep in each iteration [default 3s]\nniters Numbner of iterations in the loop\ntextfile File containing text to be read through stdin\n");
            return 0;
            break;
            
            case'p':
            numProcs = atoi(optarg);
            break;
            
            case'c':
            numChars = atoi(optarg);
            break;
            
            case's':
            sleeptime = atoi(optarg);
            break;
            
            case'i':
            niters = atoi(optarg);
            break;
      }
   }
   
   return 0;
}
