#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main (int argc, char *argv[]) {
   //***VARIABLE DECLARATION***
   int opt;
   int slaves = 0;
   
   //***GETOPT***
   while((opt = getopt(argc, argv, "hn:c:s:i:")) != -1)
   {
      switch(opt)
      {
            case'h':
            printf("Usage:\nchain [-h] [-p nprocs] [-c nchars] [-s sleeptime] [-i niters] < textfile\nnprocs Number of processes [default 4]\nnchars Number of characters read into the buffer [default 80]\nsleeptime Time to sleep in each iteration [default 3s]\nniters Numbner of iterations in the loop\ntextfile File containing text to be read through stdin\n");
            return 0;
            break;
            
            case'n':
            slaves = atoi(optarg);
            if(slaves>20){
               printf("Slaves cannot be more than 20\n");
               slaves = 0;
               exit(0);
            }else{
            }
            break;
            
            case'c':
            break;
            
            case's':
            break;
            
            case'i':
            break;
      }
   }
   
   return 0;
}
