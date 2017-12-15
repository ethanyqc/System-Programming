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

int pInd;


int perfect(int start){
    int sum = 1;

    for (int i=2;i<start;i++)
      if (!(start%i)) sum+=i;

    if (sum==start) return 1;
    else return 0;

}

//TODO: terminate signals
void terminate(int signum){
  memset(sm->prc, 0, sizeof(sm->prc));
  exit(0);
}


int main(int argc, char *argv[]){

//TODO:Creat sharedn memory
  sid = shmget(MEMKEY,sizeof(shared),IPC_CREAT |0660);//created shared memmory using the KEY
  if(sid<0) perror("shmget");
  sm = shmat(sid,0,0);//attach the shared memory
  if(sm<0) perror("sm");



//TODO:get the current pid
  int curPid = getpid();

//TODO: setup message
  mid = msgget( MSGKEY, IPC_CREAT | 0660 );
  msg = malloc(sizeof(message));

//TODO: setup content
  msg->type = GETINDEX;
  msg->num = curPid;

//TODO: send msg to manage
  msgsnd(mid,msg,sizeof(msg->num),0);

//TODO: rcv message to requst index

  msgrcv(mid, msg, sizeof(msg->num), SENDINDEX, 0);

  pInd = msg->num;




//TODO: setup signals
  struct sigaction action;
  action.sa_handler=terminate;
  sigaction(SIGINT,&action,NULL);
  sigaction(SIGHUP,&action,NULL);
  sigaction(SIGQUIT,&action,NULL);


  int start = atoi(argv[1]);//the argument 1, starting number

//FIXME: index size
  for(int index=start;index<33554432;index++){

      if(check(sm->bits,index)) sm->prc[pInd].skipCt++;
      else{
        if(perfect(index)){
              msg->type = PRFCT;
              msg->num = index;
              msgsnd(mid,msg,sizeof(msg->num),0);
              //printf("%d\n", index);
              sm->prc[pInd].perfectCt++;
        }

        set(sm->bits,index);//once checked,set 1
        sm->prc[pInd].testCt++;
      }

  }


}
