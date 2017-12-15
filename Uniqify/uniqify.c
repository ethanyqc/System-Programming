//THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING
//A TUTOR OR CODE WRITTEN BY OTHER STUDENTS - YCHE498
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>


//1.Parse sentence into word
//2.sort word
//3.takeout repeat

int main(int argc, char* argv[]){

  int pipe1[2];
  int pipe2[2];

  pid_t child1;
  pid_t child2;

  pipe(pipe1);
  pipe(pipe2);

  if ((child1 = fork()) == -1){
      perror("error child1");
      exit(1);
  }
  //fork the 2nd time
  if ((child2 = fork()) == -1){
      perror("error child2");
      exit(1);
  }


  //child1 process: sort
  if(child1 == 0){

        //char buf[1024];
        close(pipe1[1]);
        close(pipe2[0]);
        dup2(pipe2[1], STDOUT_FILENO);
        dup2(pipe1[0], STDIN_FILENO);
        close(pipe2[1]);
        close(pipe1[0]);
        execlp("sort", "sort",(char *)NULL);
        exit(0);
        //wait(NULL);
  }


  else{//parent process
      close(pipe1[0]);
      FILE *fp = fdopen(pipe1[1],"w");
      char c;
      int chct = 0;//count the character been inputed
      char *word = malloc(36*sizeof(char));//array store word

      while((c = fgetc(stdin))) {

          if(isalpha(c)){//if c is a character

            if(chct <35) {
              //put the chatcer into word array
              word[chct]=tolower(c);
              chct++;
              //character increase
            }
            //end conditon with letter ending
            if (feof(stdin) && chct >= 5 && chct <35){ //if reach the end of file, and word length is between 5 and 35
              word[chct]='\0';//null terminated
              fputs(word,fp);
              fputs("\n",fp);
              break;
            }

          }

          if(!isalpha(c)){//if c is not a character
            if(chct >= 5){
              word[chct]='\0';//null terminated
              fputs(word,fp);//put word in fp
              fputs("\n",fp);//add enter

            }
            //clear word array
            word = malloc(36*sizeof(char));
            chct=0;

            if (feof(stdin)){//if reach end of file. if ending is a special, break
              break;
            }

          }

      }
      fclose(fp);
      close(pipe1[1]);
      wait(NULL);
      sleep(1);


      //TODO:child2 process: surpress
      if(child2==0){
        close(pipe2[1]);
        char prevBuff[37]="";//one extra becuase fgets will read the \n
        char nowBuff[37];
        int ct=0;
        FILE *fl = fdopen(pipe2[0], "r");
        //char s;

        //unique word counter

        while(fgets(nowBuff,sizeof(nowBuff),fl)){
              strtok(nowBuff,"\n");
              if(strcmp(nowBuff, prevBuff)!=0){
                if(strcmp("", prevBuff)!=0){
                  printf("%-35s %5d\n",prevBuff,ct+1);
                  ct=0;
                }
              }
              else ct++;
              strcpy(prevBuff,nowBuff);
        }
        printf("%-35s %5d\n", nowBuff, ct+1);//last

        fclose(fl);
        close(pipe2[0]);
        exit(0);

      }




  }


  return 0;
}
