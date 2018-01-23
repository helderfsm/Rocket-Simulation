#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

/* Gravity */
#define g 9.8065
/* Maximum size of the buffer to receive data */
#define MAX_BUF 1024
/* Maximum thrust allowed = 50000 [N]*/
#define THRUST_MAX 50000
/* Maximum alpha allowed = 45 [deg]*/
#define ALPHA_MAX  0.785

int main()
{
  /* Creating the pipe to receive data from interface */
  /* will receive reference velocities*/
  int fd_from_interface;
  char * from_interface_pipe = "\tmp\interface_controller_pipe";
  char data_from_interface[MAX_BUF];
  fd_from_interface = open(from_interface_pipe, O_RDONLY | O_NONBLOCK);

  /* Creating the pipe to receive data from dynamics */
  /* will receive reference velocities and mass*/
  int fd_from_dynamics;
  char * from_dynamics_pipe = "\tmp\dynamics_controller_pipe";
  char data_from_dynamics[MAX_BUF];
  fd_from_dynamics = open(from_dynamics_pipe, O_RDONLY | O_NONBLOCK);

  /* Creating the pipe to send data to dynamics */
  /* will send thrust and alpha */
  int fd_to_dynamics;
  char * to_dynamics_pipe = "\tmp\controller_dynamics_pipe";
  mkfifo(to_dynamics_pipe, 0664);
  fd_to_dynamics = open(to_dynamics_pipe, O_WRONLY | O_NONBLOCK);

  /* Init controller variables */
  float thrust = 0.0;
  float alpha = 0.0;
  float v_x_ref = 0.0;
  float v_z_ref = 0.0;
  float v_x = 0.0;
  float v_z = 0.0;
  float mass = 0.0;
  float error_v_x = 0.0;
  float error_v_z = 0.0;
  float last_error_v_x = 0.0;
  float last_error_v_z = 0.0;
  float kp_thrust = 16.8;
  float ki_thrust = 1.8; ///estva 0.5
  float kp_alpha = 2.2;
  float ki_alpha = 0.022;

  /* sample time for the dynamics = 50 [ms] */
  float delta_t = 0.05;

  int flag=1; //flag for autopilot

  while(1)
  {
    /* Opens pipe from interface */
    while (fd_from_interface < 0) {
      //printf("Error opening interface to controller pipe\n");
      fd_from_interface = open(from_interface_pipe, O_RDONLY | O_NONBLOCK);
    }
    /* Reads the reference velocities sent from interface */
    /* and atribute each value for v_x_ref and v_z_ref */
    read(fd_from_interface, data_from_interface, MAX_BUF);
    sscanf(data_from_interface, "%f,%f", &v_x_ref, &v_z_ref);
    //printf("Received from interface; v_x_ref: %0.2f, v_z_ref: %0.2f\n", v_x_ref, v_z_ref);
    if (data_from_interface[0] == 'q'){
      break; 
    }

    /* Opens pipe from dynamics */
    while (fd_from_dynamics < 0){
     // printf("Error opening dynamics to controller pipe\n");
      fd_from_dynamics = open(from_dynamics_pipe, O_RDONLY | O_NONBLOCK);
    }
    /* Reads the current velocities and mass of the UAV sent from dynamics */
    /* and atribute each value for v_x, v_z, mass */
    read(fd_from_dynamics, data_from_dynamics, MAX_BUF);
    sscanf(data_from_dynamics, "%f,%f,%f", &mass, &v_x, &v_z);
    //printf("Received from dynamics: mass: %0.2f, v_x: %0.2f, v_z: %0.2f\n", mass, v_x, v_z);


    //IF AUTOPILOT ON
   if (data_from_interface[0] == 'e'){
      flag=1;
    }


    //IF AUTOPILOT OFF
    if (data_from_interface[0] == 'd'){
      flag=0;
    }



    if (flag==1){ // if autopilot on
      /* ============= Proportional Controller ============ */
      error_v_x = v_x_ref - v_x;
      error_v_z = v_z_ref - v_z;
      thrust = mass*(error_v_z*kp_thrust - g + (error_v_z+last_error_v_z)*ki_thrust*delta_t);
      alpha = -(error_v_x*kp_alpha+(last_error_v_x+error_v_z)*ki_alpha)/abs(error_v_z*kp_thrust - g);
      last_error_v_z += error_v_z;
      last_error_v_x += error_v_x;
      if (thrust < 0) thrust = 0;
      if (thrust > THRUST_MAX) thrust = THRUST_MAX;
      if (alpha < -ALPHA_MAX) {alpha = -ALPHA_MAX; last_error_v_x = 0.0;}
      if (alpha > ALPHA_MAX) {alpha = ALPHA_MAX; last_error_v_x = 0.0;}
      if (thrust == 0) last_error_v_z = 0.0;
      /* ================================================== */
    }
  
    if (flag==0){ // if autopilot off
      thrust=v_x_ref; // from line 74
      alpha=v_z_ref;
      if (thrust < 0) thrust = 0;
      if (thrust > THRUST_MAX) thrust = THRUST_MAX;
      if (alpha < -ALPHA_MAX) {alpha = -ALPHA_MAX;}
      if (alpha > ALPHA_MAX) {alpha = ALPHA_MAX;}
      if (thrust == 0) last_error_v_z = 0.0;
    }


    /* Open pipe to dynamics */
    while (fd_to_dynamics < 0) {
      //printf("Error opening controller to dynamics pipe\n");
      fd_to_dynamics = open(to_dynamics_pipe, O_WRONLY | O_NONBLOCK);
    }
    /* Concatenate thrust and alpha as data and send it to dynamics */
    char data_to_dynamics[128];
    char * pointer = data_to_dynamics;
    pointer += sprintf(pointer,"%f,", thrust);
    pointer += sprintf(pointer,"%f,", alpha);
    pointer += sprintf(pointer,"%d",flag);
    write(fd_to_dynamics, data_to_dynamics, sizeof(data_to_dynamics));

    if (mass == 500) printf("NO FUEL !\n");
    /* Print of current control */
    //else printf("V_X_ref: %3.2f\tV_X: %3.2f\tAlpha: %2.1fº\tV_Z_ref: %3.2f\tV_Z: %3.2f\tThrust: %5dN\n", v_x_ref, v_x, (alpha*57.32), v_z_ref, v_z, (int) thrust);


    /* Delay 50ms to simulate sample time */
    usleep(50000);
  }
  /* Closes pipes */
  close(fd_to_dynamics);
  close(fd_from_interface);
  close(fd_from_dynamics);
  /*  Unlink pipes */
  unlink(from_interface_pipe);
  unlink(from_dynamics_pipe);
  unlink(to_dynamics_pipe);
  return 0;
}
