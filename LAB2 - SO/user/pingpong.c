#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

void pingpong(int n, int is_ping, int my_sem, int other_sem) 
{

  while(0 < n) 
  {
    sem_down(my_sem);
    if(is_ping) 
    {
      printf("    ping\n");
    } else 
    {
      printf("        pong\n");
    }
    --n;
    sem_up(other_sem);
  }
}


int main(int argc, char **argv) 
{
  if(argc < 2) 
  {
    fprintf(2,"ERROR : invalid arguments.\n");
    exit(1);
  }
  int n = atoi(argv[1]);
  
  if (n == 0)
  {
    fprintf(2, "ERROR : invalid parameter.\n");
    exit(1);
  }

  int sem_ping = sem_open(0, 1);
  int sem_pong = sem_open(1, 0);

  if(fork() == 0) 
  {
    // Va a printear n pongs
    pingpong(n, 0, sem_pong, sem_ping);
  } 
  else 
  {
    // Va a printear n pings
    pingpong(n, 1, sem_ping, sem_pong);
    wait(0);
  }

  sem_close(sem_ping);
  sem_close(sem_pong);

  return 0;
}
