#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>


/* Maximum size of the buffer to receive data */
#define MAX_BUF 1024

int main()
{
  /* Creating the pipe to send data to controller */
  /* will send thrust and alpha */
  int fd_to_controller_i;
  char * to_controller_pipe_i = "\tmp\interface_controller_pipe";
  char data_to_controller_i[MAX_BUF];
  mkfifo(to_controller_pipe_i, 0666);
  fd_to_controller_i = open(to_controller_pipe_i, O_WRONLY | O_NONBLOCK);
  int flag=1; //flag for autopilot

 
  while(1)
  {
   
    printf("Insert 'd' to desingage autopilot\n");
    printf("Insert 'e' to engage autopilot\n");
    printf("Insert 'q' to quit\n");  
    gets(data_to_controller_i);

    for(int i=1;i<4;i++){
      printf("\n");
    }

    /* Opens the pipe to controller */
    fd_to_controller_i = open(to_controller_pipe_i, O_WRONLY | O_NONBLOCK);
    while (fd_to_controller_i < 0) {
     // printf("Error opening interface to controller pipe\n");
      fd_to_controller_i = open(to_controller_pipe_i, O_WRONLY | O_NONBLOCK);
    }


    if (data_to_controller_i[0] == 'e') { 
      printf("autpilot engaged\n");    
      printf("\n");  
      flag=1;
    }  

    if (data_to_controller_i[0] == 'd') {
      printf("autpilot desingaged\n");
      printf("\n"); 
      flag=0;
    } 

    if (flag ==1){ //if autopilot on
       printf("Insert Reference (v_x_ref, v_z_ref):\n");
    }

     if (flag ==0){ //if autopilot off
       printf("Insert Reference: thrust[0;50000] ','' alfa[-0.785;0.785]: \n");
    }


    /* Sends data to controller */
    /* reference velocities */
    write(fd_to_controller_i, data_to_controller_i, sizeof(data_to_controller_i));


    if (data_to_controller_i[0] == 'q') {
      break;
    }  
     
  }
  /* Close the pipes */
  close(fd_to_controller_i);
  /* Unlink pipes */
  unlink(to_controller_pipe_i);
 
  return 0;
}


