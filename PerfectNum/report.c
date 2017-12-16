/* THIS CODE IS MY OWN WORK. IT WAS WRITTEN WITHOUT CONSULTING
 A TUTOR OR CODE WRITTEN BY OTHER STUDENTS -YCHE498*/
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <signal.h>
#include <string.h>
#include "hdrtest.h"

shared *sm;
int sid;
//message *msg;
int main(int argc, char *argv[]){
  sid = shmget(MEMKEY,sizeof(shared),IPC_CREAT |0660);//created shared memmory using the KEY
  sm = shmat(sid,0,0);//attach the shared memory

  for(int i=0;i<20;i++){
    if(sm->prfctNms[i]!=0) printf("%d ",sm->prfctNms[i]);
  }

  //TODO: print process info
  printf("\n");
  for(int j=0;j<20;j++){
    if(sm->prc[j].pid!=0)
      printf("pid:%d, perfectCt:%d, testCt:%d, skipCt:%d, total:%d\n",sm->prc[j].pid,sm->prc[j].perfectCt,sm->prc[j].testCt,sm->prc[j].skipCt,sm->prc[j].perfectCt+sm->prc[j].testCt+sm->prc[j].skipCt);
  }


  if(argc>=2){
    if(strcmp(argv[1],"-k")==0){
      if(kill(sm->mPid, SIGINT)==-1) perror("kill fail");
    }
  }
}
