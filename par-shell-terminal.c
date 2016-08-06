#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#include <fcntl.h> // for open
#include <unistd.h> // for close

#include "commandlinereader.h"
#include "list.h"
#define EXIT_GLOBAL_COMMAND "exit-global"
#define EXIT_COMMAND   "exit"
#define MAXARGS        7
#define BUFFER_SIZE    100
#define MAXPAR         2
#define WRITING_PERIOD 10
#define LOG_FILE       "log.txt"




int main(int argc, char **argv) {

	char buffer[BUFFER_SIZE];
	int numargs, Wfifo, Rfifo, i, PIDfifo;
	char *args[MAXARGS];
	char msg [BUFFER_SIZE];
	char pid_msg [BUFFER_SIZE];
		
	if (argc != 1 && argc != 2) {
		printf("Invalid argument count.\n");
		printf("Usage:\n");
		printf("\t%s [MAXPAR]\n\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	if  ((Wfifo = open(argv[1],O_WRONLY)) < 0){
		perror ("Fail to open FIFO for reading");
		exit(EXIT_FAILURE);
	}
	if ((Rfifo = open("par-shell-out",O_RDONLY)) < 0){ 
		perror ("Fail to open FIFO for writing");
		exit(EXIT_FAILURE);
	}

	
	
	printf("Insert your commands:\n");
	while (1) {
	    numargs = readLineArguments(args, MAXARGS);
	    if (numargs < 0) {
	      printf("Error reading arguments\n");
	      continue;
	    }
	    if (numargs == 0) {
	      continue;
	    }
	    if (strcmp(args[0], "stats") == 0){
		strcpy(buffer, args[0]);
	    	for (i=1; i < numargs ; i++){
			strcat(buffer, " ");
			strcat(buffer, args[i]);
		}
		if(write (Wfifo, buffer, BUFFER_SIZE) <= 0){
			perror ("couldnt write on par-shell-in");
			break;
		}
		if(read (Rfifo, &msg, BUFFER_SIZE)<= 0);
		else {
			printf("PROC:\tTEMP:\n%s\n",msg);
	   	}
	    }
		
	    else if (strcmp(args[0], EXIT_GLOBAL_COMMAND) == 0) {
		strcpy(buffer, args[0]);
		if(write (Wfifo, buffer, BUFFER_SIZE) <= 0){
			perror ("couldnt write on par-shell-in");
			break;
		}
	        break;
	   } 
	

	    else if (strcmp(args[0], EXIT_COMMAND) == 0) {
	      	break;
	    }
	    else{ 
			strcpy(buffer, args[0]);
	    		for (i=1; i < numargs ; i++){
				strcat(buffer, " ");
				strcat(buffer, args[i]);
			}


			if(write (Wfifo, buffer, BUFFER_SIZE) <= 0){
				perror ("couldnt write on par-shell-in");
				break;
	    		}
	    	}	
	 
       	}
	    

	close (Wfifo);
	close (Rfifo);
	return EXIT_FAILURE;
}




