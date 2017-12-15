//THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING
 //A TUTOR OR CODE WRITTEN BY OTHER STUDENTS YCHE498
#include <ar.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <utime.h>
#include <time.h>
#include <fcntl.h>
#include <dirent.h>


void ar_t(int arch){
  struct ar_hdr buff;
  lseek(arch,8,SEEK_SET);//jump the magic number
  while(read(arch, &buff, sizeof(struct ar_hdr))){
      int fsize = atoi(buff.ar_size);
      struct ar_hdr fileBuff;//file buff
      snprintf(fileBuff.ar_name,16,"%-16s",buff.ar_name);
      printf("%s\n", fileBuff.ar_name);

      if (fsize%2==1){
        fsize++;
      }
		  lseek(arch,(int)fsize,SEEK_CUR);//jump the whole file
    }
}



void ar_v(int arch){
  struct ar_hdr buff;
  lseek(arch,8,SEEK_SET);//jump the magic number
  while(read(arch, &buff, sizeof(struct ar_hdr))){
      int fsize = atoi(buff.ar_size);
      struct ar_hdr fileBuff;//file buff

      //TODO: name
      snprintf(fileBuff.ar_name,16,"%-16s",buff.ar_name);

      //TODO: mode
      long perm;
      perm = strtol(buff.ar_mode,0,8);
      char str[10];//mode string
      typeof(S_IRUSR) p[9] = {S_IRUSR,S_IWUSR,S_IXUSR,S_IRGRP,S_IWGRP,S_IXGRP,S_IROTH,S_IWOTH,S_IXOTH};
      int i,j,k;
      for(int i=0,j=1,k=2;k<9;i+=3,j+=3,k+=3){
        if(perm&p[i]){
          str[i]='r';
        }else str[i]='-';
        if(perm&p[j]){
          str[j]='w';
        }else str[j]='-';
        if(perm&p[k]){
          str[k]='x';
        }else str[k]='-';
      }

      //TODO: uid gid
      snprintf(fileBuff.ar_uid,6,"%-6s",buff.ar_uid);
      snprintf(fileBuff.ar_gid,6,"%-6s",buff.ar_gid);
      char ugid[14];
      strcpy(ugid,fileBuff.ar_uid);
      strcat(ugid,"/");
      strcat(ugid,fileBuff.ar_gid);
      //printf("%s\n",ugid);

      //TODO: filesize
      snprintf(fileBuff.ar_size,10,"%-10s",buff.ar_size);


      //TODO: Date
      snprintf(fileBuff.ar_date,12,"%-12s",buff.ar_date);
      time_t t;
      struct tm *date;
      t=atol(fileBuff.ar_date);
      ctime(&t);
      date=localtime(&t);
      char calender[21];
      strftime (calender, 21, "%b %d %H:%M %Y ",  date);
      //printf("%s\n",buff);

      //TODO:result
      char resultV[100];
      strcpy(resultV,str);
      strcat(resultV,"     ");
      strcat(resultV,ugid);
      strcat(resultV,fileBuff.ar_size);
      strcat(resultV,calender);
      strcat(resultV,fileBuff.ar_name);

      printf("%s\n",resultV);


      if (fsize%2==1){
        fsize++;
      }
		  lseek(arch,(int)fsize,SEEK_CUR);//jump the whole file
    }
}

void ar_q_test(int argc, char* argv[]){//TODO: int arch, int app
  //open archive
  int arch;//test if archive exist
  arch = open(argv[2],O_RDWR);


  if(arch==-1){
    //TODO: Create new archive
    printf("no archive");
    arch = creat(argv[2],0666);//create archive
    write(arch,"!<arch>\n",8);//write archive top
    close(arch);//close archive
    //******finishe cretae archive

    int i;
    //loop throgh arguments
    for(i=3;i<argc;i++){
      int app;
      arch=open(argv[2],O_RDWR);//open archive !!(reopen)
      app = open(argv[i],O_RDWR);//open the append file, store in app
      lseek(arch,0,SEEK_END);//put to the end
      struct stat file;
      struct ar_hdr buff_empty;
      fstat(app,&file);


      //put file info into empty hdr
      snprintf(buff_empty.ar_name,16,"%-16s",argv[i]);
      snprintf(buff_empty.ar_date,12,"%-12d",(int)file.st_mtime);
      snprintf(buff_empty.ar_uid,6,"%-6d",(int)file.st_uid);
      snprintf(buff_empty.ar_gid,6,"%-6d",(int)file.st_gid);
      snprintf(buff_empty.ar_mode,8,"%-8o",(int)file.st_mode);
      snprintf(buff_empty.ar_size,10,"%-10d",(int)file.st_size);
      snprintf(buff_empty.ar_fmag,2,"%-2s",ARFMAG);

      int size2 = sizeof(struct ar_hdr)+1;
      char str[size2];
      sprintf(str, "%s %s %s %s %s %s %s",buff_empty.ar_name,buff_empty.ar_date,buff_empty.ar_uid,buff_empty.ar_gid,buff_empty.ar_mode,buff_empty.ar_size,ARFMAG);

      //write into archive
      if(write(arch,str,sizeof(struct ar_hdr))==-1){
          printf("Error");
          exit(1);
      }

      int blksz = (int)file.st_blocks;
      char* rb[blksz];
      int size;
      while((size=read(app,rb,blksz))){//read from append fd, store in the rb buff, of read the total of block size
        write(arch,rb,size);
        if (size%2==1){
          write(arch,"\n",1);
        }
      }

      //end line
      close(app);
      //close(arch_tmp);

    }//end for loop

    //close(arch);//close archive
  }

  else if(arch!=-1){
    //TODO: Directly add file into archive
    printf("archive found");
    //arch=open(argv[2],O_RDWR);//open archive !!
    lseek(arch,0,SEEK_END);//put to the end
    int i;
    //loop throgh arguments
    for(i=3;i<argc;i++){
      int app;
      app = open(argv[i],O_RDWR);//open the append file, store in app
      struct stat file;
      struct ar_hdr buff_empty;
      fstat(app,&file);

      snprintf(buff_empty.ar_name,16,"%-16s",argv[i]);
      snprintf(buff_empty.ar_date,12,"%-12d",(int)file.st_mtime);
      snprintf(buff_empty.ar_uid,6,"%-6d",(int)file.st_uid);
      snprintf(buff_empty.ar_gid,6,"%-6d",(int)file.st_gid);
      snprintf(buff_empty.ar_mode,8,"%-8o",(int)file.st_mode);
      snprintf(buff_empty.ar_size,10,"%-10d",(int)file.st_size);
      snprintf(buff_empty.ar_fmag,2,"%-2s",ARFMAG);

      int size2 = sizeof(struct ar_hdr)+1;
      char str[size2];
      sprintf(str, "%s %s %s %s %s %s %s",buff_empty.ar_name,buff_empty.ar_date,buff_empty.ar_uid,buff_empty.ar_gid,buff_empty.ar_mode,buff_empty.ar_size,ARFMAG);


      //write into archive
      if(write(arch,str,sizeof(struct ar_hdr))==-1){
          printf("Error");
          exit(1);
      }

      int blksz = (int)file.st_blocks;
      char* rb[blksz];
      int size;
      while((size=read(app,rb,blksz))){
        write(arch,rb,size);
        if (size%2==1){
          write(arch,"\n",1);
        }
      }


      //end line


      close(app);

    }  //end for loop

  }
  close(arch);

}

void test(char* name){
  printf("%s",name);
}



void ar_A(char* argv[]){

//1. open archive
int arch;
arch=open(argv[2],O_RDWR);
//2. find directory
DIR* dir;
struct dirent* file;


dir=opendir(".");
close(arch);
while ((file=readdir(dir)) != NULL) {

  if(file->d_type == DT_REG && strcmp(file->d_name,".DS_Store")!=0 && strcmp(file->d_name,argv[2])!=0 && strcmp(file->d_name,"a.out")!=0 && strcmp(file->d_name,"myar.c")!=0){
    //printf("%s\n", file->d_name);
    //addFile(argv[2], file->d_name);
    char fileStrName[sizeof(file->d_name)+1];
    strcpy(fileStrName, file->d_name);
    struct stat timeFile;
    stat(fileStrName,&timeFile);
    long modiTime=timeFile.st_mtime;
    //printf("Modified time for %s: %ld\n",fileStrName,modiTime);
    time_t curTime;
    time(&curTime);
    //printf("Current time for %s: %ld\n",fileStrName,curTime);
    if(curTime-modiTime<=3600*2){//if within 2*3600 sec

      //printf("Modified time for %s: %ld\n",fileStrName,modiTime);
      //printf("Current time for %s: %ld\n",fileStrName,curTime);
      char strName[sizeof(file->d_name)+1];
      strcpy(strName, file->d_name);
      int app;
      arch=open(argv[2],O_RDWR);//open archive !!(reopen)
      app = open(strName,O_RDWR);//open the append file, store in app
      lseek(arch,0,SEEK_END);//put to the end
      struct stat file;
      struct ar_hdr buff_empty;
      fstat(app,&file);

      snprintf(buff_empty.ar_name,16,"%-16s",strName);
      snprintf(buff_empty.ar_date,12,"%-12d",(int)file.st_mtime);
      snprintf(buff_empty.ar_uid,6,"%-6d",(int)file.st_uid);
      snprintf(buff_empty.ar_gid,6,"%-6d",(int)file.st_gid);
      snprintf(buff_empty.ar_mode,8,"%-8o",(int)file.st_mode);
      snprintf(buff_empty.ar_size,10,"%-10d",(int)file.st_size);
      snprintf(buff_empty.ar_fmag,2,"%-2s",ARFMAG);

      int size2 = sizeof(struct ar_hdr)+1;
      char str[size2];
      sprintf(str, "%s %s %s %s %s %s %s",buff_empty.ar_name,buff_empty.ar_date,buff_empty.ar_uid,buff_empty.ar_gid,buff_empty.ar_mode,buff_empty.ar_size,ARFMAG);

      //write into archive
      if(write(arch,str,sizeof(struct ar_hdr))==-1){
          printf("Error");
          exit(1);
      }

      int blksz = (int)file.st_blocks;
      char* rb[blksz];
      int size;
      while((size=read(app,rb,blksz))){
        write(arch,rb,size);
        if (size%2==1){
          write(arch,"\n",1);
        }
      }
      //end line

      close(app);

    }

  }

    }

    closedir(dir);
    close(arch);


}

//.out x a.a a.txt
void ar_x(int argc,char* argv[]){


  //#1 check if a file exist in archive
  int fExist=0;
  int arch = open(argv[2], O_RDWR); // open archive
  int i;
  int fSize;

  for(i=3;i<argc;i++){
    struct ar_hdr buff;//create buff
    lseek(arch,8,SEEK_SET);//jump the magic number

    while(read(arch, &buff, sizeof(struct ar_hdr))){

        //get the file size of the reading file
        fSize = (size_t)atoi(buff.ar_size);//get file size
        int len = strlen(argv[i]);
//*************

        struct ar_hdr fileBuff;//file buff
        snprintf(fileBuff.ar_name,16,"%-16s",buff.ar_name);

        if(strncmp(fileBuff.ar_name,argv[i],len)==0){//compare string

            printf("The file %s has %d size\n",fileBuff.ar_name,fSize);//print out the existing files in archive
            int xfile;//extracting file
            xfile = creat(argv[i],0666);//create empty file
            close(xfile);
            int newFile;
            newFile=open(argv[i],O_RDWR);//open the created file, into newFile
//**************

            struct stat arch_st;
            fstat(arch,&arch_st);
            int blksz=(int)arch_st.st_blksize;
            printf("blksize:%d \n", blksz);
            int bufferSize;
            if(fSize<blksz){
              bufferSize=fSize;
            }
            else{
              bufferSize=blksz;
            }
            char* rb[bufferSize];//
            int writeSize;
            while((writeSize=read(arch,rb,fSize))!=0){
              write(newFile,rb,writeSize);
              fSize-=bufferSize;
            }//end while
            //Finished writing
            //TODO:Restore timestamp

            int t = atol(buff.ar_date);
            struct utimbuf tb;
            tb.modtime = t;
            utime(argv[i], &tb);//function can change file metadata.


        }//end if
            if (fSize%2==1){
              fSize++;
            }

            //jump file size similar to t
            lseek(arch,(int)fSize,SEEK_CUR);
      }//end while

  }//end for


    //printf("%d",fExist);
    close(arch);


}



int main(int argc, char* argv[]){

    if(strcmp(argv[1],"q")==0){
      ar_q_test(argc,argv);
    }
    if(strcmp(argv[1],"t")==0){
      int file = open(argv[2], O_RDWR);
      if(file==-1){
        perror("Error");
        exit(1);
      }
      ar_t(file);
    }
    if(strcmp(argv[1],"v")==0){
      int file = open(argv[2], O_RDWR);
      if(file==-1){
        perror("Error");
        exit(1);
      }
      ar_v(file);
    }
    if(strcmp(argv[1],"A")==0){
      int arch = open(argv[2],O_RDWR);
      if(arch==-1){
        perror("Error");
        exit(1);
      }

      ar_A(argv);
    }
    if(strcmp(argv[1],"x")==0){
      int arch = open(argv[2],O_RDWR);
      if(arch==-1){
        perror("Error");
        exit(1);
      }
      ar_x(argc,argv);
    }



    return 0;

}
