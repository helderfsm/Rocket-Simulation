#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <g2.h>
#include <g2_X11.h>
#include <g2_PS.h>
#include <g2_gd.h>



/* Gravity */
#define g 9.8065
/* Maximum size of the buffer to receive data */
#define MAX_BUF 1024



// FUNCTIONS FOR G2------------------------------------------------
void openg2 (int *d) // inicialize g2 window
{
int dev;
   *d = g2_open_vd ();
   dev = g2_open_X11(800, 600);
   g2_attach(*d, dev);
   g2_clear(*d);
   g2_set_font_size(*d,16); 
   g2_set_auto_flush(*d,1); 
   g2_set_background (*d, 0); //gackgroud color 0=white
}

void closeg2 (int d) //close g2 window
{ 
   g2_flush(d);
   g2_close(d);
}

void data_display (float vx, float vz, float px, float pz, float al, float t, float f, int d)
{      
  char data_1[100];
  char data_2[100];
  char data_3[100];
  char data_4[40];
  char data_5[25];
  g2_clear(d);                           /*Limpa a janela*/
  g2_pen(d, 1);                            /*Escolhe a cor preta*/
  sprintf(data_1, "Horizontal vel: %0.2f [m/s]        Vertical vel: %0.2f [m/s]", vx, vz); 
  sprintf(data_2, "Horizontal pos: %0.2f [m]      Altitude: %0.2f [m]", px, pz); 
  sprintf(data_3, "Alpha: %0.2f [rad]                       Thrust: %0.2f [N]", al, t);
  sprintf(data_4, "Fuel : %0.1f%% ", f);
  sprintf(data_5,"Autopilot ");
  g2_string(d, 50, 550, data_1);
  g2_string(d, 50, 530, data_2);
  g2_string(d, 50, 510, data_3);
  g2_string(d, 50, 490, data_4);
  g2_string(d, 50, 470, data_5);
}





///MAIN----------------------------------------------------------------


int main() {
  // G2
  // Variables to g2
  int d;  
  //inicialize g2 window
  openg2(&d);



  /* Creating the pipe to receive data from controller*/
  /* will receive thrust and alpha */
  int fd_from_controller;
  char * from_controller_pipe = "\tmp\controller_dynamics_pipe";
  char data_from_controller[MAX_BUF];
  fd_from_controller = open(from_controller_pipe, O_RDONLY | O_NONBLOCK);

  /* Creating the pipe to send data to controller */
  /* will send reference velocities and mass */
  int fd_to_controller;
  char * to_controller_pipe = "\tmp\dynamics_controller_pipe";
  mkfifo(to_controller_pipe, 0665);
  fd_to_controller = open(to_controller_pipe, O_WRONLY | O_NONBLOCK);

  /* sample time for the dynamics = 50 [ms] */
  float delta_t = 0.05;


  float x = 0.0;      /* position [m] */
  float z = 0.0;      /* altitude [m] */
  float v_x = 0.0;    /* velocity in x [m/s] */
  float v_z = 0.0;    /* velocity in z [m/s] */
  float a_x = 0.0;    /* acceleration in x [m/s2] */
  float a_z = 0.0;    /* acceleration in z [m/s2] */
  float m = 1000.0;   /* rocket mass w/ fuel = 1000 [Kg] */
  float m_i = 500.0;  /* rocket mass w/out fuel = 500 [Kg] */
  float m_fi = 500.0; /* initial fuel mass = 500 [Kg] */
  float m_f = m_fi;   /* fuel mass = 500 [Kg] */
  float thrust = 0.0;
  float alpha = 0.0;

  /* Fuel quality = 0.0005 [kg/N.s] */
  float B = 0.0005;

  // flag from autopilot
  int flag=1;
 

  while(1)
  {
    /* Opens pipe from controller */
    while (fd_from_controller < 0) {
      //printf("Error opening controller to dynamics pipe\n");
      fd_from_controller = open(from_controller_pipe, O_RDONLY | O_NONBLOCK);
    }
    /* Reads the control variables sent from controller */
    /* and atribute each value for thrust and alpha */
    read(fd_from_controller, data_from_controller, MAX_BUF);
    sscanf(data_from_controller, "%f,%f,%d", &thrust, &alpha, &flag);
    //printf("Received from controller: thrust: %f alpha: %f\n", thrust, alpha);

    /* ============= Dynamics Equations ============ */
    m_f = m_f - B*thrust*delta_t;
    if (m_f < 0) {m_f = 0; thrust = 0;}
    m = m_i + m_f;
    float fuelPerc = (m_f/m_fi*100);
    a_x = -thrust*sin(alpha)/(m) + B*thrust*v_x/(m);
    a_z = thrust*cos(alpha)/(m) + B*thrust*v_z/(m) - g;
    v_x = v_x + a_x*delta_t;
    v_z = v_z + a_z*delta_t;
    x = x + v_x*delta_t + 0.5*a_x*pow(delta_t,2);
    z = z + v_z*delta_t + 0.5*a_z*pow(delta_t,2);
    if (z < 0) {z = 0; v_z = 0; x = 0; v_x = 0; alpha = 0;}
    /* ============================================== */

    /* Open the pipe to controller */
    while (fd_to_controller < 0) {
      //printf("Error opening dynamics to controller pipe\n");
      fd_to_controller = open(to_controller_pipe, O_WRONLY | O_NONBLOCK);
    }
    /* Concatenate mass, v_x and v_z as data and send it to dynamics */
    char data_to_controller[128];
    char * pointer = data_to_controller;
    pointer += sprintf(pointer,"%f,", m);
    pointer += sprintf(pointer,"%f,", v_x);
    pointer += sprintf(pointer,"%f", v_z);
    write(fd_to_controller, data_to_controller, sizeof(data_to_controller));

    /* Print of current dynamics */
    /* will previously be done in interface */
   // printf("Fuel: %3d%%\tX: %6d\tV_X: %3.2f\tA_X: %3.2f\ta: %4.1fº\tT: %5d N\n", (int) fuelPerc, (int) x, v_x, a_x, (alpha*57.32), (int) thrust);
   // printf("Fuel: %3d%%\tZ: %6d\tV_Z: %3.2f\tA_Z: %3.2f\ta: %4.1fº\tT: %5d N\n", (int) fuelPerc, (int) z, v_z, a_z, (alpha*57.32), (int) thrust);


    //G2 print flight info
    data_display(v_x,v_z,x,z,alpha,thrust,fuelPerc,d);
    


    //Draw Rocket in position
    g2_pen(d, 1);
    int largura =10;
    int altura=30;
    g2_rectangle (d , x/10+200, z/10, x/10+largura+200,z/10+altura);
    g2_filled_triangle(d,x/10+200, z/10+altura,x/10+largura+200,z/10+altura,x/10+200+largura/2,z/10+altura+15);
    if (flag==1){
      g2_filled_rectangle (d , 120, 470, 140 ,480); // se enable desenha retangulo
      g2_pen(d,7); // 7 green 19 red
    }

    /* Delay 50ms to simulate sample time */
    usleep(50000);
  }
  /* Closes the pipes */
  close(fd_to_controller); 
  close(fd_from_controller);
  /* Unlink the pipes */
  unlink(from_controller_pipe);
  unlink(to_controller_pipe);

  //close g2 win
  closeg2(d);
  return 0;
}


