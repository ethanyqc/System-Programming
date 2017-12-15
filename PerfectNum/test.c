#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <signal.h>
#include "hdrtest.h"

shared *sm;
int sid;

int main(int argc, char *argv[]){
  sid = shmget(MEMKEY,sizeof(shared),IPC_CREAT |0660);//created shared memmory using the KEY
  sm = shmat(sid,0,0);//attach the shared memory

  for(int i=0;i<20;i++){
    printf("%d\n",sm->perfectnum[i]);
  }
}
