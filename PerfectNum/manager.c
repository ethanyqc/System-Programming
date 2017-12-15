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
message *msg;
int mid;
int managePid;
void terminate1(int signum){
  //TODO: kill the _process
  for(int i=0;i<20;i++){
    if(sm->prc[i].pid!=0){
      kill(sm->prc[i].pid, SIGINT);
    }
  }
  //TODO: sleep
  sleep(5);
  //TODO: detach
  shmdt(sm);
  shmctl(sid, IPC_RMID, 0);
  msgctl(mid, IPC_RMID, 0);
  exit(0);
}

int getProcessIndex(){
  int x=-1;
  for(int a=0;a<20;a++){
    if(sm->prc[a].pid==0){
      x = a;
      break;
    }
  }
  return x;
}

int main(int argc, char *argv[]){

  managePid = getpid();
//TODO:Creat sharedn memory
  sid = shmget(MEMKEY,sizeof(shared),IPC_CREAT |0660);//created shared memmory using the KEY
  sm = shmat(sid,0,0);//attach the shared memory

//TODO: signals set up
    struct sigaction action;
    action.sa_handler=terminate1;
    sigaction(SIGINT,&action,NULL);
    sigaction(SIGHUP,&action,NULL);
    sigaction(SIGQUIT,&action,NULL);

//TODO: Intialize share memory
    memset(sm->bits, 0, sizeof(sm->bits));
    memset(sm->prfctNms, 0, sizeof(sm->prfctNms));
    memset(sm->prc, 0, sizeof(sm->prc));
    sm->mPid = managePid;//get mPid of manager andstore

//TODO: setup message
    mid = msgget( MSGKEY, IPC_CREAT | 0660 );

//TODO: keep revieve message
    while(1){
      msg = malloc(sizeof(message));
      msgrcv(mid, msg, sizeof(msg->num), 0, 0);

//TODO: process get index commented

      if(msg->type == GETINDEX){
      //TODO: find free index in process array
          int pi = getProcessIndex();
          if(pi!=-1){
            sm->prc[pi].pid = msg->num;//pid=compute process id
            msg->type = SENDINDEX;//set up msg type
            msg->num = pi;//set up content
            msgsnd(mid, msg, sizeof(msg->num),0);//sent to compute
          }

      }
      if(msg->type == PRFCT){
      //TODO: update sm->prfctNms
          int per = msg->num;
          for(int b=0;b<20;b++){
            if(sm->prfctNms[b]==0){
              sm->prfctNms[b]=per;
              break;
            }
          }
      }


    }//end while

}
