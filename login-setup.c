/*
author:junaid
description:The following code facilitates simple pin security
            while opening terminal if installed into system by
            running "sudo make install". The program accepts once
            arguement which is paprent's process id. The code will
            ask to create new password if it is first run on the system

*/
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<signal.h>
//ctrl+z
#define SIGSTP 20
int ppid;

void __kill_parent(){
  kill(ppid,SIGKILL);
}

sigset_t* __get_sigset_to_block(sigset_t *set){
  sigemptyset(set);
  sigaddset(set,SIGINT);
  sigaddset(set,SIGSTP);
  sigaddset(set,SIGABRT);
  sigaddset(set,SIGQUIT);
  sigaddset(set,SIGKILL);
  return set;
}

void __clear_screen(){
  fflush(stdout);
  system("clear");
}

int main(int argc,char* argv[]){
 //data
 int fd,c=0,attempt=0,attempt_set=0;
 char cnt,pin[4],pass[4],b_rdr;
 sigset_t set,old;
 char *file = "/tmp/passwd";
 ppid = atoi(argv[1]);

 //mask all interuptable signals
 sigprocmask(SIG_SETMASK,__get_sigset_to_block(&set),&old);
 //open password file
 fd = open(file,O_RDONLY|O_CREAT,0777);
 //read 4digit pin from file
 fflush(stdin);
 if(read(fd,&pin,4)==4){
    //we don't need any open file here, so close it immediately
    close(fd);

    //prompt user to enter the pin to unlock terminal
    while(++attempt){
      //let's clear the screen
      printf("\n\tENTER 4 DIGIT PIN :");
      scanf("%4s",pass);
      //alternate to fflush(stdin) coz it doesnt work properly in care of stdin
      do
         b_rdr=getchar();
      while(b_rdr != '\n' && b_rdr != EOF);
      //compare entered and stored pass
      __clear_screen();
      if(strcmp(pin,pass)==0)
        break;
      else
        printf("INCORRECT PIN" );

      //close the terminal once if user makes 5 unsuccessfull attempts
      if(attempt>=5)
      {
        //let's close the terminal after 2 timers, ie: 15 attempts
        if(attempt_set++ >= 3)
        {
          attempt=3;
          while(attempt--){
          fflush(stdout);
          sleep(1);
          system("clear");
          printf("You are banned!\nExiting terminal");
          }
          __kill_parent();
        }

        attempt=15;
        while(attempt--){
        fflush(stdout);
        sleep(1);
        system("clear");
        printf("You made maximum attempts,wait %d seconds to try again",attempt);
        }
       //to get back into loop
       attempt=0;
      }
     }
   }
 else{
    //making sure that file is closed
    close(fd);
    //open file again to write password
    fd = open(file,O_WRONLY|O_CREAT,O_TRUNC);
    while(1){
      printf("\nCREATE 4 DIGIT PIN :");
      scanf("%4s",pass);
      //verify the pin length, if it is not equal to 4 then ask once again
      if(sizeof(pass) != 4)
        continue;
      //write pin to file
      write(fd,pass,4);
      break;
    }
    system("clear");
  }
    system("clear");
  //give access to terminal
  return 0;
}
